#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define NOT_HUNGRY  1
#define HUNGRY      2
#define EATING      3
#define RIGHT   ((i + 1) % data->n)
#define LEFT    ((i + data->n - 1) % data->n)

typedef struct s_data
{
    int             n;
    int             t_sleep;
    int             t_eat;
    int             *state;
    bool            *locked;
    pthread_mutex_t *state_mutex;
}   t_data;

typedef struct s_arg
{
    t_data  *data;
    int     i;
}   t_arg;

int ft_min(int a, int b)
{
    if (a < b)
        return (a);
    return (b);
}

int ft_max(int a, int b)
{
    if (a > b)
        return (a);
    return (b);
}

// if the LEFT and RIGHT threads are not eating
// and thread number i is hungry, change its state to EATING
// and signal to the while loop in `take_forks` to stop blocking.
// if a thread has a state of HUNGRY then it's guaranteed
// to be out of the critical section of `take_forks`.
void    test(int i, t_data *data)
{
    if (
        data->state[i] == HUNGRY
        && data->state[LEFT] != EATING
        && data->state[RIGHT] != EATING
    )
    {
        data->state[i] = EATING;
        data->locked[i] = false;
    }
}

// set the state of the thread number i to HUNGRY
// and block until the LEFT and RIGHT threads are not EATING
// in which case they will call `test` from `put_forks`
// which will result in breaking the while loop
void    take_forks(int i, t_data *data)
{
    pthread_mutex_lock(data->state_mutex);
    data->locked[i] = true;
    data->state[i] = HUNGRY;
    test(i, data);
    pthread_mutex_unlock(data->state_mutex);
    while (data->locked[i]);
}

// set the state of the thread number i to NOT_HUNGRY
// then signal to the LEFT and RIGHT threads
// so they can start eating when their neighbors are not eating
void    put_forks(int i, t_data *data)
{
    pthread_mutex_lock(data->state_mutex);
    data->state[i] = NOT_HUNGRY;
    test(LEFT, data);
    test(RIGHT, data);
    pthread_mutex_unlock(data->state_mutex);
}

void    *philosopher(void *_arg)
{
    t_arg   *arg = _arg;

    while (true)
    {
        printf("%d is thinking\n", arg->i);
        take_forks(arg->i, arg->data);
        printf("%d is eating\n", arg->i);
        usleep(arg->data->t_eat * 1000);
        put_forks(arg->i, arg->data);
        printf("%d is sleeping\n", arg->i);
        usleep(arg->data->t_sleep * 1000);
    }
    return (NULL);
}

void    data_init(t_data *data, pthread_mutex_t *state_mutex, char **argv)
{
    int i = 0;

    data->n = atoi(argv[1]);
    data->t_eat = atoi(argv[2]);
    data->t_sleep = atoi(argv[3]);
    pthread_mutex_init(state_mutex, NULL);
    data->state_mutex = state_mutex;
    data->state = malloc(data->n * sizeof(int));
    data->locked = malloc(data->n * sizeof(bool));
    while (i < data->n)
    {
        data->state[i] = NOT_HUNGRY;
        data->locked[i] = true;
        i++;
    }
}

int main(int argc, char **argv)
{
    pthread_mutex_t state_mutex;
    t_data          data;
    t_arg           *args;
    pthread_t       *threads;
    int             i;

    if (argc != 4)
    {
        fputs("Error\nInvalid argument count\n", stderr);
        return (1);
    }
    data_init(&data, &state_mutex, argv);
    args = malloc(data.n * sizeof(t_arg));
    i = 0;
    while (i < data.n)
    {
        args[i].data = &data;
        args[i].i = i;
        i++;
    }
    threads = malloc(data.n * sizeof(pthread_t));
    i = 0;
    while (i < data.n)
    {
        pthread_create(threads + i, NULL, philosopher, args + i);
        i++;
    }
    i = 0;
    while (i < data.n)
        pthread_join(threads[i++], NULL);
}
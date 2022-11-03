#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


int number[4] = {1, 2 , 3, 4};

void *eating(void *arg)
{
    int index = *(int*)arg;
    printf("Im using spoon %d\n", number[index]);
    free(arg);
    return 0;
}

int main(int ac, char *av[])
{
    pthread_t philo[4];
    int i ;
    for (i = 0; i < 4; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&philo[i], NULL, &eating, a) != 0)
            perror("error eating");
        }

        for (i = 0; i < 4; i++)
        {
            if (pthread_join(philo[i], NULL) != 0)
            {
                perror("failed to join");
            }
        }
    return 0;
}
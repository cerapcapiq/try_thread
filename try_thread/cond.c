#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex_eat;
pthread_cond_t cond_eat;

int spaghetti = 0;

void* eating(void *arg)
{
    for (int i = 0; i < 10; i++)
    pthread_mutex_lock(&mutex_eat);
    spaghetti += 10;
    printf("im eating  %d\n", spaghetti);
    pthread_mutex_unlock(&mutex_eat);
    pthread_cond_signal(&cond_eat);
    sleep(2);
    
    printf("done eating\n");
    return 0;
}


void* philos(void *arg)
{
     pthread_mutex_lock(&mutex_eat);
     while (spaghetti < 5){
     printf("no food. waiting .. \n");
       pthread_cond_wait(&cond_eat, &mutex_eat);
     }
    spaghetti -= 5;
    printf("im sleeping, my energy %d\n", spaghetti);
    pthread_mutex_unlock(&mutex_eat);
    return 0;
}


int main(int ac, char *av[])
{
    int i ; 
    pthread_t philo[4];

    pthread_mutex_init(&mutex_eat, NULL);
    pthread_cond_init(&cond_eat, NULL);
    for (i = 0; i < 4; i++)
    {
        if (i == 3)
    { 

        if(pthread_create(philo + i, NULL, &eating, NULL) != 0)
        {
            perror("error create");
            return 1;
        }
    }
        else {
            if (pthread_create(&philo[i], NULL, &philos, NULL) != 0)
            {
                perror("failed people");
            }
        }
      printf("philo number %d thinking\n", i);
      sleep(3);
    
 
    }
    for (i = 0; i < 4; i++)
    {   
       if (pthread_join(philo[i], NULL) != 0)
        return 2;
       }
    pthread_mutex_destroy(&mutex_eat);
       pthread_cond_destroy(&cond_eat);
    return 0;

}
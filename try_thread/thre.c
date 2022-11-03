#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex;


void* eat(void *arg)
{
    
    pthread_mutex_lock(&mutex);
    printf("im using the spoon\n");
    
    pthread_mutex_unlock(&mutex);
    sleep(2);
    
    printf("done eating\n");
    return 0;
}


void* sleepingg()
{
    printf("sleeping....zzzz..\n");
    return 0;
}


int main(int ac, char *av[])
{
    int i ; 
    pthread_t philo[4];

    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < 4; i++)
    { 

        if(pthread_create(philo + i, NULL, &eat, &i) != 0)
        {
            perror("error create");
            return 1;
        }
      printf("philo number %d thinking\n", i);
      sleep(3);
    
 
    }
      for (i = 0; i < 4; i++)
    { 
        if(pthread_create(philo + i, NULL, &sleepingg, NULL) != 0)
        {
            perror("error create");
            return 1;
        }
      printf("philo number %d sleeping\n", i);
 
    }
    for (i = 0; i < 4; i++)
    {   
       if (pthread_join(philo[i], NULL) != 0)
        return 2;
       }
    pthread_mutex_destroy(&mutex);
    return 0;

}
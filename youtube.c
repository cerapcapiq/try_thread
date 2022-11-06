#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define philosopher 7 

pthread_mutex_t spoon[philosopher];

int main(int ac, char *av[])
{
    int i, res;

    pthread_t a_thread[philosopher];
    void *thread_func(void *j);
    for(i=0; i <philosopher; ++i)
    pthread_mutex_init(&spoon[i], NULL);

    for(i=0;i<philosopher;++i)
    {
        int *arg = malloc(sizeof(int*));
        *arg = i;
        pthread_create(&a_thread[i],NULL,thread_func,arg);
    }
     for(i=0;i<philosopher;++i)
     pthread_join(a_thread[i], NULL);

     printf("\nthread join successful\n");
       for(i=0;i<philosopher;++i)
       pthread_mutex_destroy(&spoon[i]);

       exit(0);

}

void *thread_func(void *param)
{
    int i; //iteration = 10;
    int n = *(int *)param;
    for (i=0;++i;)
    {
        sleep(1);
        pthread_mutex_lock(&spoon[n]);
        pthread_mutex_lock(&spoon[(n+1)%philosopher]);
        printf("\nBegin eating :%d with spoons %D and %d",n,n, (n+1)%philosopher);
        sleep(2);
        printf("\nFinish eating:%d", n);
        pthread_mutex_unlock(&spoon[n]);
        pthread_mutex_unlock(&spoon[(n+1)%philosopher]);
        printf("\nThinking:%d", n);

    }
    return 0;
}
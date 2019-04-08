#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sched.h>
#include <semaphore.h>


sem_t *empty = NULL;
sem_t *full = NULL;

char buf[10];
int i = 0;
int j = 0;

void *Pro()
{
    while(1)
    {
        sem_wait(empty);
        char *in = &buf[i++%10];
        scanf("%c",in);
        sem_post(full);
    }
}

void *Con()
{
      while(1)
    {
        sleep(1);
        sem_wait(full);
        char *out = &buf[j++%10];
        printf("%d:%c\n",j,*out);
        sem_post(empty);
    }
}

int main()
{
    pthread_t P,C;

    empty = sem_open("Producer",O_CREAT,0666,10);
    full = sem_open("Consumer",O_CREAT,0666,0);

    pthread_create(&P,NULL,Pro,NULL);
    pthread_join(P,NULL);

    pthread_create(&C,NULL,Con,NULL);
    pthread_join(C,NULL);

    sem_close(empty);
    sem_unlink("empty");

    sem_close(full);
    sem_unlink("full");

    return 0;
}

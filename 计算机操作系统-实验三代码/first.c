#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<semaphore.h>
#include<sys/types.h>
#include <sys/wait.h>


sem_t *P1_sig = NULL;
sem_t *P2_sig = NULL;
sem_t *P3_sig = NULL;

int main(int argc, char *argv[])
{
    pid_t pid;
    P1_sig = sem_open("P1_sig",O_CREAT,0666,0);
    P2_sig = sem_open("P2_sig",O_CREAT,0666,0);
    P3_sig = sem_open("P3_sig",O_CREAT,0666,0);

    pid = fork();

    if(pid < 0)
    {
        printf("进程为创建失败！");
        
    }


    else if(pid == 0)
    {
        //sleep(1);
        sem_wait(P1_sig);
        printf("I am the process P2\n");
        sem_post(P1_sig);
        sem_post(P2_sig);
       
         pid = fork();

            if(pid < 0)
            {
                printf("进程为创建失败！");
                
            }

            else if(pid == 0)
            {
                sem_wait(P2_sig);
                sem_wait(P3_sig);
                printf("I am the process P4\n");
              
            }
    }

   else 
    {
        printf("I am the process P1\n");
        sem_post(P1_sig);

        pid = fork();

        if(pid < 0)
        {
        printf("进程为创建失败！");
       
        }

        else if(pid == 0)
        {
            sem_wait(P1_sig);
            printf("I am the process P3\n");
            sem_post(P1_sig);
            sem_post(P3_sig);
            return 0;

        }
    }

    sem_close(P1_sig);
    sem_unlink("P1 signalname");
    sem_close(P2_sig);
    sem_unlink("P2 signalname");
    sem_close(P3_sig);
    sem_unlink("P3 signalname");
    return 0;    

}

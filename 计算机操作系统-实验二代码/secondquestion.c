#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
void main()
{
   pid_t pid;
   pid = fork();
   if(pid>0)
     while(1);
   else if(!pid)
   {
      int ret;
      ret = execl("/usr/bin/vi","vi","/home/mjw/hooks.txt",NULL);
      if(ret==-1)
        printf("excel");
   }
   else if(pid==-1)
     printf("fork");
}

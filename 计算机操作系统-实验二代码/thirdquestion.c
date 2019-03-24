#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void main()
{
	pid_t p1,p2,p3,p4,p5;
	while((p1=fork())==-1);
	if(p1>0)
	{
		printf("p1的pid是%d,ppid是%d\n", getpid(),getppid());
		printf("p3的pid是%d,ppid是%d\n",p1,getpid());
		while((p2=fork())==-1);
		if(p2>0)
		{
			printf("p2的pid是%d,ppid是%d\n", p2,getpid());
		}
		if(p2==0)
		{
			while((p4=fork())==-1);
			if(p4>0)
			{
				printf("p4的pid是%d,ppid是%d\n", p4,getpid());
				while((p5=fork())==-1);
				if(p5>0)
				{
					printf("p5的pid是%d,ppid是%d\n", p5,getpid());
				}
			}

		}
	}
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void main()
{
		pid_t p1,p2,p3,p4,p5;
		while((p1=fork())==-1);
		if(p1>0)
		{
			while((p2=fork())==-1);
			{
				if(p2==0)
				{
					while((p5=fork())==-1);
					{
						if(p5>0)
						{
							while((p4=fork())==-1);
							{
								if(p4==0)
								{
									while(1)
									{
										printf("p4的pid是%d,ppid是%d\n",getpid(),getppid());
									}
								}
								else if(p4>0)
								{
									while(1)
									{
									        printf("p2的pid是%d,ppid是%d\n",getpid(),getppid());
									}
								}
							}
						}
						else if(p5==0)
						{
							while(1)
							{
								printf("p5的pid是%d,ppid是%d\n",getpid(),getppid());
							}
						}
					}
					//使用exit（0）退出程序
					//exit(0);
				}
				else if(p2>0)
				{
					while(1)
					{
						printf("p1的pid是%d,ppid是%d\n",getpid(),getppid());
					}
				}
			}
		}
		else if(p1==0)
		{
			while(1)
			{
				printf("p3的pid是%d.ppid是%d\n",getpid(),getppid());
			}
		}
}

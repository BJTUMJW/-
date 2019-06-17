#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define NULL 0

typedef struct mdf{//MDF结构体
	char username[20];//用户名
	char filename[20];//文件名
	struct mdf *next;
}MDF;

typedef struct ufd{//UFD结构体
	char filename[20];//文件名
	int protect;//文件保护码
	unsigned int length;//文件长度
	struct ufd *next;
}UFD;

typedef struct afd{//AFD结构体
	char filename[20];//文件名
	int protect;//文件保护码
	unsigned int point;//文件读写指针
	struct afd *next;
}AFD;


MDF *pmdf;//全局链表头指针
UFD *pufd;
AFD *pafd;
char UserUFD[20];//已经登陆成功的用户名


void initMDF()//初始化MDF表
{
	FILE *fp;
	pmdf= (MDF*)malloc(sizeof(MDF));
	MDF *p = pmdf;
	if((fp = fopen("MDF.txt", "r+")) == NULL){//打开MDF文件
		puts("the MDF cannot open!\n");
		exit(1);
	}
	while (!feof(fp)){//把MDF文件中的内容装入链表
		p->next = (MDF*)malloc(sizeof(MDF));
		p = p->next;
		fscanf(fp, "%s", p->username);
		fscanf(fp, "%s", p->filename);
	}
	p->next = NULL;
	fclose(fp);
}

void printUFD()//打印MDF表
{
	UFD *p = pufd->next;
	puts("文件名\t\t保护码\t\t文件长度\n");
	while (p){
		printf("%s", p->filename);
		printf("\t\t%d" , p->protect);
		printf("\t\t%d\n", p->length);
		p=p->next;
	}


}
void initUFD(char *name)//初始化UFD表
{
	FILE *fp;
	pufd= (UFD*)malloc(sizeof(UFD));
	UFD *p = pufd;
	if((fp = fopen(name, "r+")) == NULL){
		puts("the UFD cannot open!\n");
		exit(1);
	}
	while (!feof(fp)){//建立UFD链表

		p->next = (UFD*)malloc(sizeof(UFD));
		p = p->next;
		fscanf(fp, "%s", p->filename);
		fscanf(fp, "%d", &p->protect);
		fscanf(fp, "%d", &p->length);
		fgetc(fp);

	}
	p->next = NULL;
	fclose(fp);


}

int checkuser()//检测登陆的用户名
{
	char username[20];
	while(1){
		puts("请输入用户名: \n");
		scanf("%s", username);
		MDF *p = pmdf;
		while(p){
			if(strcmp(username, p->username) == 0){
				strcpy(UserUFD, p->filename);
				initUFD(p->filename);
				printUFD();
				return 1;
			}
			p= p->next;
		}
		puts("同户名不存在\n");

	}
}

void initAFD()//初始化AFD
{
	pafd = (AFD*)malloc(sizeof(AFD));
	pafd->next = NULL;
}

bool create()//创建文件命令
{
	char filename[20];
	UFD *p = pufd->next;
	AFD *pa = pafd;
	puts("请输入要创建的文件名: \n");
	scanf("%s", filename);
	while (p){
		if(strcmp(filename, p->filename) == 0){
			puts("此文件已经存在了!\n");
			return 0;
		}
		if(!p->next)
			break;
		p= p->next;
	}
	p->next = (UFD*)malloc(sizeof(UFD));
	p=p->next;
	strcpy(p->filename, filename);
	p->protect = 2;
	p->length = 0;
	p->next = NULL;

	while(pa->next){//创建文件后加入到AFD
		pa=pa->next;
	}
	pa->next = (AFD*)malloc(sizeof(AFD));
	pa = pa->next;
	strcpy(pa->filename ,filename);
	pa->protect = 2;
	pa->point = 0;
	pa->next = NULL;
	printf("文件创建成功\n");
	return 1;
}

bool _delete()//删除文件命令
{
	char filename[20];
	puts("请输入要删除的文件名: \n");
	scanf("%s", filename);
	UFD *p = pufd;
	UFD *temp;
	while (p->next){
		if(strcmp(filename, p->next->filename) == 0){
			temp = p->next;
			p->next = p->next->next;
			free(temp);
			printf("文件%s删除成功!\n", filename);
			return 1;
		}
		p= p->next;
	}
	puts("要删除的文件不存在!\n");
	return 0;

}

bool open()//打开文件命令
{
	char filename[20];
	unsigned int protect;
	puts("请输入要打开的文件名: \n");
	scanf("%s", filename);

	puts("请输入要打开的文件保护类型: \n");
	scanf("%d", &protect);

	UFD *p = pufd->next;
	AFD *pa = pafd->next;
	while (pa){
		if(strcmp(filename, pa->filename) == 0){
			printf("文件%s已经打开!\n",filename);
			return 1;
		}
		if(!pa->next)
			break;
		pa = pa->next;
	}

	if(!pa)
		pa=pafd;
	while (p){
		if(strcmp(filename, p->filename) == 0){
			pa->next = (AFD*)malloc(sizeof(AFD));
			pa = pa->next;
			strcpy(pa->filename , p->filename);
			pa->protect = protect;
			if(protect == 1)
				pa->point = 0;
			else
				pa->point = p->length;
			pa->next=NULL;
			printf("文件%s已经打开!\n",filename);
			return 1;
		}
		p= p->next;
	}
	puts("要打开的文件不存在!\n");
	return 0;

}

void close()//关闭文件命令
{
	char filename[20];
	UFD *pu = pufd->next;
	puts("请输入要关闭的文件名: \n");
	scanf("%s", filename);
	AFD *p = pafd;
	AFD *temp;
	while (p&&p->next){
		if(strcmp(filename, p->next->filename) == 0){
			temp = p->next;
			p->next = p->next->next;

			if(temp->protect == 2){
				while(pu){
					if(strcmp(temp->filename, pu->filename) == 0){
						pu->length =temp->point;
						break;
					}
					pu = pu->next;
				}
			}


			free(temp);
			printf("文件%s关闭成功!\n", filename);
			return ;
		}
		p= p->next;
	}
		puts("要关闭的文件没有被打开!\n");
}

int read()//读文件命令
{
	char filename[20];
	unsigned int length;
	AFD *p = pafd->next;
	puts("请输入要读的文件名: \n");
	scanf("%s", filename);
	puts("请输入要读的长度\n");
	scanf("%d", &length);

	while (p){
		if(strcmp(filename, p->filename) == 0){
			p->point += length;
			printf("文件%s读取成功!\n", filename);
			return 1;
		}
		p= p->next;
	}
	puts("读取失败文件没有打开过!\n");
	return 0;
}

int write()//写文件命令
{
	char filename[20];
	unsigned int length;
	AFD *p = pafd->next;
	puts("请输入要写的文件名: \n");
	scanf("%s", filename);
	while (p){
		if(strcmp(filename, p->filename) == 0){
			if(p->protect != 2){
				printf("文件%s不可写!\n", filename);
				return 0;
			}
			puts("请输入要写的长度\n");
			scanf("%d", &length);
			p->point += length;
			printf("文件%s写入成功!\n", filename);
			return 1;
		}
		p= p->next;
	}
	puts("写入失败文件没有打开过!\n");
	return 0;


}

void destroy()//释放内存
{
	MDF *pm = pmdf;
	while(pm){
		pmdf = pmdf->next;
		free(pm);
		pm = pmdf;
	}
	AFD *pa = pafd;
	while(pa){
		pafd = pafd->next;
		free(pa);
		pa = pafd;
	}
	UFD *pu = pufd;
	while(pu){
		pufd = pufd ->next;
		free(pu);
		pu = pufd;
	}
}

void saveUFD()//保存UFD文件
{
	FILE *fp;
	UFD *p = pufd->next;
	if((fp = fopen(UserUFD, "w")) == NULL){
		puts("the UFD cannot open!\n");
		exit(1);
	}
	while (p){
		fprintf(fp, "%s", "\n");
		fprintf(fp, "%s%s", p->filename,"\t\t");
		fprintf(fp, "%d%s", p->protect,"\t\t");
		fprintf(fp, "%d", p->length);
		p=p->next;
	}

	fclose(fp);

}

void exit1()//推出系统
{
	AFD *pa = pafd->next;
	UFD *pu = pufd->next;
	while(pa){
		if(pa->protect == 2){
			while(pu){
				if(strcmp(pa->filename, pu->filename) == 0){
					pu->length = pa->point;
					break;
				}
				pu = pu->next;
			}
		}
		pa =pa->next;

	}

	saveUFD();
	printUFD();
	destroy();



}

void operate()//命令识别
{
	while(1){
		char command[20];
		char name[][8] = {"create", "delete", "open", "close","read", "write","exit"};
		puts("请输入命令: \n");
		scanf("%s", command);
		if(strcmp(command, name[0] ) == 0)
			create();
		else if(strcmp(command, name[1] ) == 0)
			_delete();
		else if(strcmp(command, name[2] ) == 0)
			open();
		else if(strcmp(command, name[3] ) == 0)
			close();
		else if(strcmp(command, name[4] ) == 0)
			read();
		else if(strcmp(command, name[5] ) == 0)
			write();
		else if(strcmp(command, name[6] ) == 0)
        {
			exit1();
			return;
		}else
			puts("无效命令，请重新输入:\n");
	}


}

void print()
{
	puts("文件系统\n");
	puts("***使用说明***:\n");
	puts("本文件系统共有三个用户分别是user1 user2 user3\n\
	  系统命令有create, delete, open, close,read, write,exit\
	  \nexit----------------------------------------退出系统");
}

int main()
{
	print();
	initMDF();
	checkuser();
	initAFD();
	operate();//命令识别
	return 0;

}

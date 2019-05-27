#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define M 32 //物理内存块数
#define N 64 //虚拟内存块数



/******************PBA******************/
typedef struct LNode
{
    int data;
    int flag; //访问位
    int modify; //修改位
    struct LNode *next;
}LNode;
typedef struct Link
{
    int num;//当前链表上的结点数
    LNode *next;
}Link;

void generate();//生成访问序列
bool isInNodes(int n);
void addToLink(int data, int type);
void emptyIdle();
void emptyModi();
void PBA(int n);
void PBA_ALL();//处理随机生成器生成的页面序列并计算缺页率

int p;//工作集的起始位置
int table[32];//物理内存，每一个元素代表一个页面
LNode *nodes;//改进型Clock置换算法用到的数据结构
Link idle;
Link modified;


/**********************FIFO*******************/
typedef struct node
{
    int num;
    struct node *next;
}Node, *pNode;
typedef struct queue
{
    int n;
    pNode front;
    pNode rear;

}Queue, *pQueue;

void initQueue(pQueue q);//初始化队列
void push(pQueue q, int num);//队列中加入新的页面结点
void pop(pQueue q);//将页面移出内存
void destroy (pQueue q);//销毁队列
bool findInQueue (pQueue q, int num);//查找页面是否已经调入内存
void fifo(pQueue q, int num);//先进先出置换算法实现函数
void FIFO_ALL();//处理随机生成器生成的页面序列并计算缺页率

/**********************LRU/OPT*********************/
bool isInMemo(int n);//指定页号是否已经在内存中

void LRU(int n);//LRU算法实现函数
void LRU_ALL();//处理随机生成器生成的页面序列并计算缺页率

void OPT(int n);//OPT算法实现函数
void OPT_ALL();//处理随机生成器生成的页面序列并计算缺页率

int size;//给进程分配的内存的大小
int lost; //缺页数
int access[32]; //访问序列
int index;//指示当前下标
int memo[3]={-1,-1,-1};

int main()
{
    size = 3;
    lost = 0;
    index = 0;
    PBA_ALL();

    size = 3;
    lost = 0;
    FIFO_ALL();

    size = 3;
    for(int i=0;i<size;i++)
    {
        memo[i]=-1;
    }
    lost = 0;
    index = 0;
    LRU_ALL();

    for(int i=0;i<size;i++)
    {
        memo[i]=-1;
    }
    lost = 0;
    index = 0;
    OPT_ALL();

    return 0;
}

/*********生成页面访问序列********/
void generate()
{
    srand ( (unsigned) time (NULL)); //用时间做种，每次产生随机数不一样
    p = rand() % 64;
    int m = 8, e = 8;
    int i, j;
    double t;
    t = rand()%10/10.0;
    for (i = 0; i < 4; i++)
    {
        for (j=i*m; j<(i+1)*m;j++)
        {
            access[j] = (p + rand() % e) % 64;
        }
        double r = (rand() % 10) / 10.0;
        if (r < t)
        {
            p = rand() % 64;
        }
        else
        {
            p = (p + 1) % 64;
        }
    }
    printf("生成的页面随机访问序列：\n");
    for(int i=0;i<32;i++)
    {
        printf("%d ", access[i]);
    }
    printf("\n");
}
/***********PBA算法所需**********/
void PBA_ALL()
{
    int i = 0, j = 0;
    generate();
    printf ("页面缓冲置换算法(PBA)\n");
    idle.num = 0;
    idle.next = NULL;
    modified.num = 0;
    modified.next = NULL;
    nodes = (LNode*) malloc (size * sizeof (LNode));

    for (i = 0; i < size; i++)
    {
        nodes[i].data = -1;
        nodes[i].flag = 0;
        nodes[i].modify = 0;
        nodes[i].next = NULL;
    }

    for (i = 0; i < 32; i++)
    {
        PBA (i);
        for (j = 0; j < size; j++)
        {
            printf ("%d ", nodes[j].data);
        }
        printf ("\n");
    }
    printf ("页面缓冲置换算法（PBA）缺页率：%f   %d\n", lost / 32.0, lost);
    return 0;
}


bool isInNodes (int n)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        if (nodes[i].data == access[n])
        {
            return true;
        }
    }
    return false;
}

LNode* isinLinks (int n)
{
    LNode *p,*q;
    p = idle.next;
    q = NULL;

    while (p)
    {
        if (p->data == access[n])
        {
            if (q != NULL)
            {
                q->next = p->next;
                p->next = NULL;
                idle.num--;
                break;
            }
            else
            {
                idle.next = NULL;
            }
        }
        q = p;
        p = p->next;
    }
    if (p == NULL)
    {
        p = modified.next;

        while (p != NULL)
        {
            if (p->data == access[n])
            {
                if (p == modified.next)
                { modified.next = p->next; }

                else
                {
                    q->next = p->next;
                    p->next = NULL;
                    modified.num--;
                }

                if (modified.num == 0)
                { modified.next = NULL; }
                break;
            }
            q = p;
            p = p->next;
        }
    }
    return p;
}

void PBA (int n)
{
    if (isInNodes (n))
    {
        printf ("已装入内存\n");
    }
    else
        if (index == size)
        {
            struct LNode *p;
            if ( (p = isinLinks (n)) != NULL)
            {
                nodes = (LNode*) realloc (nodes, (size + 1) * sizeof (LNode));
                nodes[size] .data = p->data;
                nodes[size].flag = p->flag;
                nodes[size].modify = p->modify;
                nodes[size].next = p->next;
                free (p);
                size++;
                index++;
            }
            else
            {
                lost++;//缺页
                if (nodes[n % 3].modify == 1)
                {
                    addToLink (nodes[n % 3].data, 1);
                }
                else
                {
                    addToLink (nodes[n % 3].data, 0);
                }
                nodes[n % 3].data = access[n];
                nodes[n % 3].flag = 1;
                nodes[n % 3].next = NULL;
                if (rand() % 10 < 4)
                {
                    nodes[n % 3].modify = 0;
                }
                else
                {
                    nodes[n % 3].modify = 1;
                }
            }
        }
        else
        {
            nodes[index].data = access[n];
            nodes[index].flag = 1;
            nodes[index].next = NULL;
            if (rand() % 10 < 4)
            {
                nodes[index].modify = 1;
            }
            else
            {
                nodes[index].modify = 0;
            }
            index++;
        }
}

void addToLink (int data, int type)
{
    LNode* p;
    LNode* q;
    q = (LNode*) malloc (sizeof (LNode));
    q->data = data;
    q->flag = 1;
    if (type == 1)
    {
        q->modify = 1;
        p = modified.next;
    }
    else
    {
        q->modify = 0;
        p = idle.next;
    }
    q->next = NULL;
    if (p == NULL)
    {
        if (type == 0)
        {
            idle.next = q;
        }

        else
        {
            modified.next = q;
        }
    }
    else
    {
        while (p)
        {
            if (p->next == NULL)
            {
                p->next = q;
                break;
            }

            else
            {
                p = p->next;
            }
        }
    }
    if (type == 0)
    {
        idle.num += 1;
        if (idle.num == 10)
        {
            emptyIdle();
        }
    }
    else
    {
        modified.num += 1;

        if (modified.num == 10)
        {
            emptyModi();
        }
    }
}

void emptyIdle ()
{
    LNode* p;
    p = idle.next;

    while (p)
    {
        idle.next = p->next;
        free (p);
        p = idle.next;
    }

    idle.num = 0;
}

void emptyModi()
{
    LNode* p;
    p = modified.next;
    while (p)
    {
        modified.next = p->next;
        free (p);
        p = modified.next;
    }
    modified.num = 0;
}






/************FIFO算法所需************/
//FIFO
void FIFO_ALL()
{
    Queue q;
    pNode p;
    initQueue (&q);
    int i = 0;
    printf("\n");
    printf ("先进先出置换算法：\n");
    for (; i < 32; i++)
    {
        fifo(&q, access[i]);
        p = q.front->next;
        while (p)
        {
            printf ("%d ", p->num);
            p = p->next;
        }
        printf ("\n");
    }
    printf ("先进先出算法缺页率：%f  %d\n", lost / 32.0, lost);
    destroy (&q);
}

//初始化队列
void initQueue(pQueue q)
{
    q->rear = (pNode) malloc (sizeof (Node));
    if (q->rear == NULL)
    {
        printf ("failed\n");
    }
    else
    {
        q->front = q->rear;
        q->rear->next = NULL;
        q->front->next = NULL;
        q->n = 0;
    }
}

//插入页表
void push(pQueue q, int num)
{
    pNode p = (pNode) malloc (sizeof (Node));
    if (p == NULL)
    {
        printf ("failed");
    }
    else
    {
        p->next = NULL;
        p->num = num;
        if (q->front == q->rear)
        {
            q->front->next = p;
            q->rear = p;
        }
        else
        {
            q->rear->next = p;
            q->rear = p;
        }
        q->n++;
    }
}

//删除页表
void pop(pQueue q)
{
    pNode p;

    if (q->front != q->rear)
    {
        p = q->front->next;
        q->front->next = p->next;
        if (p == q->rear)
        {
            q->front = q->rear;
        }
        q->n--;
        free (p);
    }
}

//删除队列
void destroy(pQueue q)
{
    while (q->front != q->rear)
    {
        pop (q);
    }
}

//查找页表
bool findInQueue (pQueue q, int num)
{
    pNode p;
    if (q->front != q->rear)
    {
        p = q->front->next;
        while (p)
        {
            if (p->num == num)
            {
                return true;
            }
            else
            {
                p = p->next;
            }
        }
    }
    return false;
}
//FIFO算法
void fifo(pQueue q, int num)
{
    if (findInQueue (q, num))
    {
        printf ("已装入内存\n");
    }
    else
    {
        if(q->n==size)
        {
            pop(q);
            push(q, num);
            lost++;
        }
        else
        {
            push(q, num);
        }
    }
}

/********************LRU算法所需******************/
//执行函数
void LRU_ALL()
{
    int i;
    printf("\n");
    printf("最近最久未使用算法\n");
    for (i = 0; i < 32; i++)
    {
        LRU (i);
        printf ("%d %d %d\n", memo[0], memo[1], memo[2]);
    }
    printf ("最近最久未使用缺页率： %2f %d \n", lost / 32.0, lost);
}

bool isInMemo(int n)
{
    int i = 0;
    for (; i < size; i++)
    {
        if (access[n] == memo[i])
        {
            return true;
        }
    }
    return false;
}

void LRU(int n)
{
    int i, j;
    if (isInMemo(n))
    {
        printf ("已经装入内存\n");
    }
    else
        if (index == size)
        {
            int max = n, pos = -1, tag;

            for (i = 0; i < size; i++)
            {
                for (j = n - 1; j >= 0; j--)
                {
                    if (access[j] == memo[i])
                    {
                        tag = j;
                        break;
                    }
                }
                if (tag < max)
                {
                    max = tag;
                    pos = i;
                    if (max == 0)
                    {
                        break;
                    }
                }
            }
            memo[pos] = access[n];
            lost++;
        }
        else
        {
            memo[index] = access[n];
            index++;
        }
}

/**********************OPT算法所需******************/
void OPT_ALL()
{
    int i = 0;
    printf("\n");
    printf("最佳置换算法：\n");
    for (; i < 32; i++)
    {
        OPT(i);
        printf ("%d %d %d\n", memo[0], memo[1], memo[2]);
    }
    printf ("最佳置换算法缺页率： %2f %d\n", lost / 32.0, lost);
}

void OPT(int n)
{
    int i = 0, j = 0;
    if (isInMemo (n))
    {
        printf ("页面已被调入\n");
    }
    else
        if (index == size)
        {
            lost++;
            int max = 0, pos, tag;
            for (i = 0; i < size; i++)
            {
                tag = -1;
                for (j = n + 1; j < 32; j++)
                {
                    if (access[j] == memo[i])
                    {
                        tag = j;
                        break;
                    }
                }
                if (tag == -1)
                {
                    max = 32;
                    pos = i;
                    break;
                }
                else
                {
                    if (max < tag)
                    {
                        max = tag;
                        pos = i;
                    }
                }
            }
            memo[pos] = access[n];
        }
        else
        {
            memo[index] = access[n];
            index++;
        }
}









#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define M 32 //�����ڴ����
#define N 64 //�����ڴ����



/******************PBA******************/
typedef struct LNode
{
    int data;
    int flag; //����λ
    int modify; //�޸�λ
    struct LNode *next;
}LNode;
typedef struct Link
{
    int num;//��ǰ�����ϵĽ����
    LNode *next;
}Link;

void generate();//���ɷ�������
bool isInNodes(int n);
void addToLink(int data, int type);
void emptyIdle();
void emptyModi();
void PBA(int n);
void PBA_ALL();//����������������ɵ�ҳ�����в�����ȱҳ��

int p;//����������ʼλ��
int table[32];//�����ڴ棬ÿһ��Ԫ�ش���һ��ҳ��
LNode *nodes;//�Ľ���Clock�û��㷨�õ������ݽṹ
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

void initQueue(pQueue q);//��ʼ������
void push(pQueue q, int num);//�����м����µ�ҳ����
void pop(pQueue q);//��ҳ���Ƴ��ڴ�
void destroy (pQueue q);//���ٶ���
bool findInQueue (pQueue q, int num);//����ҳ���Ƿ��Ѿ������ڴ�
void fifo(pQueue q, int num);//�Ƚ��ȳ��û��㷨ʵ�ֺ���
void FIFO_ALL();//����������������ɵ�ҳ�����в�����ȱҳ��

/**********************LRU/OPT*********************/
bool isInMemo(int n);//ָ��ҳ���Ƿ��Ѿ����ڴ���

void LRU(int n);//LRU�㷨ʵ�ֺ���
void LRU_ALL();//����������������ɵ�ҳ�����в�����ȱҳ��

void OPT(int n);//OPT�㷨ʵ�ֺ���
void OPT_ALL();//����������������ɵ�ҳ�����в�����ȱҳ��

int size;//�����̷�����ڴ�Ĵ�С
int lost; //ȱҳ��
int access[32]; //��������
int index;//ָʾ��ǰ�±�
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

/*********����ҳ���������********/
void generate()
{
    srand ( (unsigned) time (NULL)); //��ʱ�����֣�ÿ�β����������һ��
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
    printf("���ɵ�ҳ������������У�\n");
    for(int i=0;i<32;i++)
    {
        printf("%d ", access[i]);
    }
    printf("\n");
}
/***********PBA�㷨����**********/
void PBA_ALL()
{
    int i = 0, j = 0;
    generate();
    printf ("ҳ�滺���û��㷨(PBA)\n");
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
    printf ("ҳ�滺���û��㷨��PBA��ȱҳ�ʣ�%f   %d\n", lost / 32.0, lost);
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
        printf ("��װ���ڴ�\n");
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
                lost++;//ȱҳ
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






/************FIFO�㷨����************/
//FIFO
void FIFO_ALL()
{
    Queue q;
    pNode p;
    initQueue (&q);
    int i = 0;
    printf("\n");
    printf ("�Ƚ��ȳ��û��㷨��\n");
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
    printf ("�Ƚ��ȳ��㷨ȱҳ�ʣ�%f  %d\n", lost / 32.0, lost);
    destroy (&q);
}

//��ʼ������
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

//����ҳ��
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

//ɾ��ҳ��
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

//ɾ������
void destroy(pQueue q)
{
    while (q->front != q->rear)
    {
        pop (q);
    }
}

//����ҳ��
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
//FIFO�㷨
void fifo(pQueue q, int num)
{
    if (findInQueue (q, num))
    {
        printf ("��װ���ڴ�\n");
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

/********************LRU�㷨����******************/
//ִ�к���
void LRU_ALL()
{
    int i;
    printf("\n");
    printf("������δʹ���㷨\n");
    for (i = 0; i < 32; i++)
    {
        LRU (i);
        printf ("%d %d %d\n", memo[0], memo[1], memo[2]);
    }
    printf ("������δʹ��ȱҳ�ʣ� %2f %d \n", lost / 32.0, lost);
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
        printf ("�Ѿ�װ���ڴ�\n");
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

/**********************OPT�㷨����******************/
void OPT_ALL()
{
    int i = 0;
    printf("\n");
    printf("����û��㷨��\n");
    for (; i < 32; i++)
    {
        OPT(i);
        printf ("%d %d %d\n", memo[0], memo[1], memo[2]);
    }
    printf ("����û��㷨ȱҳ�ʣ� %2f %d\n", lost / 32.0, lost);
}

void OPT(int n)
{
    int i = 0, j = 0;
    if (isInMemo (n))
    {
        printf ("ҳ���ѱ�����\n");
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









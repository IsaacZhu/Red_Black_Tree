#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "timecount.h"

enum COLOR {BLACK,RED};

typedef struct rbnode{
    int key;
    int color;
    struct rbnode* p;
    struct rbnode* lchild;
    struct rbnode* rchild;
}rbnode;

rbnode *T;  //T应当是全局的

//用数组实现一个队列
//入队
int enqueue(rbnode *queue[100],int tail,rbnode *newnode,int qlevel[100],int level){
    queue[tail]=newnode;
    qlevel[tail]=level;
    tail++;
    return tail;
}

//出队
rbnode* dequeue(rbnode *queue[100],int head){
    return queue[head++];
}

//进行层次遍历，以观察红黑树信息
void LevelTraverse(rbnode *T){
    rbnode *queue[100];
    int qlevel[100];    //记录层次
    int level=0;
    rbnode* x;
    int head=0;
    int tail=0;
    tail=enqueue(queue,tail,T,qlevel,level);

    printf("LevelTraverse Start\n");
    while (head != tail){
        x = dequeue(queue,head);
        head++;
        if (qlevel[head-1]>level) {
            level++;
            printf("\n");
        }
        
        printf("x's level:%d  key:%d color:%s lchild.key:%d rchild.key:%d \n",
            level,x->key,x->color==BLACK?"BLACK":"RED",
            x->lchild==NULL?0:x->lchild->key,x->rchild==NULL?0:x->rchild->key);

        if (x->lchild!=NULL){       //若左孩子不空，入队
            tail = enqueue(queue,tail,x->lchild,qlevel,level+1);
        }
        if (x->rchild!=NULL){       //若右孩子不空，入队
            tail = enqueue(queue,tail,x->rchild,qlevel,level+1);
        }
    }//while
    printf("LevelTraverse End\n");
}//LevelTraverse

//左旋
void LEFT_ROTATE(rbnode* x){
    rbnode *y = x->rchild;
    if (y == NULL) return;  //TODO:仍需仔细考虑
    y->p = x->p;
    if (T == x)  T = y;     //如果x就是根节点，现在根节点就变为y
    else if (x == x->p->lchild) x->p->lchild = y; //否则，y就成为x的父亲的孩子
    else x->p->rchild = y;
    x->rchild = y->lchild;
    y->lchild = x;
    x->p = y;
}

//右旋
void RIGHT_ROTATE(rbnode* x){
    rbnode *y = x->lchild;
    if (y == NULL) return;  //TODO:仍需仔细考虑
    y->p = x->p;
    if (T == x)  T = y;     //如果x就是根节点，现在根节点就变为y
    else if (x == x->p->lchild) x->p->lchild = y; //否则，y就成为x的父亲的孩子
    else x->p->rchild = y;
    x->lchild = y->rchild;
    y->rchild = x;
    x->p = y;
}

//调整
//注意在RB_INSERT中，已经保证了z不是根节点
void RB_INSERT_FIXUP(rbnode* z){
    rbnode* y;

    while (z->p != NULL && z->p->color == RED){     //父子颜色均为红色，需要调整 
        //这里还有一个先决条件是z->p不空，否则z已经是根节点，应当结束循环
        if  (z->p->p == NULL){      //z是根节点的儿子 此时不用处理
            break;
        }
        else {
            if (z->p == z->p->p->lchild){   //z父亲是爷爷的左孩子
                y = z->p->p->rchild;    //y是z的。。。大伯？
                if (y == NULL){         //考虑y是空，相当于y是黑色
                    if (z == z->p->rchild){    //case 2
                        z = z->p;                   //case 2
                        LEFT_ROTATE(z);           //case 2
                    }
                    else{   //case 3
                        z->p->color = BLACK;    //case 3
                        z->p->p->color = RED;   //case 3
                        RIGHT_ROTATE(z->p->p);//case 3
                    }
                }
                else if (y->color == RED){   //满足case1，即修改一层颜色
                    z->p->color = BLACK;
                    y->color = BLACK;
                    z->p->p->color = RED;
                    z = z->p->p;
                }//else if y red
                else if (z == z->p->rchild){    //case 2
                    z = z->p;                   //case 2
                    LEFT_ROTATE(z);           //case 2
                }//else if

                else {
                    //添加对z.p 及z.p.p为空的处理
                    if (z->p == NULL){      //z.p为空，只可能是case 1   此时z是根节点
                        break;              //结束循环，等待根节点被改成黑色即可。
                    }
                    else if (z->p->p == NULL){  //可能为case 1，也可能为case 2
                        z->p->color = BLACK;    //修改z父亲的颜色即可
                        break;                  
                    }
                    else{
                        z->p->color = BLACK;    //case 3
                        z->p->p->color = RED;   //case 3
                        RIGHT_ROTATE(z->p->p);//case 3
                    }
                }
            }//if z父亲是爷爷左孩子
            
            else {  //z父亲是爷爷的右孩子 该处理过程与上面的过程对称  
                y = z->p->p->lchild;    //y是z的。。。大伯？
                if (y == NULL){         //考虑y是空，相当于y是黑色
                    if (z == z->p->lchild){    //case 2
                        z = z->p;                   //case 2
                        RIGHT_ROTATE(z);           //case 2
                    }
                    else{   //case 3
                        z->p->color = BLACK;    //case 3
                        z->p->p->color = RED;   //case 3
                        LEFT_ROTATE(z->p->p);//case 3
                    }
                }
                else if (y->color == RED){   //满足case1，即修改一层颜色
                    z->p->color = BLACK;
                    y->color = BLACK;
                    z->p->p->color = RED;
                    z = z->p->p;
                }//if y red
                else if (z == z->p->lchild){    //case 2
                    z = z->p;                   //case 2
                    RIGHT_ROTATE(z);           //case 2
                }//else if

                //case3 添加对z.p 及z.p.p为空的处理
                else{
                    if (z->p == NULL){      //z.p为空，只可能是case 1   此时z是根节点
                        break;              //结束循环，等待根节点被改成黑色即可。
                    }
                    else if (z->p->p == NULL){  //可能为case 1，也可能为case 2
                        z->p->color = BLACK;    //修改z父亲的颜色即可
                        break;                  
                    }
                    else{
                        z->p->color = BLACK;    //case 3
                        z->p->p->color = RED;   //case 3
                        LEFT_ROTATE(z->p->p);//case 3
                    }
                }//else  case3
            }//else z父亲是爷爷右孩子
        }//else (z->p->p!=NULL)
    }//while

    T->color = BLACK;   //无脑保证根节点颜色为黑
}//RB_INSERT_FIXUP

//插入
void RB_INSERT(int newkey){
    if (T->key == 0){   //树是空树,直接作为根节点即可
        T->key = newkey;
        T->color = BLACK;
        T->p = T->lchild = T->rchild = NULL;
        return;
    }
    
    rbnode* z;          //建新节点
    z = (rbnode *)calloc(1,sizeof(rbnode));
    z->color = RED;
    z->key = newkey;
    z->lchild = NULL;
    z->rchild = NULL;

    rbnode *x = T;      //搜索插入位置
    rbnode *y;          //y用于记录x搜索时的前驱
    while (x != NULL){
        y = x;
        if (x->key > newkey)  x = x->lchild;
        else x = x->rchild;
    }

    if (y->key > newkey) y->lchild = z; //将新节点插入树中
    else y->rchild = z;
    z->p = y;

    RB_INSERT_FIXUP(z);   //调整
}

int main(){
    //init
    int i,array[64],j;
    int scale[5]={12,24,36,48,60};
    FILE *fp,*fpt;

    //input
    fp=fopen("input.txt","r+");
	for (i=0;i<60;++i){
		fscanf(fp,"%d",&array[i]);
	}	
	fclose(fp);

	
	//fp=fopen("output.txt","w+");
	fpt=fopen("time.txt","w+");
	//fprintf(fp,"********result*********\n");
	fprintf(fpt,"********result*********\n");


	printf("**************CALCULATION START...****************\n");
    //Insertion
    for (i=0;i<5;++i){
        T=(rbnode *)calloc(1,sizeof(rbnode));
        T->key=0;   //以此说明T是空树
        timestart();
        for (j=0;j<scale[i];++j){
            RB_INSERT(array[j]);
        }
        LevelTraverse(T);//tc
        exit(0);//tc
        timeend();
        //output time to time.txt
		fprintf(fpt,"\nN:%d time is %.8lfs\n",scale[i],returntime());
		//output time to screen
		outputtime();
    }
    printf("\n**************CALCULATION END**********************\n");
    //fclose(fp);
	fclose(fpt);
}
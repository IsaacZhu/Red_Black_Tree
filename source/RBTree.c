#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "timecount.h"

enum COLOR {BLACK,RED};

typedef struct rbnode{
    int key;
    int color;
    int size;   //记录结点数
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
        
        printf("x's level:%d  key:%d color:%s size:%d p.key:%d lchild.key:%d rchild.key:%d \n",
            level,x->key,x->color==BLACK?"BLACK":"RED",x->size,
            x->p==NULL?0:x->p->key,
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

//前序遍历红黑树，并输出
void preorder(rbnode* root){
    rbnode* x = root;
    printf("x's key:%d color:%s size:%d p.key:%d lchild.key:%d rchild.key:%d \n",
            x->key,x->color==BLACK?"BLACK":"RED",x->size,
            x->p==NULL?0:x->p->key,
            x->lchild==NULL?0:x->lchild->key,x->rchild==NULL?0:x->rchild->key);
    if (root->lchild != NULL) preorder(root->lchild);
    if (root->rchild != NULL) preorder(root->rchild);
}

//中序遍历红黑树，并输出
void inorder(rbnode* root){
     if (root->lchild != NULL) inorder(root->lchild);
    rbnode* x = root;
    printf("x's key:%d color:%s size:%d p.key:%d lchild.key:%d rchild.key:%d \n",
            x->key,x->color==BLACK?"BLACK":"RED",x->size,
            x->p==NULL?0:x->p->key,
            x->lchild==NULL?0:x->lchild->key,x->rchild==NULL?0:x->rchild->key);
    if (root->rchild != NULL) inorder(root->rchild);
}

//后序遍历红黑树，并输出
void postorder(rbnode* root){
    if (root->lchild != NULL) postorder(root->lchild);
    if (root->rchild != NULL) postorder(root->rchild);
    rbnode* x = root;
    printf("x's key:%d color:%s size:%d p.key:%d lchild.key:%d rchild.key:%d \n",
            x->key,x->color==BLACK?"BLACK":"RED",x->size,
            x->p==NULL?0:x->p->key,
            x->lchild==NULL?0:x->lchild->key,x->rchild==NULL?0:x->rchild->key);
}

//左旋
void LEFT_ROTATE(rbnode* x){
    rbnode *y = x->rchild;
    if (y == NULL) return;  //TODO:仍需仔细考虑
    y->p = x->p;
    if (T == x)  T = y;     //如果x就是根节点，现在根节点就变为y
    else if (x == x->p->lchild) x->p->lchild = y; //否则，y就成为x的父亲的孩子
    else x->p->rchild = y;
    x->rchild = y->lchild;
    if (y->lchild != NULL) y->lchild->p = x;
    y->lchild = x;
    x->p = y;
    y->size = x->size;
    x->size = (x->lchild!=NULL?x->lchild->size:0)+(x->rchild!=NULL?x->rchild->size:0)+1;
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
    if (y->rchild != NULL) y->rchild->p = x;
    y->rchild = x;
    x->p = y;
    y->size = x->size;
    x->size = (x->lchild!=NULL?x->lchild->size:0)+(x->rchild!=NULL?x->rchild->size:0)+1;
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
        T->size = 1;
        T->p = T->lchild = T->rchild = NULL;
        return;
    }
    
    rbnode* z;          //建新节点
    z = (rbnode *)calloc(1,sizeof(rbnode));
    z->color = RED;
    z->key = newkey;
    z->size = 1;
    z->lchild = NULL;
    z->rchild = NULL;

    rbnode *x = T;      //搜索插入位置
    rbnode *y;          //y用于记录x搜索时的前驱
    while (x != NULL){
        y = x;
        y->size ++;     //size加1
        if (x->key > newkey)  x = x->lchild;
        else x = x->rchild;
    }

    if (y->key > newkey) y->lchild = z; //将新节点插入树中
    else y->rchild = z;
    z->p = y;

    RB_INSERT_FIXUP(z);   //调整
}

//查找
rbnode* TREE_SEARCH(int key){
    rbnode* x = T;
    while (x != NULL){
        if (x->key > key) x = x->lchild;
        else if (x->key < key) x = x->rchild;
        else return x;
    }
    return NULL;
}

//移植
void RB_TRANSPLANT(rbnode* u, rbnode* v){
    if (u->p == NULL){   //u为根节点
        T = v;
    }
    else if (u == u->p->lchild){ //u为左孩子
        u->p->lchild = v;
    }
    else{               //u为右孩子
        u->p->rchild = v;
    }

    if (v != NULL) v->p = u->p;
}

//返回以root为根的子树的最小的节点 
//实际用于找后继
rbnode* TREE_MINIMUM(rbnode* root){
    rbnode* x;
    rbnode* y;
    x = root;
    y = x;
    while (x->lchild != NULL){
        y = x;
        x = x->lchild;
    }
    return y;
}

//删除后的调整
void RB_DELETE_FIXUP(rbnode* x){
    rbnode* w;
    while (x != T && x!=NULL && x->color == BLACK){    
        if (x == x->p->lchild){ //x是左孩子
            w = x->p->rchild;
            if (w == NULL){

            }
            else if (w->color == RED){      //case 1
                w->color = BLACK;
                x->p->color = RED;
                LEFT_ROTATE(x->p);
                w = x->p->rchild;
            }
            else if (w->color == BLACK && 
                (w->lchild == NULL || w->lchild->color == BLACK) && 
                (w->rchild == NULL || w->rchild->color == BLACK)){      //case 2
                w->color = RED;
                x = x->p;
            }
            else if (w->rchild == NULL || w->rchild->color == BLACK){   //case 3
                w->lchild->color = BLACK;
                w->color = RED;
                RIGHT_ROTATE(w);
                w = x->p->rchild;
            }
            else{           //case 4
                w->color = x->p->color;
                x->p->color = BLACK;
                w->rchild->color = BLACK;
                LEFT_ROTATE(x->p);
                x = T; 
            }
        }//if
        else{   //x是右孩子  与左孩子情况完全对称
            w = x->p->lchild;
            if (w == NULL){

            }
            else if (w->color == RED){      //case 1
                w->color = BLACK;
                x->p->color = RED;
                RIGHT_ROTATE(x->p);
                w = x->p->lchild;
            }
            else if (w->color == BLACK && 
                (w->lchild == NULL || w->lchild->color == BLACK) && 
                (w->rchild == NULL || w->rchild->color == BLACK)){      //case 2
                w->color = RED;
                x = x->p;
            }
            else if (w->lchild == NULL || w->lchild->color == BLACK){   //case 3
                w->rchild->color = BLACK;
                w->color = RED;
                LEFT_ROTATE(w);
                w = x->p->lchild;
            }
            else{           //case 4
                w->color = x->p->color;
                x->p->color = BLACK;
                w->lchild->color = BLACK;
                RIGHT_ROTATE(x->p);
                x = T; 
            }
        }//else
    }//while
}//RB_DELETE_FIXUP

//删除
void RB_DELETE(rbnode* z){
    if (!z) return; //z为空，结束
    rbnode* y = z;
    int y_original_color = y->color;
    rbnode* x;
    if (z->lchild == NULL && z->rchild != NULL){ //z左孩子空，把右孩子移植上来即可
        //调整路径上的size
        x = T;
        while (x != z){
            y = x;
            y->size --;     //size减1
            if (x->key > z->key)  x = x->lchild;
            else x = x->rchild;
        }
        RB_TRANSPLANT(z,z->rchild);
    }
    else if (z->rchild == NULL && z->lchild != NULL){ //z右孩子空，把左孩子移植上来即可
        //调整路径上的size
        x = T;
        while (x != z){
            y = x;
            y->size --;     //size减1
            if (x->key > z->key)  x = x->lchild;
            else x = x->rchild;
        }
        RB_TRANSPLANT(z,z->lchild);
    }
    else if (z->lchild == NULL && z->rchild == NULL){   //z左右孩子均为空,直接删除即可
        //调整路径上的size
        x = T;
        while (x != z){
            y = x;
            y->size --;     //size减1
            if (x->key > z->key)  x = x->lchild;
            else x = x->rchild;
        }
        if (z == z->p->lchild) z->p->lchild = NULL;
        else z->p->rchild = NULL;
    }
    else{
        y = TREE_MINIMUM(z->rchild);    //找z的后继y
        //调整路径上的size
        x = T;
        while (x != y){
            x->size --;     //size减1
            if (x->key > y->key)  x = x->lchild;
            else x = x->rchild;
        }

        y_original_color = y->color;
        x = y->rchild;
        if (y->p == z){    //若y是z的儿子
            if (x != NULL) x->p = y;      //无效操作
        }
        else{
            RB_TRANSPLANT(y,y->rchild); //用y的右儿子替换y
            y->rchild = z->rchild;
            y->rchild->p = z;
        }
        RB_TRANSPLANT(z,y); //用y替换z
        y->lchild = z->lchild;
        y->lchild->p = y;
        y->color = z->color;
        y->size = z->size;

        if (y_original_color == BLACK){     //y原为黑色，则右分支黑高度减少，需要调整
            RB_DELETE_FIXUP(x);
        }
    }//else
}//RB_DELETE

//找出子树x中第i小的元素
rbnode* OS_SELECT(rbnode* x,int i){
    if (x == NULL) return NULL; //找不到

    int r = (x->lchild == NULL?0:x->lchild->size) + 1;  //顺序
    if (i == r) return x;   //找到了
    else if (i < r) return OS_SELECT(x->lchild,i);      //向左子树继续寻找
    else return OS_SELECT(x->rchild,i-r);               //向右子树继续寻找
}

//对输入的数组，用输入的主元进行划分
//返回低区的元素数
int PARTITION(int pivot,int* array,int n){
    int i = -1;
    int j,tmp,flag;
    flag = 0;
    for (j=0;j<n-1;++j){
        if (array[j] < pivot){  //低区元素
            ++i;
            tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
        }
        else if (array[j] == pivot){    //把枢纽值放到最后。只执行一次
            if (flag == 0){
                array[j] = array[n-1];
                array[n-1] = pivot;
                if (array[j] < pivot){  //低区元素
                    ++i;
                    tmp = array[i];
                    array[i] = array[j];
                    array[j] = tmp;
                }
                flag = 1;
            }
        } 
    }
    tmp = array[i+1];
    array[i+1] = array[n-1];
    array[n-1] = tmp;
    return i+1;
}

//选择算法 
int SELECT(int* array,int n,int i){
    if (n == 1) return array[0];
    int j,k,key,m;
    int midarray[n/5+1];  //共n/5组
    //对每组插入排序，并将其中位数取出
    for (j=0;j<n/5;j++){
        //组内插入排序
        for (k=5*j+1;k<5*(j+1);++k){
            key = array[k];
            //将key插入
            m = k-1;
            while (m > 5*j-1 && array[m] > key){
                array[m+1] = array[m];
                m--;
            }
            array[m+1] = key;
        }
        //将中位数取出
        midarray[j] = array[5*j+2]; 
    }
    //最后一组（不足5个）进行排序
    for (k=5*j+1;k<n;++k){
        key = array[k];
        //将key插入
        m = k-1;
        while (m > 5*j-1 && array[m] > key){
            array[m+1] = array[m];
            m--;
        }
        array[m+1] = key;
    }

    //将其中位数取出
    midarray[n/5] = array[(5*j+n-1)/2];

    //递归调用SELECT，求出中位数的中位数
    int midofmid = SELECT(midarray,n/5+1,(n/5+2)/2);

    //利用partition，对输入数组进行划分
    int low = PARTITION(midofmid,array,n);
    
    if (i == low+1){    //那么中位数的中位数就是第i小的元素
        return midofmid;
    }
    else if (i < low+1){    //在低区继续寻找
        int arraylow[low];
        memcpy(arraylow,array,low*sizeof(int));
        return SELECT(arraylow,low,i);
    }
    else{               //在高区寻找i-k小的元素
        int arrayhigh[n-low-1];
        memcpy(arrayhigh,array+low+1,(n-low-1)*sizeof(int));
        return SELECT(arrayhigh,n-low-1,i-low-1);
    }
}//SELECT

int main(){
    //init
    int i,array[64],j,n,tmparray[64];
    rbnode* tmp;
    int scale[5]={12,24,36,48,60};
    FILE *fp,*fpt;

    //input
    fp=fopen("input.txt","r+");
	for (i=0;i<60;++i){
		fscanf(fp,"%d",&array[i]);
	}	
	fclose(fp);

    memcpy(tmparray,array,64*sizeof(int));
	
	fpt=fopen("time.txt","w+");
	fprintf(fpt,"********result*********\n");


	printf("**************CALCULATION START...****************\n\n");
    //Insertion
    for (i=0;i<5;++i){
        T=(rbnode *)calloc(1,sizeof(rbnode));
        T->key=0;   //以此说明T是空树
        timestart();
        for (j=0;j<scale[i];++j){
            RB_INSERT(array[j]);
        }

        n = T->size;
        
        tmp = OS_SELECT(T,n/3);
        //检验：
        if (tmp->key != SELECT(tmparray,n,n/3)){
            printf("OS_SELECT error!\n");
        }
        RB_DELETE(tmp);
        
        tmp = OS_SELECT(T,n/4);
        //检验：
        if (tmp->key != SELECT(tmparray,n,n/4)){
            printf("OS_SELECT error!\n");
        }
        RB_DELETE(tmp);
        //LevelTraverse(T);//tc
        //exit(0);//tc
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
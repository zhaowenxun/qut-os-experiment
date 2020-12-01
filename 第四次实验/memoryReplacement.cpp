#include<cstdio>
#include<algorithm>
#include<queue>
#include<random>
#include<ctime>
#include<string>

using namespace std;
int phy;//物理块
int phyBlock[25];//物理地址
int Block[25];//物理块号
int sim[5][25];//记录模拟结果
queue<int> q;
/**
 * @auther zwx
 * 系统采用固定分配，局部置换的策略
 **/
//随机生成数据 假设页面大小为 1KB(1024B)
void init(){
    srand((unsigned)(time(NULL)));
    printf("系统随机生成页面访问序列地址：");
    for(int i=1;i<=20;i++){
        phyBlock[i]=(rand()%(phy*1024));
        printf("%d: %d\n",i,phyBlock[i]);
        Block[i]=phyBlock[i]/1024;//所对应的物理块号
    }
}
//先将程序执行一遍，将模拟的结果记录着二维数组中，在模拟接收后按照平是做题的表格进行输出
bool queue_find(int x){
    int len=q.size();
    int t;
    bool f=false;
    while(len--){
        t=q.front();
        q.pop();
        if(t==x)f=true;
        q.push(t);
    }
    return f;
}
// FIFO 
void fifo(){
    memset(sim,0,sizeof(sim));
    while(q.size())q.pop();
    //采用队列的特性的排序页面置换进入的时间

    for(int i=1;i<=20;i++){
        if(queue_find(Block[i]){ //如果物理块中已经有了本页面 无需置换
            if(i!=1){
                for(int l=1;l<=phy;l++)
                    sim[i][l]=sim[i-1][l];
            }
            else 
                sim[i][1]=Block[i];
        }
        else if(q.size()<phy)//如果当前物理块中不满 也无需置换
        {
            q.push(Block[i]);
            int l=q.size();
            for(int l=1;l<=phy;l++)
                    sim[i][l]=sim[i-1][l];
            sim[i][l]=Block[i];
        }
        else { //需要置换

        }
    }

}
//LRU 
void lru(){

}
// OPT
void opt(){

}


int main (){
    while(true){
        printf("****内存页面置换算法的设计****\n");
        while(true){
            printf("物理块数选择：3 or 4\n");
            scanf("%d",phy);
            if(phy==3||phy==4)break;
            printf("输入错误,请重新输入\n");
        }
        init();//随机生成数据
        fifo();
        lru();
        opt();
        printf("****Finish!****");
    }

    return 0;
}

#include <cstdio>
#include <algorithm>
#include <queue>
#include <random>
#include <ctime>
#include <cstring>
/*
*@author qut-zwx
*/
using namespace std;
int phy;                                                                         //物理块
int phyBlock[25];                                                                //物理地址
int Block[25] = {0, 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1}; //物理块号
int sim[5][25];                                                                  //记录模拟结果
queue<int> q;
struct block_time
{
    int no, time;
};
struct time_cmp
{ //重载优先队列为按照访问时间升序
    bool operator()(const block_time a, const block_time b)
    {
        return a.time > b.time;
    }
};
priority_queue<struct block_time, vector<block_time>, struct time_cmp> lru_p;
/**
 * @auther zwx
 * 系统采用固定分配，局部置换的策略
 **/
//随机生成数据 假设页面大小为 1KB(1024B) 内存大小为8
void init()
{
    //    srand(time(NULL));
    //    printf("系统随机生成页面访问序列地址：\n");
    //    for(int i=1;i<=20;i++)
    //        printf("%6d",i);
    //    printf("\n");
    //    for(int i=1;i<=20;i++){
    //        phyBlock[i]=(rand()%(8*1024));
    //        printf("%6d",phyBlock[i]);
    //        Block[i]=phyBlock[i]/1024;//所对应的物理块号
    //    }
    printf("\n访问页面号:");
    for (int i = 1; i <= 20; i++)
        printf("%6d", Block[i]);
    puts("");
}

bool block_find(int t, int x)
{ //检查物理块中有没有当前页面
    if (t == 1)
        return false;
    for (int i = 1; i <= phy; i++)
        if (sim[i][t - 1] == x)
            return true;
    return false;
}
//先将程序执行一遍，将模拟的结果记录着二维数组中，在模拟接收后按照平是做题的表格进行输出
// FIFO
void fifo()
{
    printf("\n采用FIFO页面置换算法\n");
    memset(sim, -1, sizeof(sim));
    while (q.size())
        q.pop();
    //采用队列的特性的排序页面置换进入的时间
    int miss = 0;
    for (int i = 1; i <= 20; i++)
    {
        if (i != 1)
        { //先将物理块中的序列复制一下
            for (int l = 1; l <= phy; l++)
                sim[l][i] = sim[l][i - 1];
        }
        if (block_find(i, Block[i]))
        { //如果物理块中已经有了本页面 无需置换
            continue;
        }
        else if (q.size() < phy) //如果当前物理块中不满 也无需置换
        {
            q.push(Block[i]);
            int l = q.size();
            sim[l][i] = Block[i];
            miss++;
        }
        else
        { //需要置换
            int test = q.front();
            q.pop();
            q.push(Block[i]);
            for (int l = 1; l <= phy; l++)
            {
                if (sim[l][i - 1] == test)
                    sim[l][i] = Block[i];
            }
            miss++;
        }
    }
    printf("物理块序列：\n");
    for (int i = 1; i <= phy; i++)
    {
        printf("内存块号 %d:", i);
        for (int j = 1; j <= 20; j++)
            if (sim[i][j] == -1)
                printf("      ");
            else
                printf("%6d", sim[i][j]);
        printf("\n");
    }
    printf("缺页次数为：%d 缺页率为：%.2f \n", miss, 100.0 * miss / 20);
}
//LRU
void lru()
{ //使用优先队列模拟大根堆，采用最近访问时间排序
    printf("\n采用LRU页面置换算法\n");
    memset(sim, -1, sizeof(sim));
    int del[10];            //用于记录当前访问时间，帮助优先队列实现懒惰删除
    int miss = 0, book = 0; //book记录当前物理块中的页面数
    struct block_time ans;
    for (int i = 1; i <= 20; i++)
    {
        ans.no = Block[i];
        del[ans.no] = i;
        ans.time = i;
        lru_p.push(ans);
        if (i != 1)
        { //先将物理块中的序列复制一下
            for (int l = 1; l <= phy; l++)
                sim[l][i] = sim[l][i - 1];
        }
        if (block_find(i, Block[i]))
        { //如果物理块中已经有了本页面 无需置换
            continue;
        }
        else if (book < phy) //如果当前物理块中不满 也无需置换
        {
            sim[++book][i] = Block[i];
            miss++;
        }
        else
        { //需要置换
            ans = lru_p.top();
            lru_p.pop();
            while (del[ans.no] != ans.time)
            { //检查是否被懒惰删除
                ans = lru_p.top();
                lru_p.pop();
            }
            for (int l = 1; l <= phy; l++)
            {
                if (sim[l][i - 1] == ans.no)
                    sim[l][i] = Block[i];
            }
            miss++;
        }
    }
    printf("物理块序列：\n");
    for (int i = 1; i <= phy; i++)
    {
        printf("内存块号 %d:", i);
        for (int j = 1; j <= 20; j++)
            if (sim[i][j] == -1)
                printf("      ");
            else
                printf("%6d", sim[i][j]);
        printf("\n");
    }
    printf("缺页次数为：%d 缺页率为：%.2f \n", miss, 100.0 * miss / 20);
}
// OPT
void opt()
{
    printf("\n采用OPT页面置换算法\n"); //把物理块中的页号对未来将要访问到的页面进行遍历，
    memset(sim, -1, sizeof(sim));      //找到未来最久没被访问的页面
    int miss = 0, book = 0;            //book记录当前物理块中的页面数
    for (int i = 1; i <= 20; i++)
    {
        if (i != 1)
        { //先将物理块中的序列复制一下
            for (int l = 1; l <= phy; l++)
                sim[l][i] = sim[l][i - 1];
        }
        if (block_find(i, Block[i]))
        { //如果物理块中已经有了本页面 无需置换
            continue;
        }
        else if (book < phy) //如果当前物理块中不满 也无需置换
        {
            sim[++book][i] = Block[i];
            miss++;
        }
        else
        {                              //需要置换
            int ans = 1, ans_time = 0; //记录需要置换的页面
            for (int l = 1; l <= phy; l++)
            { //寻找未来最久没被访问且在当前物理块的页面号
                int res_time = 0x3f3f3f;
                for (int j = i + 1; j <= 20; j++)
                {
                    if (sim[l][i] == Block[j])
                    {
                        res_time = j;
                        break;
                    }
                }
                if (res_time > ans_time)
                {
                    ans_time = res_time;
                    ans = l;
                }
            }
            sim[ans][i] = Block[i]; //置换
            miss++;
        }
    }
    printf("物理块序列：\n");
    for (int i = 1; i <= phy; i++)
    {
        printf("内存块号 %d:", i);
        for (int j = 1; j <= 20; j++)
            if (sim[i][j] == -1)
                printf("      ");
            else
                printf("%6d", sim[i][j]);
        printf("\n");
    }
    printf("缺页次数为：%d 缺页率为：%.2f \n", miss, 100.0 * miss / 20);
}

int main()
{
    printf("****内存页面置换算法的设计****\n");
    while (true)
    {
        printf("物理块数选择：3 or 4\n");
        scanf("%d", &phy);
        if (phy == 3 || phy == 4)
            break;
        printf("输入错误,请重新输入\n");
    }
    init(); //随机生成数据
    fifo();
    lru();
    opt();
    printf("\n****Finish!****\n");
    return 0;
}

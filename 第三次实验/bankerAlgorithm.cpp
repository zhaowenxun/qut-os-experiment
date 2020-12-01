#include<cstdio>
#include<cstring>
#include<queue>
using namespace std;
/*
*@author qut-zwx
*/
int n;
//系统中的进程的总数
int m;
//资源类总数
const int M=(int)1e4+5;
int Available[M];
int Max[M][M];
int Allocation[M][M];
int Need[M][M];
int Request[M];
int num ;
//要请求资源的进程名称

//安全检查
int Work[M];
//表示系统可以给进程进程继续运行所需要的各类资源数目
bool Finish[M];
bool sub_check(int *l,int* r) {
	for (int i=0;i<n;i++)
	        if(l[i]<r[i])
	            return false;
	return true;
}
queue<int> q;
int run_b;//记录上次尝试成功的进程
bool test(int i) {
		if(!Finish[i]&&sub_check(Work,Need[i])) {
			printf("进程Process--%d执行\n",i);
			q.push(i);
			for (int j=1;j<=m;j++) {
				Work[j]+=Allocation[i][j];
			}
			Finish[i]=true;
            printf("系统当前可用资源：\n");
            for (int i=1;i<=m;i++) {
                printf("%d ",Work[i]);
            }
            puts("");
            run_b=i;
			return true;
		}
	}


bool check() {
	memset(Finish,0,sizeof(Finish));
	//清空
	if(!sub_check(Available,Request))//检查一下请求的资源是否能被当前可用资源满足
	return false;
	//假定可以给num分配资源
	for (int i=1;i<=m;i++) {
		Available[i]-=Request[i];
		Allocation[num][i]+=Request[i];
		Need[num][i]-=Request[i];
		Work[i]=Available[i];
	}

    run_b=0;//从0开始尝试
	for (int i=0;i<n;i++){
    for (int i=run_b;i<n;i++)
        if(test(i))
            continue;
    for (int i=0;i<run_b;i++)
        if(test(i))
            continue;
    }
	//遍历尝试求解安全序列
	bool f=true;
	for (int i=0;i<n;i++)
        if(!Finish[i]) {
            f=false;
            break;
	}
	if(f) {
		printf("安全队列是:");
		while(q.size()) {
			printf("Process--%d ",q.front());
			q.pop();
		}
		printf("\n系统当前可用资源：");
		for (int i=1;i<=m;i++) {
			printf("%d ",Available[i]);
		}
		puts("");
		return true;
	} else {
		for (int i=1;i<=m;i++) {
			//还原原先分配的资源
			Available[i]+=Request[i];
			Allocation[num][i]-=Request[i];
			Need[num][i]+=Request[i];
		}
		return false;
	}
}
int main () {
	printf("***请输入系统中总进程数目和进程所需要的资源总数***\n");
	scanf("%d%d",&n,&m);
	printf("***请输入nxm大小的最大需求的资源***\n");
	for (int i=0;i<n;i++)
	        for (int j=1;j<=m;j++)
	        scanf("%d",&Max[i][j]);
	printf("***请输入nxm大小的已经分配的资源***\n");
	for (int i=0;i<n;i++)
	        for (int j=1;j<=m;j++)
	        scanf("%d",&Allocation[i][j]);
	printf("***请输入长度为m的现有可利用的资源***\n");
	for (int i=1;i<=m;i++)
	            scanf("%d",&Available[i]);
	for (int i=0;i<n;i++)
	        for (int j=1;j<=m;j++)
	            Need[i][j]=Max[i][j]-Allocation[i][j];
	printf("***对初始状态进行安全性检查***\n");
    num=0;
    memset(Request,0,sizeof(Request));
	    if(check())
	        printf("经过银行家算法的判断，初始状态是安全状态\n");
	    else{
	        printf("经过银行家算法的判断，初始状态不是安全状态\n");
	        return 0;
	    }
    puts("");
	while(true) {
		printf("***请输入需要请求资源的队列名并输入长度为m的请求队列***\n");
		scanf("%d",&num);
		for (int i=1;i<=m;i++)
		            scanf("%d",&Request[i]);
		if(check()) {
			printf("经过银行家算法的判断，可以满足Process--%d 进程请求资源\n",num);
		} else
		            printf("经过银行家算法的判断，不可以满足Process--%d 进程请求资源\n",num);
        puts("");
	}

	return 0;
}

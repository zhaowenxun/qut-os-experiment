#include <cstdio>
#include <ctime>
#include <algorithm>
#include <queue>

using namespace std;

/*
 * @author qut-zwx
 */
enum Status
{
	runing,
	ready,
	blocked
};
enum Policy
{
	spf,
	rr,
	stf,
	hrm
};
struct PCB
{
	int id; /* 进程标识数 */

	int cputime;	  /* 进程已经占用cpu的时间 */
	int alltime;	  /* 进程还需占用cpu 的时间 */
	int startblock;	  /* 进程的阻塞时间，表示startblock个时间片后进程将进入阻塞状态 */
	int blocktime;	  /* 进程被阻塞的时间，表示再等待blocktime后将转为就绪状态 */
	Status state;	  /* 进程当前的状态 */
	struct PCB *next; /* 队列指针 */

	int arrive_time;
	int run_time;
	int end_time;
};
struct pcmp
{
	bool operator()(const struct PCB l, const struct PCB r) /* 优先队列中根据到达时间排序 */
	{
		return (l.arrive_time > r.arrive_time);
	}
};
struct p_cmp
{
	bool operator()(struct PCB l, struct PCB r)
	{
		return (l.run_time > r.run_time);
	}
};
struct end_cmp
{
	bool operator()(const struct PCB l, const struct PCB r)
	{
		return (l.id > r.id);
	}
};
priority_queue<struct PCB, vector<PCB>, struct pcmp> p;		   /*按照到达时间排序，到达越早排的靠前 */
priority_queue<struct PCB, vector<PCB>, struct p_cmp> p_queue; /*按照运行时间排序，运行时间越短排的越靠前 */
priority_queue<struct PCB, vector<PCB>, struct end_cmp> p_end; /* 最终的优先队列，按照 */
queue<char> order;											   /* 运行id的顺序 */
queue<struct PCB> q_rr;
int n, num;
struct PCB f;
void spfn()
{
	int t;
	while (p.size())
	{
		t = 0;
		while (p.size() || p_queue.size())
		{
			if (p.size() && t < p.top().arrive_time)
			{
				for (t; t <= p.top().arrive_time; t++)
				{
					printf("当前时间是%d 当前无进程在运行\n", t);
				}
			}
			while (p.size() && p.top().arrive_time <= t) /* t为当前时刻，把已经到达的进程加入到运行队列 */
			{
				p_queue.push(p.top());
				p.pop();
			}
			f = p_queue.top();
			p_queue.pop();
			f.end_time = t + f.run_time;
			for (; t < f.end_time; t++)
			{
				printf("当前时间是%d\n 运行的进程编号为process-%c 此进程到达时间为%d 还需要运行时间为 %d 运行结束时间为%d \n", t, f.id + 'A', f.arrive_time, f.end_time - t - 1, f.end_time);
			}
			f.cputime = f.end_time - f.arrive_time;
			order.push((char)(f.id + 'A'));
			t = f.end_time;
			p_end.push(f);
		}
	}
	printf("当前时间为 %d 所有进程已经运行完毕！\n", t);
	printf("短作业优先执行顺序为： ");
	while (order.size())
	{
		printf("%c ", order.front());
		order.pop();
	}
	puts("");
	int c_time = 0;
	double r_time = 0;
	while (p_end.size())
	{
		f = p_end.top();
		int cycle_time = f.end_time - f.arrive_time;	  /* 周转时间 */
		double rt_time = (double)cycle_time / f.run_time; /* 带权周转时间 */
		c_time += cycle_time;
		r_time += rt_time;
		printf("进程%c 周转时间:%d 带权周转时间:%.2f \n", f.id + 'A', cycle_time, rt_time);
		p_end.pop();
		f.cputime = 0;
		p.push(f); /* 把调度算法的结果还原。 */
	}
	printf("平均周转时间:%d 平均带权周转时间:%.2f \n", c_time / num, r_time / num);
}

void rrn()
{
	int t = 0; /* 当前时间 */

	while (p.size() || q_rr.size())
	{
		if (!q_rr.size() && p.size() && t < p.top().arrive_time) /* 当前时间内 队列内没有进程到达 */
		{
			for (t; t < p.top().arrive_time; t++)
				printf("当前时间是%d 当前无进程在运行\n", t);
		}
		while (p.size() && p.top().arrive_time <= t) /* 解决进程队列没有进程的情况 */
		{
			q_rr.push(p.top());
			p.pop();
		}
		f = q_rr.front();
		order.push((char)(f.id + 'A'));
		q_rr.pop();
		f.cputime++;
		f.alltime = f.run_time - f.cputime;
		printf("当前时间是%d\n 运行的进程编号为process-%c 此进程到达时间为%d 已经占用CPU时间为%d 还需要运行时间为 %d  \n", t++, f.id + 'A', f.arrive_time, f.cputime, f.alltime);
		while (p.size() && p.top().arrive_time <= t)
		{
			q_rr.push(p.top());
			p.pop();
		}
		if (f.alltime == 0) /* 运行结束 */
		{
			f.end_time = t;
			p_end.push(f);
		}
		else
			q_rr.push(f);
	}
	printf("当前时间为 %d 所有进程已经运行完毕！\n", t);
	printf("轮转调度算法优先执行顺序为： ");
	while (order.size())
	{
		printf("%c ", order.front());
		order.pop();
	}
	puts("");
	int c_time = 0;
	double r_time = 0;
	while (p_end.size())
	{
		f = p_end.top();
		int cycle_time = f.end_time - f.arrive_time;	  /* 周转时间 */
		double rt_time = (double)cycle_time / f.run_time; /* 带权周转时间 */
		c_time += cycle_time;
		r_time += rt_time;
		printf("进程%c 周转时间:%d 带权周转时间:%.2f \n", f.id + 'A', cycle_time, rt_time);
		p_end.pop();
		p.push(f); /* 把调度算法的结果还原。 */
	}
	printf("平均周转时间:%d 平均带权周转时间:%.2f \n", c_time / num, r_time / num);
}

int main()
{
	printf("*******************************操作系统-进程调度模拟实验*******************************\n");
	printf("请输入进程的数量\n");
	scanf("%d", &num);

	for (int i = 0; i < num; i++)
	{
		printf("当前进程ID：process-%c\n", (char)(i + 'A'));
		f.id = i;
		printf("请分别输入进程到达时间： 进程运行时间：\n");
		scanf("%d%d", &f.arrive_time, &f.run_time);
		p.push(f);
	}
	while (true)
	{
		printf("请选择调度算法（0：spf,1:rr,2:stf,3:hrm）\n");
		scanf("%d", &n);
		switch (n)
		{
		case 0:
			spfn();
			break;
		case 1:
			rrn();
			break;
			/*		case 2: strn();break; */
			/*		case 3: hrmn();break; */
		default:
			printf("输入错误，程序结束\n");
			break;
		}
	}
	return (0);
}

#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<time.h>
#include<malloc.h>
#include<unistd.h>

#define buffer_full_size 10
/**
os experiment: producer-consumer
@auther: qut-zwx
*/
sem_t empty,mutex,full;
pthread_t producer1,producer2,consumer1,consumer2;
//定义缓冲区
struct BUFFER {
	char product[10];
	int sum;
	int head,rear;
};
//打印当前时间
void print_time(){
    time_t now_time=time(NULL);
	printf("%s",ctime(&now_time));
}

//缓冲区中数据表示为单个字母
char produce(){
	return (char)('A'+rand()%26);
}

//打印
void print(struct BUFFER *b){
    int i;//c99以下不允许在for中定义
    printf("缓冲区内数据为： ");
    if(b->head==b->rear)
    printf("空");
	else if(b->head<b->rear){
		for( i=b->head;i<b->rear;i++)
			printf("%c ",b->product[i]);
	}
	else {
		for( i=b->head;i<=buffer_full_size;i++)
			printf("%c ",b->product[i]);
		for( i=0;i<b->rear;i++)
			printf("%c ",b->product[i]);
	}
	printf("\n\n");

}

//生产者
void *producer(void *buffer ){
	struct BUFFER *b=(struct BUFFER*)buffer;
	int i;
	for( i=0;i<10;i++){//进程重复10次运行
		while(b->sum==buffer_full_size){
			sem_wait(&empty);//当满的时候，生产者在此阻塞
		}
		//不满的时候，需要往缓冲区内添加
		sem_wait(&mutex);
        if(pthread_self()==producer1)
            printf("当前运行的进程是：producer_one 次数为第%d次\n",i+1);
        else
            printf("当前运行的进程是：producer_two 次数为第%d次\n",i+1);
		char c=produce();
		b->sum=(b->sum+1)%buffer_full_size;
		b->product[b->rear++]=c;
		b->rear%=buffer_full_size;
		printf("生产的数据为%c \n",c);
		printf("当前缓冲区中数据数目为%d\n",b->sum);
		print(b);//打印当前缓冲区中的数据
		print_time();

		sem_post(&mutex);
		sem_post(&full);
		sleep(rand()%3);//随机时间等待
	}
}


//消费者
void *consumer(void *buffer ){
	struct BUFFER *b=(struct BUFFER *)buffer;
	int i;
	for( i=0;i<10;i++){//进程重复10次运行
		while(b->sum==0){
			sem_wait(&full);//当空的时候，生产者在此阻塞
		}
		//不满的时候，需要往缓冲区内添加
		sem_wait(&mutex);
        if(pthread_self()==consumer1)
            printf("当前运行的进程是：consumer_one 次数为第%d次\n",i+1);
        else
            printf("当前运行的进程是：consumer_two 次数为第%d次\n",i+1);
		char c=b->product[b->head];
		b->sum=(b->sum-1+buffer_full_size)%buffer_full_size;
		b->head=(b->head+1)%buffer_full_size;
		printf("消费的数据为%c \n",c);
		printf("当前缓冲区中数据数目为%d\n",b->sum);
		print(b);//打印当前缓冲区中的数据
		print_time();

		sem_post(&mutex);
		sem_post(&empty);
		sleep(rand()%3);//随机时间等待
	}
}


int main(){
    srand((unsigned)time(NULL));

	struct BUFFER *buffer;
	buffer = (struct BUFFER *)malloc(sizeof(struct BUFFER));
	//信号量初始化
	sem_init(&empty,0,10);
	sem_init(&mutex,0,1);
	//进程创建
	pthread_create(&producer1,NULL,producer,(void *)buffer);
	pthread_create(&producer2,NULL,producer,(void *)buffer);
	pthread_create(&consumer1,NULL,consumer,(void *)buffer);
	pthread_create(&consumer2,NULL,consumer,(void *)buffer);
	//进程销毁
	pthread_join(producer1,NULL);
	pthread_join(producer2,NULL);
	pthread_join(consumer1,NULL);
	pthread_join(consumer2,NULL);

	//信号量销毁
	sem_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);
	return 0;
}

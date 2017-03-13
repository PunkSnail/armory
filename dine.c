#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

// 5 把锁
pthread_mutex_t lock[5];

void* dine(void* arg)
{
	//第几个人
	int number = (int)arg;
	int left, right;
	if(number < 4)
	{
		right =number;
		left = number + 1;
	}
	else
	{
		right = 0;
		left = number;
	}
	int i = 0;
	//开始抢，总共抢了三轮，如果各线程设置了随机休眠，看起来会多些
	while(i < 5)
	{	//先拿右边筷子，若该线程没有没有加锁成功会阻塞在这一步
		//直到有线程释放该资源再继续抢，反之加锁成果会阻塞其他线程
		pthread_mutex_lock(&lock[right]);

		//抢左手边，看起来是一个条件，其实在if之前就已经获得了右手边的筷子
		//调用此函数加锁失败也不会阻塞
		if(pthread_mutex_trylock(&lock[left]) == 0)
		{	
			//开吃
			printf("Number %d 开始吃...\n", number);
			//解锁左手边筷子
			pthread_mutex_unlock(&lock[left]);
		}
		
		//无论有没有抢到另一只筷子此线程都放下已有的筷子
		//解锁的同时会唤醒其他阻塞在锁上的全部线程
		pthread_mutex_unlock(&lock[right]);
		
		sleep(rand() % 3);
		//sleep(1);
		i++;
	}
	return NULL;
}


int main(void)
{
	srand((unsigned int)time(NULL));

	// 5 个子线程同桌吃饭，争抢 5 支筷子
	pthread_t thid[5];

	for(int i = 0; i < 5; ++i)
	{
		pthread_mutex_init(&lock[i], NULL);
	}
	//设置线程属性，特殊需求用	
	//pthread_attr_t attr;
	//pthread_attr_init(&attr);
	//设置线程分离，分离后不用主线程释放，多用于服务器
	//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	for(int i = 0; i < 5; i++)
	{
		pthread_create(&thid[i], NULL, dine, (void*)i);
	}
	//主线程不挂，子线程们才能继续抢
	//sleep(12);
	//while(1);//写了死循环后面的语句就没了意义

	for(int i = 0; i < 5; i++)
	{	//阻塞等待回收子线程
		pthread_join(thid[i], NULL);
	}

	for(int i = 0; i < 5; i++)
	{
		pthread_mutex_destroy(&lock[i]);
	}
	//销毁属性
	//pthread_attr_destroy(&attr);
	//让线程主动退出
	//pthread_exit(NULL);

	return 0;
}

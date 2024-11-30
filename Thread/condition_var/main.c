#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>

#define MAX_NUM 5

pthread_t thread_1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int global_var = 0;

static void *thread_handle_1(void *args)
{
	pthread_mutex_lock(&lock);

	while(global_var < MAX_NUM){
		global_var++;
		printf("thread_1 var++ = %d\n", global_var);
		sleep(1);
	}
	pthread_cond_signal(&cond);//notify for thread 2
	printf("Thread_1 success!!!\n");
	pthread_mutex_unlock(&lock);

	// pthread_exit(NULL);
}

int  main(int argc, char *argv[])
{
	int ret;

	ret = pthread_create(&thread_1, NULL, &thread_handle_1, NULL);
	if(ret != 0)
	{
		printf("Thread 1 create failed.\n");
		return -1;
	}

	pthread_mutex_lock(&lock);
	while (1)
	{
		pthread_cond_wait(&cond, &lock); /*Wait notifly signal from thread 1*/
		if(global_var == MAX_NUM)
		{
			printf("thread_2 var = %d\n", global_var);
			break;
		}
	}
	pthread_mutex_unlock(&lock);
	
	pthread_join(thread_1, NULL); //block thread_1 until it terminates

	return 0;
}

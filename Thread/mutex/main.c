#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread_1, thread_2;

int global_var = 1;

static void *thread_handle_1(void *args)
{
	// while(1){
	// 	global_var++;
	// 	printf("thread_1 var++ = %d\n", global_var);
	// 	sleep(1);
	// }
	pthread_mutex_lock(&lock1);
	global_var++;
	printf("thread_1 var++ = %d\n", global_var);
	pthread_mutex_unlock(&lock1);

}

static void *thread_handle_2(void *args)
{
	// while(1){
	// 	global_var+=2;
	// 	printf("thread_2 var++ = %d\n", global_var);
	// 	sleep(2);
	// }
	pthread_mutex_lock(&lock2);
	global_var++;
	printf("thread_2 var++ = %d\n", global_var);
	sleep(3);
	pthread_mutex_unlock(&lock2);
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
	sleep(2);

	ret = pthread_create(&thread_2, NULL, &thread_handle_2, NULL);
	if(ret != 0)
	{
		printf("Thread 2 create failed.\n");
		return -1;
	}
	pthread_join(thread_2, NULL); //block thread_2 until it terminates
	pthread_join(thread_1, NULL); //block thread_2 until it terminates
	// while(1);
	return 0;
}

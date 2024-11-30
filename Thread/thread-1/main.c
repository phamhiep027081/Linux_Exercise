#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>

pthread_t thread_1, thread_2;

typedef struct Student_S
{
	char 	_name[30];
	char 	_phone[10];
	int 	_age;
}Student;

static void *thread_handle(void *args)
{
	pthread_t threadId = pthread_self();
	Student *st = (Student *)(args);
	if(threadId == thread_1)
	{	
		printf("Thread 1 active\n");
	}
	else
	{
		printf("Your Name		: %s\n", st->_name);
		printf("Your Phone		: %s\n", st->_phone);
		printf("Your Age		: %d\n", st->_age);
	}
	// while (1)
	// {
	// 	printf("Hello thread 1\n");
	// 	sleep(1);
	// }
	// exit(0); //all thread terminate immediadately
}

static void *thread_handle2(void *args)
{
	// printf("call pthread exit()\n");
	// pthread_exit(NULL);

	while (1)
	{
		printf("Hello thread 2\n");
		sleep(1);
	}
}
int  main(int argc, char *argv[])
{
	Student s = {0};
	int ret;

	memcpy(s._name, "Pham Chinh Hiep", sizeof(s._name));
	memcpy(s._phone, "0123456789", sizeof(s._phone));
	s._age = 23;

	ret = pthread_create(&thread_1, NULL, &thread_handle, NULL);
	if(ret != 0)
	{
		printf("Thread 1 create failed.\n");
		return -1;
	}

	// ret = pthread_create(&thread_2, NULL, &thread_handle, &s);
	ret = pthread_create(&thread_2, NULL, &thread_handle2, NULL);
	if(ret != 0)
	{
		printf("Thread 2 create failed.\n");
		return -1;
	}
	while(1);
	// pthread_join(thread_2, NULL); //block thread_2 until it terminates
	// sleep(5);
	// pthread_cancel(thread_2); // cancel thread_2 after 5s
	return 0;
}

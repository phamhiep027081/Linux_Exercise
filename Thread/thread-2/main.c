#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <sys/file.h>
#include <stdbool.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condEnter = PTHREAD_COND_INITIALIZER;
pthread_cond_t condWrite = PTHREAD_COND_INITIALIZER;
pthread_cond_t condRead = PTHREAD_COND_INITIALIZER;

static bool ready_wr_flags = false;
static bool ready_rd_flags = false;
typedef struct Student_S
{
	char 	_name[30];
	char 	_phone[10];
	int 	_age;
}Student;
const char *fileName = "student_info.txt";
Student student;


static void *thread_handle_1(void *args)
{	
	pthread_mutex_lock(&mutex);
	printf("-Thread_1 active-\n");
	Student *st = (Student *)args;	
	while(1)
	{
		while (ready_wr_flags || ready_rd_flags) {
            pthread_cond_wait(&condEnter, &mutex);
        }
		printf("\tEnter student info\t\n");
		printf("Enter Name: ");
		fgets(st->_name, sizeof(st->_name), stdin);
		student._name[strcspn(student._name, "\n")] = '\0'; // Delete '\n'
		printf("Enter Phone Number: ");
		fgets(st->_phone, sizeof(st->_phone), stdin);
		student._phone[strcspn(student._phone, "\n")] = '\0';
		printf("Enter Age: ");
		scanf("%d", &st->_age);
		getchar();

		ready_wr_flags = true;

		pthread_cond_signal(&condWrite);//notify for thhread 2

	}
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

static void *thread_handle_2(void *args)
{
	pthread_mutex_lock(&mutex);
	printf("-Thread_2 active-\n");
	Student *st = (Student *)args;
	while (1)
	{
		while (!ready_wr_flags) {
            pthread_cond_wait(&condWrite, &mutex);
        }

		int fd = open(fileName, O_CREAT | O_RDWR | O_APPEND, 0644);
		char buffer[50];
		if(fd)
		{
			memset(buffer, '\0', sizeof(buffer));
			printf("\tWrite to file\t\n");
			snprintf(buffer, sizeof(buffer), "%s %s %d\n", st->_name, st->_phone, st->_age);
			write(fd, buffer, strlen(buffer));
			close(fd);
		}
		ready_wr_flags = false;
		ready_rd_flags = true;

		pthread_cond_signal(&condRead);//notify for thread 3
	}
	pthread_mutex_unlock(&mutex);
	
	pthread_exit(NULL);
}

static void *thread_handle_3(void *args)
{
	pthread_mutex_lock(&mutex);
	printf("-Thread_3 active-\n");
	while (1)
	{
		while (!ready_rd_flags) {
            pthread_cond_wait(&condRead, &mutex);
        }

		int fd = open(fileName, O_RDONLY);
		char buffer[50];

		if(fd)
		{
		char buffer[1];          // Đọc từng byte
        char line[50] = "";     // Dùng để chứa một dòng
        int linePos = 0;         // Vị trí trong dòng
        ssize_t bytesRead;       // Số byte đọc được

		printf("\tStudent info:\n");
        while ((bytesRead = read(fd, buffer, 1)) > 0) {
            // Thêm byte đọc được vào dòng
            if (buffer[0] == '\n' || linePos >= sizeof(line) - 1) {
                // Khi gặp '\n' hoặc dòng quá dài, in ra
                line[linePos] = '\0'; // Kết thúc chuỗi
                printf("%s\n", line); // In dòng ra terminal
                linePos = 0;          // Reset dòng
            } else {
                line[linePos++] = buffer[0];
            }
        }

        // Xử lý trường hợp dòng cuối không kết thúc bằng '\n'
        if (linePos > 0) {
            line[linePos] = '\0';
            printf("%s\n", line);
        }
			close(fd);
		}
		ready_rd_flags = false;

		pthread_cond_signal(&condEnter);//notify for thread 1
		
	}
	
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
int  main(int argc, char *argv[])
{
	pthread_t thread_1, thread_2, thread_3;
	int ret;

	ret = pthread_create(&thread_1, NULL, &thread_handle_1, &student);
	if(ret != 0)
	{
		printf("Thread 1 create failed.\n");
		return -1;
	}

	ret = pthread_create(&thread_2, NULL, &thread_handle_2, &student);
	if(ret != 0)
	{
		printf("Thread 2 create failed.\n");
		return -1;
	}

	ret = pthread_create(&thread_3, NULL, &thread_handle_3, NULL);
	if(ret != 0)
	{
		printf("Thread 2 create failed.\n");
		return -1;
	}

	pthread_join(thread_1, NULL); //block thread_1 until it terminates
	pthread_join(thread_2, NULL); //block thread_2 until it terminates
	pthread_join(thread_3, NULL); //block thread_3 until it terminates

	return 0;
}

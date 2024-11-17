#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <fcntl.h>
#include <string.h>

void write_to_file(char *fileName, const char data[])
{
	int fd = open(fileName, O_CREAT | O_RDWR | O_APPEND, 0644);
	if(fd != -1)
	{
		printf("write to file-----\n");
		write(fd, data, strlen(data));
	}
	else
	{
		printf("open file failed!!!\n");
	}
}

int  main(int argc, char *argv[])
{
	
	int status;
	char *fileName = "example.txt";
	pid_t pId_B, pId_C;
	
	pId_B = fork();

	/*Process B*/
	if(pId_B == 0)
	{
		if(pId_B == 0)
		{	
			/*Child process*/
			pid_t pId_B_ = fork();
			
			if(pId_B_ >= 0)
			{
				if(pId_B_ == 0)
				{
					printf("I am process B': %d\n", getpid());
					printf("\t-----Process B' ");
					write_to_file(fileName, "Hello. I am B' process.\n");
					exit(0);
				}
				else{
					printf("I am process B, my pID: %d - Parent-A pID: %d\n", getpid(), getppid());
					waitpid(pId_B_, &status, 0);
					exit(0);
				}
			}
			else
			{
				printf("B fork() error!!!\n");
			}
		}
	}

	pId_C = fork();
	/*Process C*/
	if(pId_C == 0)
	{
		if(pId_C == 0)
		{	
			/*Child process*/
			pid_t pId_C_ = fork();
			
			if(pId_C_ >= 0)
			{
				if(pId_C_ == 0)
				{
					printf("I am process C': %d\n", getpid());
					printf("\t-----Process C' ");
					write_to_file(fileName, "Hello. I am C' process.\n");
					exit(0);
				}
				else{
					printf("I am process C, my pID: %d - Parent-A pID: %d\n", getpid(), getppid());
					waitpid(pId_C_, &status, 0);
					exit(0);
				}
			}
			else
			{
				printf("C fork() error!!!\n");
			}
		}
	}

	if(pId_B > 0 && pId_C > 0)
	{
		/*Parent A process*/
		printf("I am process A, my pID: %d\n", getpid());
		waitpid(pId_B, &status, 0);
		printf("\t-----Process B ");
		write_to_file(fileName, "Hello. I am B process.\n");

		printf("\t-----Process C ");
		waitpid(pId_C, &status, 0);
		write_to_file(fileName, "Hello. I am C process.\n");

		printf("Child process exited with status: %d\n", WEXITSTATUS(status));
	}else
	{

		printf("A fork() error!!!\n");

	}
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>

int  main(int argc, char *argv[])
{
	
	int fd;
	char w_buffer[] = "Hello. I am Hiep\n";
	char r_buffer[20];
	pid_t child_pid = fork();
	if(child_pid >= 0)
	{
		if(child_pid == 0)
		{	
			fd = open("hello.txt", O_RDONLY);
			/*Child process - Read file*/
			if(flock(fd, LOCK_SH) == 0)
			{
				if(read(fd, r_buffer, sizeof(w_buffer)) >= 0)
				{
					printf("Read string: %s", r_buffer);
				}
				flock(fd, LOCK_UN);
			}
			else
			{
				printf("Child lock failed\n");
			}
			if(fd != -1) close(fd);
		}
		else
		{
			/*Parent process - Write file*/
			fd = open("hello.txt", O_CREAT | O_WRONLY, 0644);
			if(flock(fd, LOCK_EX) == 0)
			{
				if(write(fd, w_buffer, sizeof(w_buffer)) >= 0)
				{
					printf("Write successfully\n");

				}
				else
				{
					printf("Write error!!!\n");
				}
				flock(fd, LOCK_UN);
			}
			else
			{
				printf("Parent lock failed\n");
			}
			if(fd != -1) close(fd);
		}
	}
	else
	{
		printf("fork() error!!!\n");
	}
	return 0;
}

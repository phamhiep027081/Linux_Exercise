#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int  main(int argc, char *argv[])
{
	
	int status;
	pid_t child_pid = fork();
	if(child_pid >= 0)
	{
		if(child_pid == 0)
		{	
			/*Child process*/
			printf("I am child, my pID: %d - Parent pID: %d\n", getpid(), getppid());
			sleep(3);
			// while(1);
		}
		else
		{
			/*Parent process*/
			printf("I am parent, my pID: %d - Child pID: %d\n", getpid(), child_pid);
			if(waitpid(child_pid, &status, 0) >= 0)
			{
				printf("Child process exited with status: %d\n", WEXITSTATUS(status));
			}
			else
			{
				printf("waitpid() failed!!!\n");
			}
		}
	}
	else
	{
		printf("fork() error!!!\n");
	}
	return 0;
}

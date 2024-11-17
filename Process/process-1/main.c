#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int  main(int argc, char *argv[])
{
	pid_t child_pid = fork();
	if(child_pid >= 0)
	{
		if(child_pid == 0)
		{
			printf("pId of child: %d\n", getpid());
			while(1);
		}
		else{
			printf("pId of parent: %d\n", getpid());
			while(1);
		}
	}
	else
	{
		printf("fork() error!!!\n");
	}

	return 0;
}

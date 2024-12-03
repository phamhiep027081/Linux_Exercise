#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int global = 0;
void sig_handler_1(int num)
{
	printf("\nCounter = %d\t Signal_Number = %d\n", global, num);
	exit(EXIT_SUCCESS);
}
int  main(int argc, char *argv[])
{
	if(signal(SIGINT, sig_handler_1) == SIG_ERR)/*Enter Ctrl+C to print counter and exit*/
	{
		printf("\nCann't register signal.");
		exit(EXIT_FAILURE);
	}
	/*SIGKILL & SIGSTOP cann't catch*/
	for(int i = 0; i < 10; i++)
	{
		printf("Increate counter to 1\n");
		global++;
		sleep(1);
	}
	return 0;
}

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
	sigset_t new_mask, current_mask;

	sigemptyset(&new_mask);
	sigemptyset(&current_mask);

	if(signal(SIGINT, sig_handler_1) == SIG_ERR)/*Enter Ctrl+C to print counter and exit*/
	{
		printf("\nCann't register signal.");
		exit(EXIT_FAILURE);
	}
	sigaddset(&new_mask, SIGINT);
	/*SIGKILL & SIGSTOP cann't catch*/
	if(sigprocmask(SIG_SETMASK, &new_mask, &current_mask) == 0)
	{
		if(sigismember(&new_mask, SIGINT))
		{
			printf("\nSignal exsist.");
		}
		else
		{
			printf("\nSignal doesn't exsist.");
		}
	}
	for(int i = 0; i < 10; i++)
	{
		printf("\nIncreate counter to 1");
		global++;
		sleep(1);
		if(global == 10)
		{
			printf("\nCounter = %d", global);
		}
	}
	return 0;
}

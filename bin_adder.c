#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main (int argc, char* argv[])
{
	while(1)
	{
		printf("In Child\n");
	}
}

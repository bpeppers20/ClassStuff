#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

int main (int argc, char* argv[])
{
	
	//Make a child process within the child process in order for adding
	//Semaphores and another fork Proccess here
	//This will also be the only file editing share memory
	int * nums = 0;
	key_t key = ftok("proj3", 10);
	int shmid = shmget(key, 1024, 0666|IPC_CREAT);
	if (shmid < 0)
	{
		perror("Failed to connect to shared memory");
		exit(EXIT_FAILURE);
	}
	nums = (int*) shmat(shmid, (void*)0, 0); // attach shm
	nums[0] = 1;
	while(1)
	{
		printf("In Child\n");
		nums [0] = nums[0] + 1;
		printf("%d\n",nums[0]);
	}
}

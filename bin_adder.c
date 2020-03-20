#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/sem.h>
pid_t id = -1;
char *file; //File
sem_t *mutex; // start semahore initlization

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
	nums[0] = 0;
	// Semaphore initalzation 
	mutex = sem_open( "temp5", O_CREAT | O_EXCL, 0644, 1);
	// Store numbers based on total for summation
	// Separate based on n/2
	




	// separate based on n/logn
	int sum = 0;
	int i = 0; //Start wehre numbers are being added
	// Semaphore Limit
	//Test Alg
	for(i; i < 6; i++)
	{
		nums [i] = i;
	}

	id = fork(); // Create process, Will be changed to make based on need
	if (id < 0)
	{
		perror("Failed to create process");
	}

	// Section for work
	for (i = 0; i < 6; i++)
	{
		sem_wait(mutex); // wait for turn
		// Crit Sec
		printf("Entering Critical Section...\n");
		sum += nums[i]; 
		printf("Summation of i = 0 and n = %d is %d\n",i,sum);
		printf("Exiting Critical Section...\n");
		sem_post(mutex);
	}
	//Test Alg
	// Semaphore Limit
	sem_close(mutex);//Get rid of semaphore when done
	sem_unlink("temp5");
	printf("%d\n", sum);
	int status;
	id = wait(&status);
}

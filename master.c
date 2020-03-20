#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <time.h>

pid_t id = -1; // global process id
char *file = "bin_adder.txt";// File
// Shared Memory possible global
int * shmPtr = 0; // Initilze for security reasons
int shm_id = 0; 

//Signal Handling
void kill_child(int sig) // Handles SIGKILL with SIGALRM
{
	printf("Value in Shared Memory: %d\n",shmPtr[0]);
	shmdt(shmPtr); // detach shared memory
	shmctl(id, IPC_RMID,NULL); // Remove shared memory
	printf("Process Automatically killed\n"); // log error message
	// File operations
	kill(id,SIGKILL);

}

void sigIntHandle (int sig) // handles SIGINT
{
	printf("Value in Shared Memory: %d\n",shmPtr[0]);
	shmdt(shmPtr); // detach shared memory
	shmctl(id,IPC_RMID,NULL); // remove shared memory
	//FILE operation
	printf("\nProcess manually killed\n");
	kill(id,SIGKILL); // kill process
}

int main (int argc, char* argv[])
{
	// This file will create shared memory and initlize it, but 
	// will not edit. Will only read
	id = getpid();// get parent process
	if (id < 0)
	{
		perror(" Failed to receive parent id\n");
		return -1;
	}
	int e; // For execing
	signal(SIGALRM, (void(*)(int))kill_child); // capture SIGALARM
	signal(SIGINT,(void(*)(int))sigIntHandle); // capture SIGINT
	// Shared Memory Operations
	key_t key = ftok("proj3",10); // create accessable key
	int shm_id = shmget(key,1024, 0666|IPC_CREAT); // get shared memory id
	if (shm_id < 0)
	{
		perror("Failed to connect to shared memory"); // check if connected
		return -1;
	}
	shmPtr = (int*) shmat(shm_id, (void*)0, 0); // attach shared memory
	// Shared Memory Operations 
	
	//set Alarm
	id = fork(); // Create Child Process
	if (id > 0)
	{
		alarm(100); // set alarm in parent
	}
	if (id < 0) // terminate on failure
	{
		perror("Failed to create process\n");
		return -1;
	}
	if (id == 0)
	{
		e = execvp("./bin_adder",NULL);
	}
	int status;
	wait(&status);
	shmdt(shmPtr); // detach shm
	shmctl(shm_id,IPC_RMID,NULL);
	
}


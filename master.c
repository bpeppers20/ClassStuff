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

//Signal Handling
void kill_child(int sig) // Handles SIGKILL with SIGALRM
{
//	shmdt(shmPtr); // detach shared memory
//	shmctl(id, IPC_RMID,NULL); // Remove shared memory
	printf("Process %d Automatically killed\n", id); // log error message
	// File operations
	kill(id,SIGKILL);

}

void sigIntHandle (int sig) // handles SIGINT
{
//	shmdt(shmPtr); // detach shared memory
//	shmctl(id,IPC_RMID,NULL); // remove shared memory
	//FILE operation
	printf("\nProcess %d manually killed\n",id);
	kill(id,SIGKILL); // kill process

}

int main (int argc, char* argv[])
{
	id = getpid();// get parent process
	if (id < 0)
	{
		perror(" Failed to receive parent id\n");
	}
	int e; // For execing
	signal(SIGALRM, (void(*)(int))kill_child); // capture SIGALARM
	signal(SIGINT,(void(*)(int))sigIntHandle); // capture SIGINT
	// Shared Memory Operations
	//
	//
	//
	//
	// Shared Memory Operations 
	
	//set Alarm
	id = fork(); // Create Child Process
	if (id > 0)
	{
		alarm(2); // set alarm in parent
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







}


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
#include <math.h>

pid_t id = -1;
FILE *fptr; // file pointer
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
	double size = 0; // for ceil function
	printf("Enter the number you want the summation of.\n");
	scanf("%lf",&size);
	// Separate based on n/2
	int split_size = 0;
	split_size = (int) (ceil(size / 2));
	int i_size = (int) (size); // Size for loops
	int size2 = i_size - split_size; // Size of second half of array

	int arr1[split_size]; // First halve
	int arr2[size2]; // Second half of array


	// separate based on n/logn
	int sum = 0;
	int i = 0; //Start wehre numbers are being added
	// Semaphore Limit
	//Test Alg
	for(i; i <= i_size; i++)
	{
		nums [i] = i;
	}
	for (i = 1; i <= split_size; i++) // fill in first array
	{
		arr1[i-1] = nums[i];
		printf("arr1 %d is %d\n",i - 1, arr1[i - 1]);
	}

	int j = 0;// index for second array
	for (i = split_size + 1; i <= i_size; i++)
	{
		arr2[j] = nums[i]; // fill second array starting from where arr1 left off
		printf("arr2 %d is %d\n", j, arr2[j]);
		j = j + 1; // increment j
	}
	int pNum = split_size; // n/2 processes
	if ( split_size >  15)
	{
		pNum = 15; // set proccess number to 15 if over 15
	}
	for (i = 0; i < pNum; i++)
		id = fork(); // Create process based on n / 2
	if (id < 0)
	{
		perror("Failed to create process");
	}

	// Section for work for n/2
	j = 0; // reset j to 0
	for (i = 0; i <= i_size; i++)
	{
		sleep(3);
		sem_wait(mutex); // wait for turn
		// Crit Sec
		printf("Entering Critical Section...\n");
		if ( i <= split_size) // do work with array 1
			sum += arr1[i]; 
		if ( i > split_size) // do work with array 2
		{
			sum += arr2[j];
			j = j + 1; // itterate j
		}
		printf("Exiting Critical Section...\n");
		sem_post(mutex);
	}
	for (i = 0; i < pNum; i++)
	wait(NULL);
	// Work Done with n/2 split array

	
	sem_close(mutex);//Get rid of semaphore when done
	sem_unlink("temp5");
	if ( id > 0) // only print if parent
	{
		printf("Process: %d , prints sum: %d\n",id, sum);
		fptr = fopen("adder_log", "w");
		if (fptr == NULL) 
		{
			perror("Failed to open file");
			exit(EXIT_FAILURE);
		}
		fprintf(fptr,"Process: %d, prints summation of %lf is %d with size %d\n", getpid(), size, sum, sizeof(getpid()));
		fclose(fptr);
	}
}

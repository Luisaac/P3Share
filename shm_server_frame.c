#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>
// ADD NECESSARY HEADERS
#define SHM_NAME "zhenda_renjie"
#define PAGESIZE 4096 

// Iteration Number
int iteration = 0;

// Mutex variables
pthread_mutex_t* mutex;
pthread_mutexattr_t mutexAttribute;

// stats struct
typedef struct {
    int pid;
    char birth[25];
    char clientString[10];
    int elapsed_sec;
    double elapsed_msec;
    int inuse;
} stats_t;

void exit_handler(int sig) 
{
    // ADD
    
	exit(0);
}

int main(int argc, char *argv[]) 
{
    // ADD
	
	// Creating a new shared memory segment
	int fd_shm = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0660);	
	ftruncate(fd_shm, PAGESIZE);       
	void *shm_ptr = (void*) mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
	//mutex = (pthread_mutex_t*) mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);

	mutex = (pthread_mutex_t*) shm_ptr;
	for (int i = 1; i < 64; i++) {
		stats_t* curr = (stats_t*)(shm_ptr+(i*64));
		curr->inuse = 0;
	}
	 //maybe use memcpy to cpy struct to shm_ptr

	
    // Initializing mutex
	pthread_mutexattr_init(&mutexAttribute);
	pthread_mutexattr_setpshared(&mutexAttribute, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(mutex, &mutexAttribute);

    while (1) 
	{
		// ADD
		for (int i = 1; i < 64; i++) {
			stats_t* curr = (stats_t*)(shm_ptr+(i*64));
			if (curr->inuse == 0) {
				break;
			}
			printf("%d, pid : %d, birth : %s, elapsed : %d s %f ms, %s\n",
						iteration, curr->pid, curr->birth, curr->elapsed_sec, curr->elapsed_msec, curr->clientString);
		}

		iteration++;
        sleep(1);
    }

    return 0;
}

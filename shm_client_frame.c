#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <signal.h>
// ADD NECESSARY HEADERS
#define SHM_NAME "zhenda_renjie"
#define PAGESIZE 4096


// stats struct
typedef struct {
    int pid;
    char birth[25];
    char clientString[10];
    int elapsed_sec;
    double elapsed_msec;
    int inuse;
} stats_t;


// Mutex variables
pthread_mutex_t* mutex;

void exit_handler(int sig) {
    // ADD
    // set inuse to zero

    // critical section begins
	pthread_mutex_lock(mutex);

    // Client leaving; needs to reset its segment   

	pthread_mutex_unlock(mutex);
	// critical section ends

    exit(0);
}

int main(int argc, char *argv[]) {
	// Signals
	// struct sigaction act;
	// act.sa_handler = exit_handler;


	// ADD    
	int shm_fd = shm_open(SHM_NAME, O_RDWR, 0660);
	void *shm_ptr = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);		
	mutex = (pthread_mutex_t*) shm_ptr;
    // critical section begins
    pthread_mutex_lock(mutex);
        
	// client updates available segment
    stats_t* curr;
	for (int i = 1; i < 64; i++) {
		curr = (stats_t*)(shm_ptr+(i*64));
		if (curr->inuse == 0) {
			curr->inuse = 1;
			curr->pid = getpid();
			time_t t = time(NULL);
			strcpy(curr->birth, ctime(&t));
			//TODO elapsed time
			break;
		}
	}
	pthread_mutex_unlock(mutex);
    // critical section ends



        
	while (1) {
        
		// ADD
        //TODO update elapsed time

  


		// Print active clients
		printf("Active clients :");
       	for (int i = 1; i < 64; i++) {
			stats_t* itr = (stats_t*)(shm_ptr+(i*64));
			if (itr->inuse == 1) {
				printf(" %d", itr->pid);
			}
		}
		printf("\n");
        sleep(1);
    }

    return 0;
}

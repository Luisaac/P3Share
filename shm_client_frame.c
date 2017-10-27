#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
// ADD NECESSARY HEADERS
#include <sys/types.h>
#define SMH_NAME "zhenda_renjie"
#define PAGESIZE 4096

// Mutex variables
pthread_mutex_t* mutex;

void exit_handler(int sig) {
    // ADD

    // critical section begins
	pthread_mutex_lock(mutex);

    // Client leaving; needs to reset its segment   

	pthread_mutex_unlock(mutex);
	// critical section ends

    exit(0);
}

int main(int argc, char *argv[]) {
	// ADD    
	
    // critical section begins
    pthread_mutex_lock(mutex);
        
	// client updates available segment
	int shm_fd = shm_open(SHM_NAME, O_RDWR, 0660);
	void *shm_ptr = mmap(NULL, PAGESIZE, PROT_READ |PROT_WRITE, MAP_SHARED, shm_fd, 0);		
	pthread_mutex_unlock(mutex);
    // critical section ends
        
	while (1) {
        
		// ADD
        
        sleep(1);

		// Print active clients
    }

    return 0;
}

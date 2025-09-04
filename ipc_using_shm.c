//task: calculate execution time of the application that we will run in child 
//      address space.

//findings:
//      in genral:
//          for related process use => unnamed semaphore,
//          other wise named semaphore.

#include <stdio.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <semaphore.h>

//define named constant here
#define SHM_SIZE 1024

//globla variables
static int i =0;


//section for structure declaration
typedef struct{
    char* buffer[SHM_SIZE];
    sem_t lock;
}shared_data;


//used for debuggin purpose
#define PRINT printf("check point: %d\n",i++)



int main(int argc,char* argv[]){
    const char* shared ="/my_shared_memory";

    //create memory object
    int shm_fd =shm_open(shared,O_CREAT|O_RDWR,0666);
    if(shm_fd < 0){
        perror("shm_open\n");
        return 1;
    }

    //check point 1
    PRINT;


    //file descriptor created.
    if(ftruncate(shm_fd,64) < 0){
        perror("ftruncate\n");

        //return resources
        close(shm_fd);

        return 1;
    }

    //check point 2
    PRINT;

    //object created and its size is defined 
    //time to map to process's virtual address space
    shared_data *shm =mmap(NULL,sizeof(shared_data),PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
    if(shm == MAP_FAILED){
        perror("mmap\n");

        //release resource 
        close(shm_fd);

        return 1;
    }

    //check point 3
    PRINT;


    sem_init(&shm->lock,1,1);

    //we don't need semaphore wait() will do that for us

    PRINT;



}

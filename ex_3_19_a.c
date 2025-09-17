/*DESCRIPTION:
    task: calculate execution time of the application that we will run in child 
        address space.

    ipc method used: shared memory

    findings:
        in genral:
            for related process use => unnamed semaphore,
            other wise named semaphore.
*/
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
    struct timeval start;
    sem_t lock;
}shared_data;


//used for debuggin purpose
#define PRINT printf("check point: %d\n",i++)



int main(int argc,char* argv[]){
    //command line inputs validation
    if(argc < 3){
        printf("Error: command_line_argument_count_mismatch\n");
        return 1;
    }

    const char* shared ="/my_shared_memory";

    //create memory object
    int shm_fd =shm_open(shared,O_CREAT|O_RDWR,0666);
    if(shm_fd < 0){
        perror("shm_open\n");
        return 1;
    }



    //file descriptor created.
    if(ftruncate(shm_fd,sizeof(shared_data)) < 0){
        perror("ftruncate\n");

        //return resources
        close(shm_fd);

        return 1;
    }



    //object created and its size is defined 
    //time to map to process's virtual address space
    shared_data *shm =mmap(NULL,sizeof(shared_data),PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);
    if(shm == MAP_FAILED){
        perror("mmap\n");

        //release resource 
        close(shm_fd);

        return 1;
    }




    sem_init(&shm->lock,1,1);

    //create child
    pid_t pid_child =fork();
    if(pid_child < 0){
        perror("fork failed\n");

        //release resource
        close(shm_fd);
        munmap(shm,sizeof(shared_data));
        sem_destroy(&shm->lock);
        return 1;
    }else if(pid_child == 0){
        //aquire lock
        sem_wait(&shm->lock);

        //child process
        printf("child[%d]:\n",getpid());

        //take snapshot of current time
        gettimeofday(&shm->start,NULL);

        sem_post(&shm->lock);
        //run the application
        execlp(argv[1],argv[2],NULL);

        perror("failed to run application\n");

    }else{
        //parent process
        int status;
        if(waitpid(pid_child,&status,0) < 0){
            perror("waitpid failed\n");
            return 1;
        }

        sem_wait(&shm->lock);

        printf("parent[%d]:\n",getpid());

        // take snapshot of current time 
        struct timeval end;
        gettimeofday(&end,NULL);

        float time_spend =(end.tv_sec + end.tv_usec/1000000.0) - (shm->start.tv_sec + shm->start.tv_usec/1000000.0);

        printf("execution time of program[%s]: %f\n",argv[1],time_spend);

        sem_post(&shm->lock);
        

    }


    //release resource
    sem_destroy(&shm->lock);
    close(shm_fd);
    munmap(shm,sizeof(shared_data));
    

    //we don't need semaphore wait() will do that for us

}

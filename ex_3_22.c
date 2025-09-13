/*DESCRIPTION:
    task: establish ipc using shared memory and let the child process
            write calculated collatz sequence direct to shared memory,
            
            and then parent can access and print it.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/mman.h>
#include <fcntl.h>

//define global constant
#define BUFFER_SIZE 256

//define structure for shared memory
typedef struct{
    int vec[BUFFER_SIZE];
    int size;
}shared_data_t;



//function definition
void collatz_sequence(int num,shared_data_t* mail_box){
    int i =0;
    while(num){
        // printf("%d ",num);
        mail_box->vec[i++] =num;
        if(num == 1){
            break;
        }
        if(num%2 == 0){
            num /=2;
        }else{
            num =3*num + 1;
        }
    }
    printf("\n");
    
    //make sure parent know the size of 'Collatz Sequence'
    mail_box->size =i;

    return;
}


//for debugging
#define PRINT printf("check\n");

//functions definition

void establish_shared_memory(int *shm_fd,const char* shared_object,shared_data_t* mail_box){
    //shm_open
    *shm_fd =shm_open(shared_object,O_CREAT|O_RDWR,0666);
    if(*shm_fd < 0){
        perror("shm_open failed\n");
        exit(1);
    }

    //ftruncate
    if(ftruncate(*shm_fd,sizeof(shared_data_t)) == -1){
        perror("ftruncate failed\n");
        close(*shm_fd);
        exit(1);
    }

    //map
    mail_box =mmap(NULL,sizeof(shared_data_t),PROT_WRITE|PROT_READ,MAP_SHARED,*shm_fd,0);
    if(mail_box == MAP_FAILED){
        perror("mmap failed\n");
        close(*shm_fd);
        exit(1);
    }

    printf("shared memory region created\n");
    return ;
}

int main(int argc,char* argv[]){
    //user input validation,assuming he will enter integer
    if(argc < 2){
        printf("ERROR: main_function_arguments_count_mismatch\n");
        exit(1);
    }


    const char* shared_object ="/my_shared_mem";
    int shm_fd;
    shared_data_t *mail_box;

    establish_shared_memory(&shm_fd,shared_object,mail_box);

    //try without semaphore
    
    //fork
    pid_t tmp_pid =fork();

    if(tmp_pid < 0){
        perror("fork failed\n");
        munmap(mail_box,sizeof(shared_data_t));
        close(shm_fd);
        exit(1);
    }else if(tmp_pid == 0){
        //child process
        printf("child[%d]:\v",getpid());

        int shm_fd =shm_open(shared_object,O_RDWR,0666);
        if(shm_fd < 0){
            perror("shm_open failed\n");
            exit(1);
        }

        //map
        shared_data_t* mail_box =mmap(NULL,sizeof(shared_data_t),PROT_WRITE|PROT_READ,MAP_SHARED,shm_fd,0);
        if(mail_box == MAP_FAILED){
            perror("mmap failed\n");
            close(shm_fd);
            exit(1);
        }
        printf("mailbox address: %p\n",mail_box);
        //call the collatz function
        int num =atoi(argv[1]);
        collatz_sequence(num,mail_box);

        //child confimation
        // for(int i =)
        printf("first element: %d\n",mail_box->vec[0]);

        munmap(mail_box,sizeof(shared_data_t));
        close(shm_fd);
        printf("%d: my work is done,leaving\n",getpid());
    }else{
        //parent process
        printf("parent[%d]:\v",getpid());

        //let child finish first
        wait(NULL);

        //print the collatz sequence from shared region
        // int size =mail_box->size;
        printf("mailbox address: %p\n",mail_box);
        // for(int i =0; i<size; i++){
        //     printf("%d ",mail_box->vec[i]);
        // }
        printf("\n");
        printf("my child[%d]: \nterminated\n",tmp_pid);


        //clean resoure
        munmap(mail_box,sizeof(shared_data_t));
        close(shm_fd);
        exit(0);
        
    }
    


}
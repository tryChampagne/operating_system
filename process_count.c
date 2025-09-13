/*DESCRIPTION:
    task: total number of process this program will create including parent
*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main(){
    printf("\nparent region\n\n");
    int process_count =0;//shared variable

    pid_t parent_pid =getpid();
    printf("Parent: %d\n",parent_pid);

    for(int i =0; i<4; i++){
        pid_t pid =fork();

        if(pid == 0){
            printf("\n%d: i am back\n",getpid());
            // exit(0);
        }else if(pid < 0){
            fprintf(stderr,"fork failed\n");
        }
    }

    //parent will wait for each child process
    for(int i =0; i<4; i++){
        int status;
        pid_t pid =wait(&status);
        if(pid > 0){
            printf("Parent: child[pid: %d] finished with status: %d\n",pid,WEXITSTATUS(status));
        }
    }

    //each process will execute below line as code section copied to child process too
    process_count++;




    //
    pid_t tmp =getpid();
    if(parent_pid == tmp){
        printf("Total process created includeing Me: %d\n",process_count);
        printf("Parent[pid: %d]: all child has terminated successfully,i am about too\n",tmp);

    }
}
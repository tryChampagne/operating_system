#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main(){
    pid_t pid1;

    pid1 =fork();

    if(pid1 < 0){
        fprintf(stderr,"fork failed\n");
        return 1;
    }else if(pid1 == 0){
        printf("\nchild region:\n");

        printf("child pid: %d\n",getpid());
        printf("parent pid: %d\n",getppid());
        printf("child region ends here\n\n");
    }else{
        // wait(NULL);
        printf("\nparent region:\n");

        printf("parent pid: %d\n",getpid());
        printf("parents parent pid: %d\n",getppid());
        printf("parent region ends here\n\n");

        sleep(30);
    }
}
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main(){
    pid_t pid1;

    pid1 =fork();

    pid_t pid2;

    if(pid1 < 0){
        fprintf(stderr,"fork failed\n");
        return 1;
    }else if(pid1 == 0){
        //child process
        pid_t pid_child =getpid();
        pid2 =pid_child;
        pid_t pid_parent =getppid();
        printf("child[%d] parent id is: %d\n",pid_child,pid_parent);
        return 0;
    }else{
        //parent process
        printf("parent process region\n");
        pid_t pid_parent =getpid();
        printf("parent[%d]: rember me!\n");
        printf("look for my child with id: %d\n",pid2);

        printf("hi\n");
        
        unsigned int time =10;
        sleep(time);
        return 0;
    }
}
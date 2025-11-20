/*DESCRIPTION:
    task: to run a application in child process address space

    findings:
        1.we don't need to create a pipe or initializa shared memory region
        in order to pass command line agruments to child ,because they are shared automatically.
        
        2.exit() function present in stdlib.h
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NEW_LINE printf("\n")
int main(int argc,char* argv[]){
    pid_t pid_child;
    
    //create process
    pid_child =fork();

    if(pid_child < 0){
        perror("fork failed\n");
        exit(1);
    }else if(pid_child == 0){
        printf("child: %d\n",getpid());
        printf("%s: %d\n",argv[0],getppid());

        //load application whose binary executable name given in command line argument
        execl("/sbin/iw",argv[1],argv[2],NULL);

        perror("unable to load application\n");
        exit(1);

    }else{
        //wait for child to finish first
        int status;
        wait(&status);
        printf("child[%d] terminated with code: %d",pid_child,status);

    }

    NEW_LINE;
}

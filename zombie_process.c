/*DESCRIPTION:
    task: creating a zombie process,by letting parent process to not call wait()

    findings:
        in order to list process use ps -l while our process(running program) is active in background
        using & 
            i.e ./a.out & ps -l
*/

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
        //child process
        printf("beginning of child territory\n");

        pid_t pid_child =getpid();
        pid_t pid_parent =getppid();
        printf("child[%d] parent id is: %d\n",pid_child,pid_parent);

        printf("end of child territory\n");
        return 0;
    }else{
        //parent procecss: no use of wait() system call insure that child will be zobmbie untill parent terminates itself.
        printf("parent process region\n");

        pid_t pid_parent =getpid();
        printf("parent[%d]: rember me!\n",pid_parent);
        printf("look for my child with id: %d\n",pid1);        
        unsigned int time =10;
        

        printf("parent: i am about to terminate\n");
        return 0;
    }
}
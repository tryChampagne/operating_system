/*DESCRIPTION:
    task: ipc using POSIX pipes,Half Duplex
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
    int fd[2];//read and write ends.

    char buffer[100];

    if(pipe(fd) < 0){
        perror("pipe failed\n");
        exit(1);
    }

    pid_t pid =fork();
    
    if(pid < 0){
        perror("fork failed\n");
        exit(1);
    }else if(pid == 0){
        printf("child[%d]: \n",getpid());
        //child process
        close(fd[1]);

        ssize_t byte_read =read(fd[0],buffer,sizeof(buffer));
        if(byte_read > 0){
            printf("child: message received => %s\n",buffer);
            return 0;
        }

    }else{
        //parent process
        printf("parent[%d]:\n",getpid());
        //close unused end
        close(fd[0]);

        const char* message ="Hi from Parent";
        write(fd[1],message,sizeof(message)+1);

        wait(NULL);

    }
}
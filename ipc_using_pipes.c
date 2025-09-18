/*DESCRIPTION:
    task: ipc using POSIX pipes,Half Duplex
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(){
    int fd[2];//read and write ends.

    if(pipe(fd) < 0){
        perror("pipe failed\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid =fork();
    
    if(pid < 0){
        perror("fork failed\n");
        exit(EXIT_FAILURE);

    }else if(pid == 0){
        printf("child[%d]: \n",getpid());
        char buffer[100];
        //child process
        close(fd[1]);

        ssize_t byte_read =read(fd[0],buffer,sizeof(buffer));
        if(byte_read > 0){
            printf("child: message received => %s\n",buffer);
        }else{
            perror("error occured when child tries to read.\n");
            exit(EXIT_FAILURE);
        }

        close(fd[0]);
        exit(EXIT_SUCCESS);

    }else{
        //parent process
        printf("parent[%d]:\n",getpid());
        //close unused end
        close(fd[0]);

        const char* message ="Hi from Parent";
        write(fd[1],message,strlen(message)+1);

        wait(NULL);

    }
}
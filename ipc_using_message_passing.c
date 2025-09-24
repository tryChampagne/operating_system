/*DESCRIPTION:
    IPC using Message Passing.
    OS intervention is required
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <mqueue.h>
#include <sys/wait.h>
#include <unistd.h>

//CONSTANT SECTION
#define MAIL_BOX "/kernel_shared_queue"
#define MAX_MSG_SIZE 50
#define BUFFER_SIZE 100

int main(){
    //buffer to read from and write to.
    char msg[BUFFER_SIZE];

    //set up mailbox/queue first
    mqd_t mq;
    
    //define attributes of file
    struct mq_attr attr;
    attr.mq_flags =0;
    attr.mq_maxmsg =10;
    attr.mq_msgsize =MAX_MSG_SIZE;
    attr.mq_curmsgs =0;

    mq =mq_open(MAIL_BOX,O_WRONLY|O_CREAT,0644,&attr);

    if(mq == (mqd_t)-1){
        perror("mq_open failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Parent: connected to kernel queue\n");


    pid_t id =fork();
    if(id < 0){
        perror("fork failed\n");
        exit(EXIT_FAILURE);
    }else if(id == 0){
        //child process
        mqd_t mq =mq_open(MAIL_BOX,O_RDONLY);
        if(mq == (mqd_t)-1){
            perror("mq_open failed\n");
            exit(EXIT_FAILURE);
        }

        //PRINT CHILD IS CONNECTED TO KERNEL QUEUE
        printf("CHILD: connected to kernel queue\n");

        //READ BYTES
        
        ssize_t byte_read =mq_receive(mq,msg,sizeof(msg),0);
        if(byte_read < 0){
            perror("mq_receive failed\n");
            exit(EXIT_FAILURE);
        }
        msg[byte_read] ='\0';
        //PRINT THE MESSAGE 
        printf("CHILD: message received =>\v%s\n",msg);

        mq_close(mq);
        mq_unlink(MAIL_BOX);
        exit(EXIT_SUCCESS);
    }else{
        //parent process
        
        snprintf(msg,BUFFER_SIZE,"Parent: we are waiting for you message\n");

        //send to kernel queue
        if(mq_send(mq,msg,strlen(msg)+1,0) == -1){
            perror("mq_send failed\n");
            exit(EXIT_FAILURE);
        }


        //PRINT MESSAGE SENT
        printf("PARENT: message is delivered to kernel queue\n");

        mq_close(mq);
        wait(NULL);

        
    }

}
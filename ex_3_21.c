//task: implement Collatz conjecture using child process.

//findings:
//  conjecture is something that seems to be true ,but not yet formally proven.

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

//function definition
void collatz_sequence(int num){
    while(num){
        printf("%d ",num);
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
    return;
}


int main(int argc,char* argv[]){
    //user input validation
    if(argc < 2){
        printf("argument_count_mismatch\n");
        return 1;
    }

    int num =atoi(argv[1]);
    if(num <= 0){
        printf("Input Error: program expect positive number\n");
        return 1;
    }

    //fork child
    pid_t tmp_id =fork();

    if(tmp_id < 0){
        perror("fork failed\n");
        return 1;
    }else if(tmp_id == 0){
        //child section
        printf("child[%d]: \v",getpid());

        //call function
        collatz_sequence(num);

        printf("%d: my work is done\n",getpid());
        return 0;
    }else{
        //parent section
        
        //first let child finish it's work
        wait(NULL);

        //child terminated.
        printf("parent[%d]: \v",getpid());

        printf("parent is exiting\n");
        return 0;

    }
}
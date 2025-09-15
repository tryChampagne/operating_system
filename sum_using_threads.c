/*DESCRIPTION:
    task: calculate sum of first 'n' natural number using threads
    
    thread library used: Pthread of POSIX
    
    input: 'n' via command line
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

//shared data
int sum =0;

//thread function
void* thread_handle(void* params);

int main(int argc,char* argv[]){
    //take snapshot of this time
    struct timeval start;
    gettimeofday(&start,NULL);

    //read 'n' value from user
    if(argc < 2){
        printf("2 argument expected\n");
        exit(1);
    }

    //assuming user will pass a valid integer.

    //thread stuff
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tid,&attr,thread_handle,argv[1]);

    pthread_join(tid,NULL);
    
    //let parent process print the calculated sum
    printf("sum of 1..%d: %d\n",atoi(argv[1]),sum);

    struct timeval end;
    gettimeofday(&end,NULL);

    double time_diff =(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;
    printf("time taken: %fs\n",time_diff);
}

void* thread_handle(void* params){
    int n =atoi(params);
    for(int i =1; i <=n; i++){
        sum +=i;
    }

    pthread_exit(0);
}
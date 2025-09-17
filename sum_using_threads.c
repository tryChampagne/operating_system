/*DESCRIPTION:
    task: calculate sum of first 'n' natural number using threads
    
    thread library used: Pthread of POSIX
    
    input: 'n' via command line

    nxt: create 2 threads and find sum using them.
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

//shared data
int sum =0;

//synchronization
pthread_mutex_t sum_mutex =PTHREAD_MUTEX_INITIALIZER;

//struct for thread arguments
typedef struct{
    int l;
    int h;
}thread_fun_args;

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
    pthread_t tid1,tid2;
    pthread_attr_t attr1,attr2;

    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);

    //cast input number to int first
    int n =atoi(argv[1]);
    
    //devide work between threads
    int mid =n/2;

    //for thread1
    thread_fun_args first_half;
    first_half.l =1;
    first_half.h =mid;


    //for thread2
    thread_fun_args second_half;
    second_half.l =mid+1;
    second_half.h =n;


    pthread_create(&tid1,&attr1,thread_handle,(void*)&first_half);
    pthread_create(&tid2,&attr2,thread_handle,(void*)&second_half);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    
    //let parent process print the calculated sum
    printf("sum of 1..%d: %d\n",atoi(argv[1]),sum);

    struct timeval end;
    gettimeofday(&end,NULL);

    double time_diff =(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;
    printf("time taken: %fs\n",time_diff);
}

void* thread_handle(void* params){
    thread_fun_args *tmp =(thread_fun_args*)params;
    for(int i =tmp->l; i <=tmp->h; i++){
        pthread_mutex_lock(&sum_mutex);
        sum +=i;
        pthread_t tid =pthread_self();
        printf("thread_%lu: curr sum is: %d\n",tid,sum);
        pthread_mutex_unlock(&sum_mutex);
    }

    pthread_exit(0);
}
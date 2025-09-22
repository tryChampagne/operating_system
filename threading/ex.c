#include <pthread.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
int value = 0;
void *runner(void *param); /* the thread */
int main(int argc, char *argv[])
{
pid_t pid;
pthread_t tid;
pthread_attr_t attr;


pthread_attr_init(&attr);
pthread_create(&tid,&attr,runner,NULL);
pthread_join(tid,NULL);
printf("CHILD: value = %d\n",value); /* LINE C */


wait(NULL);
printf("PARENT: value = %d\n",value); /* LINE P */

}
void *runner(void *param) {
value = 5;
pthread_exit(0);
}
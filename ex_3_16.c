/*DESCRIPTION:
    even global variable/data_structures are not shared between parent and child process
     untill explicit IPC mechanism set up (shared memory,message passing ,pipe etc.)
     ,otherwise both parent and child processes will have copy of data.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#define SIZE 5
int nums[SIZE] = {0,1,2,3,4};
int main()
{
int i;
pid_t pid;
pid = fork();
if (pid == 0) {
for (i = 0; i < SIZE; i++) {
nums[i] *= -i;
printf("CHILD: %d \n",nums[i]); /* LINE X */
}
}
else if (pid > 0) {
wait(NULL);
printf("\n");
for (i = 0; i < SIZE; i++)
printf("PARENT: %d \n",nums[i]); /* LINE Y */
}
else{
    perror("fork failed\n");
    return 1;//exit(1)
}
return 0;
}

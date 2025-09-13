/*DESCRIPTION:
    flow of execution and value return to parent by fork()
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
pid_t pid, pid1;
/* fork a child process */
pid = fork();
if (pid < 0) { /* error occurred */
fprintf(stderr, "Fork Failed");
return 1;
}
else if (pid == 0) { /* child process */
pid1 = getpid();
printf("LINE A: child: pid = %d\n",pid); /* A */
printf("LINE B: child: pid1 = %d\n",pid1); /* B */
}
else { /* parent process */
pid1 = getpid();
printf("LINE C: parent: pid = %d\n",pid); /* C */
printf("LINE D: parent: pid1 = %d\n",pid1); /* D */
wait(NULL);
}
return 0;
}
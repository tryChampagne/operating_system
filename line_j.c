/*DESCRIPTION:
    task: in what case "LINE J" will be printed?
    
    findings:
        exec family function on success never return control,so lines below exec call 
            will not we executed.
        
        however on failure, let say if program that we are trying to load in child space does not
            exist or some error occured in that case exec will return control to child process.
*/
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include<sys/wait.h>

int main()
{
pid_t pid;
/* fork a child process */
pid = fork();
if (pid < 0) { /* error occurred */
fprintf(stderr, "Fork Failed");
return 1;
}
else if (pid == 0) { /* child process */
execlp("/bin/ls","ls",NULL);
printf("LINE J");
}
else { /* parent process */
/* parent will wait for the child to complete */
wait(NULL);
printf("Child Complete");
}
return 0;
}
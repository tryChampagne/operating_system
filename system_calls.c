/*DESCRIPTION:
    task: use "strace" utility,to trace what system calls are executed when our program run.
*/
 
#include <stdio.h>

int main(){
    int a;
    printf("enter number: ");
    scanf("%d",&a);

    printf("square of this number: %d\n",a*a);

    return 0;  
}
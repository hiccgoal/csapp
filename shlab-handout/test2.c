#include <unistd.h>

int main()
{
 pid_t pid;
 
 if ((pid = fork()) == 0) {
  /* this is child process */
  printf("The return value is %d  In child process!!  My PID is %d,  My PPID is %d\n", pid,getpid(), getppid());
  exit(0);
 } 
  /* this is parent */
  printf("The return value is %d In parent process!!  My PID is %d,  My PPID is %d\n", pid,getpid(), getppid());
 
 return  0;
}

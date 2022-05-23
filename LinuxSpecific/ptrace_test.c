#define __x86_64__

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <stdio.h>

int main()
{
  pid_t child;
  struct user_regs_struct regs;
  long syscall;
  child = fork();
  if (child == 0) {
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    printf("\n");
    execl("/bin/ls", "ls", NULL);
  } else {
    wait(NULL);
    ptrace(PTRACE_GETREGS, child, 0, &regs);
    syscall = regs.orig_rax;
    printf("The child made a system call %ld\n", syscall);
    ptrace(PTRACE_CONT, child, NULL, NULL);
  }
  return 0;
}
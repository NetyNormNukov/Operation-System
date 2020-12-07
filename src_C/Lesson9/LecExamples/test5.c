#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <signal.h>

volatile sig_atomic_t flag = 0;

void on_sigusr1(int sig) {
  printf("Signal '%s'(%d) received! Total number: %d\n", sys_siglist[sig], sig, ++flag);
}

int main(void) {
  // Set a signal handler for SIGUSR1
  sigset_t old_set;
  signal(SIGUSR1, on_sigusr1);

  // At program startup, SIGUSR1 is neither blocked nor pending, so raising it
  // will call the signal handler
  raise(SIGUSR1);

   //Now let's block SIGUSR1
   {
      sigset_t sigset;
      sigemptyset(&sigset);
      sigaddset(&sigset, SIGUSR1);
      sigprocmask(SIG_BLOCK, &sigset, &old_set);
   }

  // SIGUSR1 is now blocked, raising it will not call the signal handler
  printf("About to raise SIGUSR1\n");
  raise(SIGUSR1);
  printf("After raising SIGUSR1\n");
  {
    sigset_t sigset;
    int sig_num;
    printf("Let see pending signals\n");
    sigemptyset(&sigset);
    if(sigpending(&sigset)) {
      perror("In main. sigpending");
      exit(EXIT_FAILURE);
    }
    for (sig_num = 1; sig_num <= SIGRTMAX; sig_num++) {
      if (sigismember(&sigset, sig_num)) {
        printf("\tSignal '%s' (%d) is pending!\n", sys_siglist[sig_num], sig_num);
      }
    }
  }

  //Temporary restore the old mask and get pending SIGUSR1 signal
  sigsuspend(&old_set);

  // SIGUSR1 is now again blocked, raising it will not call the signal handler
  printf("About to raise SIGUSR1\n");
  raise(SIGUSR1);
  printf("After raising SIGUSR1\n");

  return EXIT_SUCCESS;
}
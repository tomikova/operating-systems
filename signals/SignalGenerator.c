#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int pid=0;

void interrupt(int sig){
   kill(pid,SIGKILL);
   exit(0);
}

int main(int argc, char *argv[]){
   int sek,signal;
   pid=atoi(argv[1]);
   sigset (SIGINT, interrupt);
   srand((unsigned)time(NULL));
   
   while(1) {
	  sek=rand()%(7-3+1)+3;
      sleep(sek);
      signal=rand()%(4);
	  switch(signal) {
		  case 0:
			   kill(pid,SIGUSR1);
		  break;
		  case 1:
			   kill(pid,SIGUSR2);
		  break;
		  case 2:
			   kill(pid,SIGPIPE);
		  break;
		  case 3:
			   kill(pid,SIGSEGV);
		  break;
		  default:
				  printf("\nError");
		  break;
	  }
   }
   
   return 0;
}


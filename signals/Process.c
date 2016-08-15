#include <stdio.h>
#include <signal.h>
 
#define N 6   
 
int WAIT_FLAG[N]={0};
int PRIORITY[N]={0};
int CURRENT_PRIORITY=0;
 
int sig[]={SIGUSR1, SIGUSR2, SIGPIPE, SIGSEGV, SIGINT};
void disallowInterruption(){
   int i;
   for(i=0; i<5; i++)
      sighold(sig[i]);
}
void allowInterruption(){
   int i;
   for(i=0; i<5; i++)
      sigrelse(sig[i]);
}

void interrupt(int sig);

void process(int i){
	sigset (SIGUSR1, interrupt);
	sigset (SIGUSR2, interrupt);
	sigset (SIGPIPE, interrupt);
	sigset (SIGSEGV, interrupt);
	sigset (SIGINT, interrupt);
	int a;
	switch(i) {
		case 1:
			printf("\n-   P   -   -   -   -");
			for (a=1;a<6;a++){
			   printf("\n-   %d   -   -   -   -",a);
			   sleep(1);
			}             
			printf("\n-   K   -   -   -   -");
			break;
		case 2:
			printf("\n-   -   P   -   -   -");
            for (a=1;a<6;a++){
				printf("\n-   -   %d   -   -   -",a); 
				sleep(1);
            }           
            printf("\n-   -   K   -   -   -");
            break;
		case 3:
			printf("\n-   -   -   P   -   -");
            for (a=1;a<6;a++){        
				printf("\n-   -   -   %d   -   -",a);
				sleep(1);
            }
            printf("\n-   -   -   K   -   -");
			break;
		case 4:
			printf("\n-   -   -   -   P   -");
            for (a=1;a<6;a++){                
				printf("\n-   -   -   -   %d   -",a);
				sleep(1);
            }
            printf("\n-   -   -   -   K   -");
			break;
		default:
			printf("\n-   -   -   -   -   P");
            for (a=1;a<6;a++){                
				printf("\n-   -   -   -   -   %d",a);  
				sleep(1);
            } 
            printf("\n-   -   -   -   -   K");
			break;	
   }
}

void interrupt(int sig) {
   int n=1;
   int x,j;
   disallowInterruption();
   switch(sig) {
      case SIGUSR1: 
         n=1; 
         printf("\n-   X   -   -   -   -");
         break;
      case SIGUSR2: 
         n=2; 
         printf("\n-   -   X   -   -   -");
         break;
      case SIGPIPE:
         n=3; 
         printf("\n-   -   -   X   -   -");
         break;
      case SIGSEGV:
         n=4; 
         printf("\n-   -   -   -   X   -");
         break;
      case SIGINT:
         n=5; 
         printf("\n-   -   -   -   -   X");
         break;
      default:
		printf("\nUnknown signal");
		break;
   }
   WAIT_FLAG[n]++;
   do {
       x=0;
       for(j=CURRENT_PRIORITY+1;j<=n;j++) {
           if (WAIT_FLAG[j]>0) {
                x=j;
           }
       }
       if (x>0) {
		   WAIT_FLAG[x]--;
		   PRIORITY[x]=CURRENT_PRIORITY;
		   CURRENT_PRIORITY=x;
		   allowInterruption();
		   process(x);
		   disallowInterruption();
		   CURRENT_PRIORITY=PRIORITY[x];
       }
      } while (x>0);
      allowInterruption();
}
           
int main ( void )
{
   sigset (SIGUSR1, interrupt);
   sigset (SIGUSR2, interrupt);
   sigset (SIGPIPE, interrupt);
   sigset (SIGSEGV, interrupt);
   sigset (SIGINT, interrupt);
   
   int z;
   printf("PID=%d\n", getpid());
   printf("\nGP S1  S2  S3  S4  S5\n_____________________");
   sleep(10);
   
   for (z=0;z<=20;z++) {
       sleep(1);
       printf("\n%d   -   -   -   -   -",z);
   }
   
   printf ("\n\nEnd of program\n");
   return 0;
}

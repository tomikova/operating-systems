#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int SemId, Id;
int *ENTER, *EXIT;
typedef struct{
  char M[5];
}MS;
MS *ms;

int write=0;
int full=1;
int empty=2;
int sem1=3;
int sem2=4;

void SemGet(int n){
   SemId = semget(IPC_PRIVATE, n, 0600);
   if (SemId == -1) {
      printf("No semaphore!\n");
      exit(1);
   }
}

int SemSetVal(int SemNum, int SemVal){  
   return semctl(SemId, SemNum, SETVAL, SemVal);
}

int SemOp(int SemNum, int SemOp) {  
   struct sembuf SemBuf;
   SemBuf.sem_num = SemNum;
   SemBuf.sem_op  = SemOp;
   SemBuf.sem_flg = 0;
   return semop(SemId, & SemBuf, 1);
}

void SemRemove(int sig) { 
   (void) semctl(SemId, 0, IPC_RMID, 0);
}

void delete (int sig){
    (void) shmdt((char *) ENTER);
    (void) shmdt((char *) EXIT);
    (void) shmdt((char *) ms);
    (void) shmctl(Id, IPC_RMID, NULL);
    exit(0);
}
 
 void producer (int i) {
    char message[25];
    int j=0;
    if(i==1)
		sleep(1);
    SemOp(sem1,-1);
    printf("Producer %d:\n",i+1);
    scanf("%s",message); 
    SemOp(sem1,1);
    if(i==0)
		SemOp(sem2,-1);
     else
		SemOp(sem2,1);

    do {
        SemOp(full,-1);
        SemOp(write,-1);   
        ms->M[*ENTER]=message[j];
        printf("Producer%d -> %c \n",i+1,ms->M[*ENTER]);
        *ENTER=(*ENTER+1)%5;
        SemOp(write,1);
        SemOp(empty,1);
        j+=1;
        sleep(1);
    } while(message[j-1]!='\0');
}
       
 void consumer(){
	sleep(1);
	char message[50];
	int i=0;
	int j=2;
	do {
		SemOp(empty,-1);
		if (ms->M[*EXIT]!='\0'){
			printf("Consumer <- %c\n",ms->M[*EXIT]);
		}
		else
			j--;
		message[i]=ms->M[*EXIT];
		*EXIT=(*EXIT+1)%5;
		SemOp(full,1);
		i+=1;
	} while(j!=0);    
	printf("Received %s\n",message);
}

int main(int argc, char *argv[]) {
	  int i,k;
	  SemGet(5);

	  Id=shmget(IPC_PRIVATE,sizeof(int)*100,0600);
	  if(Id==-1)
		   exit (1);
		   
	  ENTER=(int *)shmat(Id,NULL,0);
	  *ENTER=0;
	  EXIT=(int *)shmat(Id,NULL,0)+1;
	  *EXIT=0;
	  ms=(MS *)shmat(Id,NULL,0)+2;
	  for(i=0;i<5;i++){
		ms->M[i]=0;
	  }
	  SemSetVal(write,1);
	  SemSetVal(full,5);
	  SemSetVal(empty,0);
	  SemSetVal(sem1,1);
	  SemSetVal(sem2,0);
	  
	  sigset(SIGINT,delete);
	  sigset(SIGINT,SemRemove);
  
	for (i=0;i<2;i++){
		switch(fork()){
			case -1:
				printf("Can't create new process!\n");
				exit(0);
			case 0:
				producer(i);
				exit(0);
           default:
                break;
		}
	}
      
	switch(fork()){
		case -1:
			printf("Can't create new process!!\n");
			exit(0);
		case 0:
			consumer();
			exit(0);
		default:
			break;
	}
    for (i=0;i<3;i++){
         wait(NULL);
    }
  
	system("PAUSE");	
	return 0;
}

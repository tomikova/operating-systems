#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int Id;
int *RIGHT;
int *FLAG;

void criticalSection (int i, int j);
void criticalSectionExit (int i, int j);

void Proces (int i){
    int k,m,j;
    if (i==0){
		j=1;
    }
    else if (i==1){
        j=0;
    }
    for (k=1;k<=5;k++) {
        criticalSection (i,j);
        for (m=1;m<=5;m++){
            printf("Process: %d, C.S. number: %d, (%d/5)\n",i+1,k,m);
            sleep(1);
        }
        criticalSectionExit (i,j);
    }
}
     
void criticalSection (int i, int j){
    FLAG[i]=1;
    while(FLAG[j]!=0){
        if (*RIGHT==j){
            FLAG[i]=0;
            while (*RIGHT==j);
				FLAG[i]=1;
        }
    }
}
     
void criticalSectionExit (int i, int j){
    *RIGHT=j;
    FLAG[i]=0;
}
     
void delete (int sig){
     (void) shmdt((char *) RIGHT);
     (void) shmdt((char *) FLAG);
     (void) shmctl(Id, IPC_RMID, NULL);
     exit(0);
}

int main(int argc, char *argv[]){
	
    int i;
    Id= shmget(IPC_PRIVATE, sizeof(int)*100, 0600);
    if (Id==-1)
		exit(1);
    RIGHT=(int *) shmat(Id, NULL, 0);
    *RIGHT=0;
    FLAG=(int *) shmat(Id, NULL, 0)+1;
    for (i=0;i<2;i++){
        FLAG[i]=0;
    }
    sigset (SIGINT, delete);
    for (i=0;i<2;i++){
        switch(fork()){
           case -1:
                printf("Can't create new process!\n");
                exit(0);
           case 0:
                Proces(i);
                exit(0);
           default:
                break;
        }
    }
    for (i=0;i<2;i++){
        wait(0);
    } 
  
  system("PAUSE");	
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>

int *SEARCH;
int *NUMBER;
int N;

void criticalSection (int i);
void criticalSectionExit (int i);

void *Thread (void *i){
    int k,m,y;
    y=*((int *)i);
    for (k=1;k<=5;k++){
        criticalSection(y);
        for (m=1;m<=5;m++){
            printf("Thread: %d, C.S. number: %d, (%d/5)\n",y+1,k,m);
            sleep(1);
        }
        criticalSectionExit(y);
    }
}

void criticalSection (int i){
     int max,j,x;
     max=0;
     SEARCH[i]=1;
     for (x=0;x<N;x++){
        if (NUMBER[x]>max){
            max=NUMBER[x];
        }
    }
     NUMBER[i]=max+1;
     SEARCH[i]=0;
     for (j=0;j<N;j++){
        while (SEARCH[j]!=0);
        while (NUMBER[j]!=0 && (NUMBER[j]<NUMBER[i] || (NUMBER[j]==NUMBER[i] && j<i)));
    }
}
      
void criticalSectionExit (int i){
    NUMBER[i]=0;
}

int main(int argc, char *argv[])
{
    int i;
    int *br;
    pthread_t *t;
    N=atoi(argv[1]);
    SEARCH=(int *) malloc (N*sizeof(int));
    NUMBER=(int *) malloc (N*sizeof(int));
    br=(int *) malloc (N*sizeof(int));
    t=(pthread_t *) malloc (N*sizeof(pthread_t));
    for (i=0;i<N;i++){
        SEARCH[i]=0;
        NUMBER[i]=0;
    }
    for (i=0;i<N;i++){
        br[i]=i;
        sleep(1);
        if(pthread_create(&t[i],NULL,Thread,((void *)&br[i]))){
            printf("Can't create new thread!\n");
            exit(1);
        }
    }
    for (i=0;i<N;i++){
        pthread_join(t[i],NULL);
     }
	 
    free(SEARCH);
    free(NUMBER);
    free(br);
    free(t);
    
  system("PAUSE");	
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <malloc.h>

pthread_mutex_t monitor;
pthread_cond_t red[5];
int fork[5]={1,1,1,1,1};
char philosophers[5]={'O','O','O','O'};

void think() {
    sleep(3);
}
     
void eat (int n){
    int i;
    pthread_mutex_lock(&monitor);
    philosophers[n]='o';
    while (fork[n]==0 || fork[(n+1)%5]==0){
        pthread_cond_wait(&red[n],&monitor);
    }
    fork[n]=fork[(n+1)%5]=0;
    philosophers[n]='X';
    for(i=0;i<5;i++){
		printf("%c ",philosophers[i]);
    }
    printf(" (%d)\n",(n+1));
    pthread_mutex_unlock(&monitor);
     
    sleep(2);
     
    pthread_mutex_lock(&monitor);
    philosophers[n]='O';
    fork[n]=fork[(n+1)%5]=1;
    pthread_cond_signal(&red[(n-1)%5]);
    pthread_cond_signal(&red[(n+1)%5]);
    for(i=0;i<5;i++){
		printf("%c ",philosophers[i]);
    }
    printf(" (%d)\n",(n+1));
    pthread_mutex_unlock(&monitor);
}

void *philosopher(void *i){
    int j;
    j=*((int *)i);
    while(1){
		think();
		eat(j);
    }
 }

int main(int argc, char *argv[]){
    int i;
    int *br;
    pthread_mutex_init (&monitor,NULL);
    pthread_cond_init (red,NULL);
    pthread_t *t;
    t=(pthread_t *)malloc(5*sizeof(pthread_t));
    br=(int *)malloc(5*sizeof(int));
    for (i=0;i<5;i++){
        br[i]=i;
        if(pthread_create(&t[i],NULL,philosopher,((void *)&br[i]))){
			printf("Can't create new thread! \n");
			exit(1);
        }
    }
    for (i=0;i<5;i++){
        pthread_join(t[i],NULL);
    }
    free(t);
	free(br);
	
	system("PAUSE");	
	return 0;
}

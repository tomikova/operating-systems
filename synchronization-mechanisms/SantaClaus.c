#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t monitor;
pthread_cond_t red[3];

int santaClaus=0;
int reindeer=1;
int consultations=2;
int reindeerNum=0;
int elvesNum=0;


void *Elf(){
    printf("Elf is waiting at the door\n");
    pthread_mutex_lock(&monitor);
    elvesNum++;
    if (elvesNum==3){
		pthread_cond_signal(&red[santaClaus]);
    }
    pthread_cond_wait(&red[consultations],&monitor);
    pthread_mutex_unlock(&monitor);
}

void *Reindeer(){
    printf("Reindeer has come\n");
    pthread_mutex_lock(&monitor);
    reindeerNum++;
    if(reindeerNum==10){
        pthread_cond_signal(&red[santaClaus]);
    }
    pthread_cond_wait(&red[reindeer],&monitor);
    pthread_mutex_unlock(&monitor);
}

void *SantaClaus(){
   int i;
   pthread_mutex_lock(&monitor);
   do{
		pthread_cond_wait(&red[santaClaus],&monitor);
		printf("Santa Claus is awake!\n");
		if(reindeerNum==10 && elvesNum>0){
		   printf("Gifts delivery\n");
		   sleep(2);
		   pthread_cond_broadcast(&red[reindeer]);
		   printf("Children received gifts! d^_^b\n");
		   printf("Reindeer are going on vacation\n");
		   reindeerNum=0;
		}
		if(reindeerNum==10){
		   printf("Feeding the reindeer\n");
		   sleep(2);                      
		}
		while (elvesNum>=3){
			  printf("Consultation with elves\n");
			   sleep(2);
			  for(i=0;i<3;i++){
				pthread_cond_signal(&red[consultations]);
				}
			   elvesNum-=3;
		}
    } while(1);
    pthread_mutex_unlock(&monitor);
}

void *NorthPole(){
   int reindeerProbability, elfProbability,doWait;
   pthread_t reindeer;
   pthread_t patuljci;
   srand((unsigned)time(NULL));
   do{
       doWait=rand()%(3-1+1)+1;                          
       sleep(doWait);
       reindeerProbability=rand()%(100-0+1);
       elfProbability=rand()%(100-0+1);
       if((reindeerProbability>50) && (reindeerNum<10)){
          if (pthread_create(&reindeer,NULL,Reindeer,NULL)){
             printf("Can't create new thread!\n");
             exit(1);
          } 
       }
       if(elfProbability>50){
          if (pthread_create(&patuljci,NULL,Elf,NULL)){
             printf("Can't create new thread!\n");
             exit(1);
          } 
        }
    } while(1);
}


int main(int argc, char *argv[]) {
  pthread_mutex_init (&monitor,NULL);
  pthread_cond_init (red,NULL);
  pthread_t thread1;
  pthread_t thread2;
  if (pthread_create(&thread1,NULL,NorthPole,NULL)){
    printf("Can't create new thread!\n");
    exit(1);
  }
  if (pthread_create(&thread2,NULL,SantaClaus,NULL)){
    printf("Can't create new thread!\n");
    exit(1);
  }
  pthread_join(thread1,NULL);  
  pthread_join(thread1,NULL); 
  
  system("PAUSE");	
  return 0;
}

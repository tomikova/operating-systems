#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

int main(int argc, char *argv[]) {
	int *pages, *requests, *counter, *pom;
	int pg, req, i, j, k, max, temp, full, hit, rem;
	double stat;
	full=-1;
	hit=0;
	
	pg=atoi(argv[1]);
	req=atoi(argv[2]);
	
	pages=(int*)malloc(pg*sizeof(int));
	counter=(int*)malloc(pg*sizeof(int));
	pom=(int*)malloc(pg*sizeof(int));
	requests=(int*)malloc(req*sizeof(int));
	
	for(i=0;i<pg;i++){
		pages[i]=0;
		counter[i]=0;
		pom[i]=0;
	}
	
	srand((unsigned)time(NULL));
	for(i=0;i<req;i++){
		requests[i]=rand()%(8-1+1)+1;
	}
	printf("\n\nZahtjevi: ");
	for(i=0;i<req-1;i++){
		printf("%d,",requests[i]);
	}
	printf("%d",requests[req-1]);
	printf("\n\n");
	
	printf("\n#N     ");
	for(i=0;i<pg;i++){
		printf("%d     ",i+1);
	}
	printf("\n");
	for(i=0;i<(7*pg);i++)
	printf("-");
	printf("\n");
	
	for(i=0;i<req;i++){
		printf(" %d    ",requests[i]);
		temp=i;
		for(j=0;j<pg;j++){
			if(pages[j]==requests[i])
			hit=1;
		}
		if(hit==0)
		full++;
		if(hit){
			for(j=0;j<pg;j++){
				if(pages[j]==requests[i])     /*if it is hit*/
				printf("(%d)   ",requests[i]);
				else {
					if(pages[j]==0)
					printf(" -    ");
					else
					printf(" %d    ",pages[j]);
				}
			}
			printf("  #hit");
		}
		else{
			if(full<pg){                   /*if it is not a hit look for free space*/
				for(j=0;j<pg;j++){
					if(j==full){
						pages[j]=requests[i];
						printf("[%d]   ",pages[j]);
					}
					else {
						if(pages[j]==0)
						printf(" -    ");
						else
						printf(" %d    ",pages[j]);
					}
				}
			}
			else{
				for(j=0;j<pg;j++){                 /*if it is not a hit and if there is no free space, use optimal strategy*/
					for(k=temp;k<req;k++){
						if(pages[j]==requests[k]){
							if(pom[j]==0){
								counter[j]=k;
								pom[j]=1;
							}
						}
					}
				}
				max=counter[0];
				rem=0;
				for(j=0;j<pg;j++){
					if(counter[j]==0){
						rem=j;
						break;
					}
					if((counter[j]>max)&&counter[j]!=0){
						max=counter[j];
						rem=j;
					}
				}
				for(j=0;j<pg;j++){
					if(j==rem){
						pages[j]=requests[i];
						printf("[%d]   ",pages[j]);
					}
					else
						printf(" %d    ",pages[j]);
				}   
			}
		}
		rem=0;
		hit=0;
		for(k=0;k<pg;k++){
			counter[k]=0;
			pom[k]=0;
		}
		printf("\n");
		sleep(1);
	}
	
	printf("\n\n"); 
	stat=((double)(full+1))/req*100;
	printf("Misses: %f percent \n\n",stat);
	free(pages);
	free(counter);
	free(requests);
	free(pom);
	return 0;
}								

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <bits/stdc++.h>

//#include <semaphore.h>

using namespace std;


pthread_t *producers;
pthread_t *consumers;

atomic<int> empty_count,fill_count;

int *buffer,buf_pos=-1,buf_len;


int produce(pthread_t self){
	int p = 1 + rand()%40;
	printf("Producer produced %d \n",p);
	return p;
}


void consume(int p,pthread_t self){
	// printf("Buffer:");
	// for(i=0;i<=buf_pos;++i)
	// 	printf("%d ",*(buffer+i));
	printf("\nConsumer consumed %d \nCurrent buffer len: %d\n",p,buf_pos);

}

void sem_wait(atomic<int> *a){
	while(a<= 0){
		//do nothing
	}
	*a--;

}

void sem_post(atomic<int> *a){
	*a++;
}

void sem_init(atomic<int> *a,int b, int c){
	*a=c;
}
void* producer(void *args){

	while(1){
		int p = produce(pthread_self());
		sem_wait(&empty_count);
	//	sem_wait(&buf_mutex);
		++buf_pos;			// critical section
		*(buffer + buf_pos) = p;
		//sem_post(&buf_mutex);
		sem_post(&fill_count);
		sleep(1 + rand()%3);
	}

	return NULL;
}


void* consumer(void *args){
	int c;
	while(1){
		sem_wait(&fill_count);
		//sem_wait(&buf_mutex);
		c = *(buffer+buf_pos);
		consume(c,pthread_self());
		--buf_pos;
		//sem_post(&buf_mutex);
		sem_post(&empty_count);
		sleep(1+rand()%5);
	}

	return NULL;
}

int main(void){

	int err;


	//sem_init(&buf_mutex,0,1);
	sem_init(&fill_count,0,0);

	producers = (pthread_t*) malloc(1*sizeof(pthread_t));

	consumers = (pthread_t*) malloc(1*sizeof(pthread_t));

	printf("Enter buffer capacity:");
	scanf("%d",&buf_len);
	buffer = (int*) malloc(buf_len*sizeof(int));

	sem_init(&empty_count,0,buf_len);


		err = pthread_create(producers,NULL,&producer,NULL);
		if(err != 0){
			printf("Error creating producer %d: %s\n",1,strerror(err));
		}else{
			printf("Successfully created producer %d\n",1);
		}


		err = pthread_create(consumers,NULL,&consumer,NULL);
		if(err != 0){
			printf("Error creating consumer %d: %s\n",1,strerror(err));
		}else{
			printf("Successfully created consumer %d\n",1);
		}



	pthread_join(*(producers),NULL);
  pthread_join(*(consumers),NULL);



	return 0;
}

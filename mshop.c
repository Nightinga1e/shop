#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int shopm [5];
int fl =0;
pthread_mutex_t mutex;
int counter = 0;

void* customer(void *args){
	pthread_mutex_lock(&mutex);
	int customernum = counter + 1;
	counter = counter + 1;
	pthread_mutex_unlock(&mutex);

	int need = 10000 + rand() % 1000;
	do{
		pthread_mutex_lock(&mutex); /////////////LOCK
		int selshop = rand() % 5;
		printf("Customer№ %d selected store = %d\n",customernum, selshop+1);
		need = need - shopm[selshop];
		printf("Took %d units of goods\n", shopm[selshop]);
		shopm[selshop] = 0;
		if(need < 0){
			shopm[selshop] = shopm[selshop] + ((-1) * need);
			need = 0;
		}
		printf("Need %d more\n\n", need);
		pthread_mutex_unlock(&mutex); ////////////UNLOCK
		sleep(3);
	}while (need != 0);
	fl = fl+1;
}

void* myloader(void *args){
	do{
		pthread_mutex_lock(&mutex); //////////LOCK
		int selshop = rand() % 5;
		printf("Loader: selected store = %d\n", selshop+1);
		shopm[selshop] = shopm[selshop] + 500;
		printf("Loader: now in the %d store %d units of goods\n\n", selshop, shopm[selshop]);
		pthread_mutex_unlock(&mutex); ////////UNLOCK
		sleep(2);
	}while( fl != 3);
}

int main()
{
	srand(time(NULL));
	int i;
	for (i=0; i<5; i++){
		shopm[i] = 1000 + rand() % 100;
		printf("Quantity of goods in the %d store = %d\n", i+1, shopm[i]);
	}
	pthread_t custhreads[3];
	pthread_t loaderthread;
	pthread_mutex_init(&mutex, NULL);
	for (i = 0; i < 3; i++){
		pthread_create(&custhreads[i], NULL, customer, NULL);
	}
	pthread_create(&loaderthread, NULL, myloader, NULL);
	for (i = 0; i<3; i++){
		pthread_join(custhreads[i], NULL);
	}
	pthread_join(loaderthread, NULL);
	if(fl == 3){
		pthread_mutex_destroy(&mutex);
		return 0;
	}
}

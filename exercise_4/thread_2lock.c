#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>


int first_count = 0;
int second_count = 0;
int nthread = 1;
int nthread_one = 1;

int main_loop_cnt = 1;

pthread_mutex_t first_lock;
pthread_mutex_t second_lock;


static void *work_one(void* num)
{
	int number = (int)num;
	int answer = 0;
	
	printf("Aquring mutex 1\n");
	pthread_mutex_lock(&first_lock);
	printf("Aquried mutex 1\n");
	printf("Aquring mutex 2\n");
	pthread_mutex_lock(&second_lock);// lock
	printf("Aquried mutex 2\n");

	answer = first_count + second_count;
	
	printf("Work_one : %d \n", answer);

	first_count++;
	second_count++;

	printf("Releasing mutex 2\n");
	pthread_mutex_unlock(&second_lock); //unlock
	printf("Releaed mutex 2\n");
	printf("Releasing mutex 1\n");
	pthread_mutex_unlock(&first_lock);
	printf("Released mutex 1\n");
	

	return NULL;
}

static void *work_two(void* num)
{
    int number = (int)num;
    int answer = 0;

    printf("Aquring mutex 1\n");
    pthread_mutex_lock(&first_lock);
    printf("Aquried mutex 1\n");
    printf("Aquring mutex 2\n");
    pthread_mutex_lock(&second_lock);// lock
    printf("Aquried mutex 2\n");

    answer = first_count + second_count;

    printf("Work_two : %d \n", answer);

    first_count++;
	second_count++;

    printf("Releasing mutex 2\n");
	pthread_mutex_unlock(&second_lock); //unlock
	printf("Releaed mutex 2\n");
    printf("Releasing mutex 1\n");
    pthread_mutex_unlock(&first_lock);
    printf("Released mutex 1\n");  

    return NULL;
}



int main(int argc, char *argv[])
{
	pthread_t *th;
	void *value;
	long i;

	if (argc < 3){
		fprintf(stderr, "%s parameter : nthread, worker_loop_cnt\n", argv[0]);
		exit(-1);
	}

	nthread = atoi(argv[1]);
	nthread_one = nthread/2;

	main_loop_cnt = atoi(argv[2]);
	th = malloc(sizeof(pthread_t) * nthread);

	pthread_mutex_init(&first_lock, NULL);
	pthread_mutex_init(&second_lock, NULL);

	
	for (int loop = 0; loop < main_loop_cnt; loop++){
		printf("--- loop %d ---\n", loop);
	
		for(i = 0; i < nthread_one; i++)
			assert(pthread_create(&th[i], NULL, work_one, (void*) i) == 0);
        
		for(i = nthread_one; i < nthread; i++)
			assert(pthread_create(&th[i], NULL, work_two, (void*) i) == 0);
		
        for(i = 0; i < nthread_one; i++)
			assert(pthread_join(th[i], &value) == 0);

	first_count = 0;
	second_count = 0;
	}
	printf("complete\n");
}


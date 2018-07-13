// libraries

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>


// buffer - FIFO queue

struct Buffer {
// these are struct Buffers's main variables (defining the buffer itself)
int front;
int rear;
int size;
int capacity;
int array[9];
// these are important shared variables that count specific processes waiting for resources; could be defined in another structure
int waitingProducers;
int waitingOddConsumers;
int waitingEvenConsumers;
};

void insert_el(struct Buffer *buf, int val)
{
    if((buf -> size) == (buf -> capacity))
    {
        printf("Buffer is full!\n");
        return;
    }
    else
    {
        buf -> rear = ((buf -> rear) + 1) % (buf -> capacity);
        buf -> array[buf -> rear] = val;
        buf -> size = (buf -> size) + 1;
        printf("%d added to buffer.\n", val);
    }
}

int remove_el(struct Buffer *buf)
{
    if((buf -> size) == 0)
    {
        printf("Buffer is empty!\n");
        return -1;
    }
    int item = buf -> array[buf -> front];
    buf -> front = ((buf -> front) + 1) % (buf -> capacity);
    buf -> size = (buf -> size) - 1;
    printf("%d removed from buffer.\n", item);
    return item;
}

// global variables

 struct Buffer *buf;
 sem_t *mutex;
 sem_t *producer_mutex;
 sem_t *even_consumer_mutex;
 sem_t *odd_consumer_mutex;

// functions: consumers and producers; why 2 producers? because I wanted them to generate random numbers independently

void producer1()
{
    printf("Producer 1 created!\n");
    while(1)
    {
        sem_wait(mutex);
        if((buf -> size) == (buf -> capacity))
        {
            sem_post(mutex);
            buf -> waitingProducers += 1;
            sem_wait(producer_mutex);
            buf -> waitingProducers -= 1;
        }

        // production
        if(buf -> array[buf -> front] != 0)
        {
            srand(buf -> array[buf -> front]);
        }
        int produced_number_1 = rand();
        printf("%d produced by producer 1.\n", produced_number_1);
        insert_el(buf, produced_number_1);

        if(((buf -> array[buf -> front]) % 2 == 0) && ((buf -> size) >= 3) && (buf -> waitingEvenConsumers > 0))
        {
            sem_post(even_consumer_mutex);
        }
        else if(((buf -> array[buf -> front]) % 2 != 0) && ((buf -> size) >= 3) && (buf -> waitingOddConsumers > 0))
        {
            sem_post(odd_consumer_mutex);
        }
        else if(((buf -> size) < (buf -> capacity)) && (buf -> waitingProducers > 0))
        {
            sem_post(producer_mutex);
        }
        else
        {
            sem_post(mutex);
        }
        usleep(rand() % 2000000);
    }
}

void producer2()
{
    printf("Producer 2 created!\n");
    while(1)
    {
        sem_wait(mutex);
        if((buf -> size) == (buf -> capacity))
        {
            sem_post(mutex);
            buf -> waitingProducers += 1;
            sem_wait(producer_mutex);
            buf -> waitingProducers -= 1;
        }

        // production
        time_t t;
        srand(time(&t));
        int produced_number_2 = rand();
        printf("%d produced by producer 2.\n", produced_number_2);
        insert_el(buf, produced_number_2);

        if(((buf -> array[buf -> front]) % 2 == 0) && ((buf -> size) >= 3) && (buf -> waitingEvenConsumers > 0))
        {
            sem_post(even_consumer_mutex);
        }
        else if(((buf -> array[buf -> front]) % 2 != 0) && ((buf -> size) >= 3) && (buf -> waitingOddConsumers > 0))
        {
            sem_post(odd_consumer_mutex);
        }
        else if(((buf -> size) < (buf -> capacity)) && (buf -> waitingProducers > 0))
        {
            sem_post(producer_mutex);
        }
        else
        {
            sem_post(mutex);
        }
        usleep(rand() % 2000000);
    }
}

void oddConsumer()
{
    printf("Odd consumer created!\n");
    while(1)
    {
        sem_wait(mutex);
        if(((buf -> size) < 3) || ((buf -> array[buf -> front]) % 2 == 0))
        {
            sem_post(mutex);
            buf -> waitingOddConsumers += 1;
            sem_wait(odd_consumer_mutex);
            buf -> waitingOddConsumers -= 1;
        }

        // consumption
        int consumed_odd_number = remove_el(buf);
        if(consumed_odd_number == -1)
        {
            printf("There was an attempt to remove element from empty buffer!\n");
        }
        else
        {
            printf("%d consumed by odd consumer.\n", consumed_odd_number);
        }

        if(((buf -> array[buf -> front]) % 2 == 0) && ((buf -> size) >= 3) && (buf -> waitingEvenConsumers > 0))
        {
            sem_post(even_consumer_mutex);
        }
        else if(((buf -> array[buf -> front]) % 2 != 0) && ((buf -> size) >= 3) && (buf -> waitingOddConsumers > 0))
        {
            sem_post(odd_consumer_mutex);
        }
        else if(((buf -> size) < (buf -> capacity)) && (buf -> waitingProducers > 0))
        {
            sem_post(producer_mutex);
        }
        else
        {
            sem_post(mutex);
        }
    }
}

void evenConsumer()
{
    printf("Even consumer created!\n");
    while(1)
        {
        sem_wait(mutex);
        if(((buf -> size) < 3) || ((buf -> array[buf -> front]) % 2 != 0))
        {
            sem_post(mutex);
            buf -> waitingEvenConsumers += 1;
            sem_wait(even_consumer_mutex);
            buf -> waitingEvenConsumers -= 1;
        }

        // consumption
        int consumed_even_number = remove_el(buf);
        if(consumed_even_number == -1)
        {
            printf("There was an attempt to remove element from empty buffer!\n");
        }
        else
        {
            printf("%d consumed by even consumer.\n", consumed_even_number);
        }

        if(((buf -> array[buf -> front]) % 2 == 0) && ((buf -> size) >= 3) && (buf -> waitingEvenConsumers > 0))
        {
            sem_post(even_consumer_mutex);
        }
        else if(((buf -> array[buf -> front]) % 2 != 0) && ((buf -> size) >= 3) && (buf -> waitingOddConsumers > 0))
        {
            sem_post(odd_consumer_mutex);
        }
        else if(((buf -> size) < (buf -> capacity)) && (buf -> waitingProducers > 0))
        {
            sem_post(producer_mutex);
        }
        else
        {
            sem_post(mutex);
        }
    }
}

void newOddConsumer()
{
    int forkRet = fork();

    if(forkRet == 0)
    {
        oddConsumer();
        exit (0);
    }

}

void newEvenConsumer()
{
    int forkRet = fork();

    if(forkRet == 0)
    {
        evenConsumer();
        exit (0);
    }

}

void newProducer1()
{
    int forkRet = fork();

    if(forkRet == 0)
    {
        producer1();
        exit(0);
    }
}

void newProducer2()
{
    int forkRet = fork();

    if(forkRet == 0)
    {
        producer2();
        exit(0);
    }
}

int main()
{
    // buffer initialization

    buf = (struct Buffer *)mmap(NULL, sizeof(struct Buffer), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    buf -> front = 0;
    buf -> rear = 8;
    buf -> size = 0;
    buf -> capacity = 9;
    buf -> waitingProducers = 0;
    buf -> waitingOddConsumers = 0;
    buf -> waitingEvenConsumers = 0;

    // semaphores initialization

    mutex = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    producer_mutex = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    odd_consumer_mutex = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    even_consumer_mutex = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(mutex, 1, 1);
    sem_init(producer_mutex, 1, 0);
    sem_init(odd_consumer_mutex, 1, 0);
    sem_init(even_consumer_mutex, 1, 0);

    // producers and consumers creation

    newOddConsumer();
    newEvenConsumer();
    newProducer1();
    newProducer2();

    while(wait(NULL) > 0);  // zwraca -1 gdy proces nie ma dzieci

    return 0;
}

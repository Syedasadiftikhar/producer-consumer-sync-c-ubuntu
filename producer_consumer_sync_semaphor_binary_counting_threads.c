#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFERSIZE 10

// Buffer and its pointers
int buffer[BUFFERSIZE];
int in = 0, out = 0;

// Binary semaphore logic
int mutex = 1;    // Binary semaphore for critical section
int empty = BUFFERSIZE; // Counter for empty slots
int full = 0;     // Counter for filled slots

// Function prototypes
void down(int *s);
void up(int *s);
void *producer(void *arg);
void *consumer(void *arg);

// Down function (binary semaphore)
void down(int *s) {
    while (*s <= 0); // Busy-wait if the semaphore value is <= 0
    (*s)--;          // Decrease the semaphore value
}

// Up function (binary semaphore)
void up(int *s) {
    (*s)++;          // Increase the semaphore value
}

// Producer function
void *producer(void *arg) {

        down(&empty); // Wait for an empty slot
        down(&mutex); // Enter critical section

        // Produce an item
        int item = rand() % 100;
        buffer[in] = item;
        printf("Producer produced item %d at position %d\n", item, in);
        in = (in + 1) % BUFFERSIZE; // Circular buffer logic

        up(&mutex); // Exit critical section
        up(&full);  // Signal a full slot


    pthread_exit(NULL);
}

// Consumer function
void *consumer(void *arg) {

        down(&full);  // Wait for a full slot
        down(&mutex); // Enter critical section

        // Consume an item
        int item = buffer[out];
        printf("Consumer consumed item %d from position %d\n", item, out);
        out = (out + 1) % BUFFERSIZE; // Circular buffer logic

        up(&mutex); // Exit critical section
        up(&empty); // Signal an empty slot
  

    pthread_exit(NULL);
}

int main() {
    int num_producers, num_consumers;

    printf("Enter the number of producers: ");
    scanf("%d", &num_producers);
    printf("Enter the number of consumers: ");
    scanf("%d", &num_consumers);

    // Threads for producers and consumers
    pthread_t producers[num_producers], consumers[num_consumers];

    // Create producer threads
    for (int i = 0; i < num_producers; i++) {
        if (pthread_create(&producers[i], NULL, producer, NULL) != 0) {
            perror("Failed to create producer thread");
            exit(1);
        }
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; i++) {
        if (pthread_create(&consumers[i], NULL, consumer, NULL) != 0) {
            perror("Failed to create consumer thread");
            exit(1);
        }
    }

    // Wait for producer threads to finish
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }

    // Wait for consumer threads to finish
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    printf("\nAll producers and consumers have finished their tasks.\n");
    return 0;
}

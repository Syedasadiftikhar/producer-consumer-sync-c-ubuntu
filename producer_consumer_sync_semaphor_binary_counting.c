#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 10

// Global variables
int mutex = 1;         // Semaphore for mutual exclusion
int empty, full = 0;   // Semaphores for empty and full slots
int buffer[BUFFERSIZE]; // Buffer to hold items
int in = 0, out = 0;   // Pointers for producer and consumer
int n, item;           // Number of items and current item

// Wait function for semaphore
void wait(int *s) {
    while (*s <= 0) {
        printf("\nCannot proceed, waiting...\n");
        return;
    }
    (*s)--;
}

// Signal function for semaphore
void signal(int *s) {
    (*s)++;
}

// Producer function
void producer() {
    if (empty <= 0) {
        printf("\nBuffer is full. Cannot produce more items.\n");
        return;
    }

    wait(&empty);       // Wait for an empty slot
    wait(&mutex);       // Enter critical section

    printf("\nEnter an item: ");
    scanf("%d", &item);
    buffer[in] = item;
    in = (in + 1) % BUFFERSIZE; // Circular buffer logic

    signal(&mutex);     // Exit critical section
    signal(&full);      // Signal that a slot is full
}

// Consumer function
void consumer() {
    if (full <= 0) {
        printf("\nBuffer is empty. Cannot consume.\n");
        return;
    }

    wait(&full);        // Wait for a full slot
    wait(&mutex);       // Enter critical section

    int item1 = buffer[out];
    printf("\nConsumed item = %d", item1);
    out = (out + 1) % BUFFERSIZE; // Circular buffer logic

    signal(&mutex);     // Exit critical section
    signal(&empty);     // Signal that a slot is empty
}

// Main function
int main() {
    printf("Enter the value of n (number of items to produce): ");
    scanf("%d", &n);

    if (n > BUFFERSIZE) {
        printf("The value of n exceeds the buffer size (%d).\n", BUFFERSIZE);
        return -1;
    }

    empty = BUFFERSIZE; // Initialize empty slots

    // Run producer and consumer
    while (in < n) {
        producer();
    }

    while (out < n) {
        consumer();
    }

    printf("\nAll items produced and consumed successfully.\n");
    return 0;
}

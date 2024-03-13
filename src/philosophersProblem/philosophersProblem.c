/* ----------------------------------------------------------------------------
Copyright (C), 2024-2025, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: philosophersProblem.c
@version: I
Creation: 11/03/2024
Last modification: 12/03/2024
References
[GfG. (2023). Dining Philosopher problem using semaphores. GeeksforGeeks. https://www.geeksforgeeks.org/dining-philosopher-problem-using-semaphores/]
------------------------------------------------------------------------------*/
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// philosophers number and identifiers
#define N 5 
int phil[N] = { 0, 1, 2, 3, 4 }; 

// philosopher states
int state[N];
enum { THINKING, HUNGRY, EATING }; 

// semaphores for critical section and each philosopher
sem_t mutex;
sem_t S[N];

// --- eating check function ---
void checkAndEat(int phnum) {
    if (state[phnum] == HUNGRY && state[(phnum + 4) % N] != EATING && state[(phnum + 1) % N] != EATING) {
        state[phnum] = EATING;
        printf("^ philosopher %d takes fork [%d] and [%d]\n", phnum + 1, (phnum + 4) % N + 1, phnum + 1);
        printf("---- philosopher %d is eating ----\n", phnum + 1);
		sleep(2);

		// philosopher can start eating signal
        sem_post(&S[phnum]); 
    }
}

// --- take fork function ---
void takeFork(int phnum) {

	// critical section
    sem_wait(&mutex); 
    state[phnum] = HUNGRY;
    printf("!philosopher no. %d is hungry\n", phnum + 1);

	// eating check
    checkAndEat(phnum); 
    sem_post(&mutex);

	// wait if unable to eat
    sem_wait(&S[phnum]); 
}

// --- place fork function ---
void placeFork(int phnum) {

	// critical section
    sem_wait(&mutex); 
    state[phnum] = THINKING;
    printf("~ philosopher no. %d is placing fork [%d] and [%d] down\n", phnum + 1, (phnum + 4) % N + 1, phnum + 1);
    printf("philosopher no. %d is thinking...\n", phnum + 1);
	sleep(1 + rand() % 3);
	// left neighbor can eat checker
    checkAndEat((phnum + 4) % N); 

	// right neighbor can eat checker
    checkAndEat((phnum + 1) % N);
    sem_post(&mutex);
}

// --- philosopher function ---
void* philosopher(void* num) {
    int* i = num;
    while (1) {
		sleep(1);
        takeFork(*i);
        placeFork(*i);
    }
}

// --- main function ---
int main() {

	// mutex semaphore initialization
    pthread_t thread_id[N];
    sem_init(&mutex, 0, 1); 

	// philosopher semaphores initialization
    for (int i = 0; i < N; i++) {
        sem_init(&S[i], 0, 0);
    }

	// philosopher threads creation
    for (int i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
        printf("philosopher no. %d is thinking...\n", i + 1);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }
}
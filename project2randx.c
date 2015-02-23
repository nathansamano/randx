/*
 * Nathan Samano
 * November 12, 2013
 * Project 2: Producer/Consumer Randomness Tester.
 *            Multiple threads generate random numbers using 3 different methods + 1 more for extra credit
 */
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#define DEBUG 0                  // main
#define DEBUG2 2                 // producer-consumer values (set to 0 to watch the producing/consuming
#define DEBUG3 0                 // main dislplay results
#define DEBUG_RAND 0             // each producer-consumer function
#define SIZE 10                  // size of buffer
#define NUM_TO_TEST 1000000      // number of random numbers generated
pthread_mutex_t mutex1;          // mutex variable
pthread_mutex_t mutex2;          // "            "
pthread_mutex_t mutex3;          // "            "
pthread_mutex_t mutex4;          // "            "
pthread_cond_t condvar1;         // condition variable
pthread_cond_t condvar2;         // "                "
pthread_cond_t condvar3;         // "                "
pthread_cond_t condvar4;         // "                "
int buffer1[SIZE]={0};           // shared variable storing random numbers for RNG #1
int buffer2[SIZE]={0};           // shared variable storing random numbers for RNG #2
int buffer3[SIZE]={0};           // shared variable storing random numbers for RNG #3
int buffer4[SIZE]={0};           // shared variable storing random numbers for RNG #4
int numOfNodes1=0;               // number of items in buffer1
int numOfNodes2=0;               // number of items in buffer2
int numOfNodes3=0;               // number of items in buffer3
int numOfNodes4=0;               // number of items in buffer4
int digitCount1[SIZE]={0};       // count lowest order digit for RNG #1
int digitCount2[SIZE]={0};       // count lowest order digit for RNG #2
int digitCount3[SIZE]={0};       // count lowest order digit for RNG #3
int digitCount4[SIZE]={0};       // count lowest order digit for RNG #4

void *producer(void *ptr) {   // produce data
    int i=0;              // counting for loop
    int rand_num=0;       // random number integer
    int rear=0;           // rear of buffer queue
    int randu=1;          // randu value
    int newseed=0;        // middle-square method value
    int x=42424242;       // set-random 8-digit starting #

/***** RNG #1 random() *****/
    if (DEBUG_RAND == 0) {
       if ((int)ptr == 1) { // required to run all four threads at the same time, ptr to pro1
          srandom((unsigned int)1);     // setting seed
          for (i=0; i<NUM_TO_TEST; i++) {
             rand_num = random();               // call random()
             if (DEBUG2 == 1) {
                printf("rand_num = %d\n", rand_num); // test rng #
             } // if
             pthread_mutex_lock(&mutex1);    // get exclusive access to buffer
             while (numOfNodes1 == SIZE) {
                pthread_cond_wait(&condvar1, &mutex1);   // if buffer is full, producer cannot do anything so block
             } // while
             /*** Enqueue ***/
             numOfNodes1 = numOfNodes1 + 1; // increment # of nodes
             buffer1[rear] = rand_num;      // set the rng # to rear of buffer1
             if (DEBUG2 == 1) {
                printf("Rear: %d\n", rear); // print value of rear
             } // if
             if (DEBUG2 == 0) {
                printf("Producer 1 storing the number %d, on loop %d\n", buffer1[rear], i);  // print loop# and rng#
             } // if
             rear = (rear + 1) % SIZE; // circle queue
             /*** Enqueue ***/
             pthread_cond_signal(&condvar1);     // wake up for consumer
             pthread_mutex_unlock(&mutex1);  // release access to buffer
          } // for
          pthread_exit(0); // exit thread
       } // if ptr
    } // if DEBUG_RAND

/***** RNG #2 constant *****/
    if (DEBUG_RAND == 0) {
       if ((int)ptr == 2) { // required to run all four threads at the same time, ptr to pro2
          for (i=0; i<NUM_TO_TEST; i++) {
             pthread_mutex_lock(&mutex2);  // get exclusive access to buffer
             while (numOfNodes2 == SIZE) {
                pthread_cond_wait(&condvar2, &mutex2);  // if buffer is full, producer cannot do anything so block
             } // while
             /*** Enqueue ***/
             numOfNodes2 = numOfNodes2 + 1; // increment # of nodes
             buffer2[rear] = 42;  // set 42 to rear of buffer2
             if (DEBUG2 == 0) {
                printf("Producer 2 storing the number %d, on loop %d\n", buffer2[rear], i); // print loop# and 42
             } // if
             rear = (rear + 1) % SIZE; // cirlce queue
             /*** Enqueue ***/
             pthread_cond_signal(&condvar2);   // wake up for consumer
             pthread_mutex_unlock(&mutex2);    // release access to buffer
          } // for
          pthread_exit(0); // exit thread
       } // if ptr
    } // if DEBUG_RAND

/***** RNG #3 RANDU *****/
    if (DEBUG_RAND == 0) {
       if ((int)ptr == 3) { // required to run all four threads at the same time, ptr to pro3
          for (i=0; i<NUM_TO_TEST; i++) {
             pthread_mutex_lock(&mutex3);  // get exclusive access to buffer
             while (numOfNodes3 == SIZE) {
                pthread_cond_wait(&condvar3, &mutex3);  // if buffer is full, producer cannot do anything so block
             } // while
             /*** Enqueue ***/
             numOfNodes3 = numOfNodes3 + 1; // increment # of nodes
             buffer3[rear] = randu;         // set value of randu to rear of buffer3
             randu = randu%(int)pow(2,31) * 65539; // RANDU formula
             if (DEBUG2 == 0) {
                printf("Producer 3 storing the number %d, on loop %d\n", buffer3[rear], i); // print loop# and randu
             } // if
             rear = (rear + 1) % SIZE; // circle queue
             /*** Enqueue ***/
             pthread_cond_signal(&condvar3);  // wake up for consumer
             pthread_mutex_unlock(&mutex3);   // release access to buffer
          } // for
          pthread_exit(0); // exit thread
       } // if ptr
    } // if DEBUG_RAND

/***** RNG #4 Middle-Square Method *****/
    if (DEBUG_RAND == 0) {
       if ((int)ptr == 4) { // required to run all four threads at the same time, ptr to pro4
          for (i=0; i<NUM_TO_TEST; i++) {
             pthread_mutex_lock(&mutex4);  // get exclusive access to buffer
             while (numOfNodes4 == SIZE) {
                pthread_cond_wait(&condvar4, &mutex4); // if buffer is full, producer cannot do anything so block
             } // while
             /*** Enqueue ***/
             numOfNodes4 = numOfNodes4 + 1; // increment # of nodes
             newseed = (x / 1000) % 10000;  // select the middle four digits for the next seed
             if (DEBUG2 == 4) {
                printf("Newseed = %d\n", newseed);
             } // if 
             buffer4[rear] = newseed;
             /*if (newseed == 0) {      // flaw with this RNG is that it will inevitably output zeros forever
                newseed = time(NULL); // so to counter that result reset newseed to another number (why not base it on time)
             } // if */
             x = newseed * newseed; // this is the squaring that takes place
             if (DEBUG2 == 0) {
                printf("Producer 4 storing the number %d, on loop %d\n", buffer4[rear], i); // print loop# and value
             } // if
             rear = (rear + 1) % SIZE; // circle queue
             /*** Enqueue ***/
             pthread_cond_signal(&condvar4); // wake up for consumer
             pthread_mutex_unlock(&mutex4);  // release access to buffer
          } // for
          pthread_exit(0); // exit thread
       } // if ptr
    } // if DEBUG_RAND

} // producer()

void *consumer(void *ptr) {  // process data
    int i=0;              // counting for loop
    int front=0;          // front of buffer queue
    int digit=0;          // examine lowest order digit

/***** RNG #1 random() *****/
    if (DEBUG_RAND == 0) {
       if ((int)ptr == 1) { // required to run all four threads at the same time, ptr to con1
          for (i=0; i<NUM_TO_TEST; i++) {
             pthread_mutex_lock(&mutex1);    // get exclusive access to buffer
             while (numOfNodes1 == 0) {
                pthread_cond_wait(&condvar1, &mutex1);   // block if nothing in buffer
             } // while
             if (DEBUG2 == 0) {
                printf("Consumer 1 retrieving the number %d, on loop %d\n", buffer1[front], i); // print loop# rng#
             } // if
             /*** Examination ***/
             digit = buffer1[front] % 10; // get last digit of rng #
             digitCount1[digit]++;        // increment count of that digit
             /*** Examination ***/
             /*** Dequeue ***/
             front = (front + 1) % SIZE;    // cirlce queue
             numOfNodes1 = numOfNodes1 - 1; // decrement # of nodes
             /*** Dequeue ***/
             pthread_cond_signal(&condvar1); // wake up for producer
             pthread_mutex_unlock(&mutex1);  // release access to buffer
          } // for
          pthread_exit(0); // exit thread
       } // if ptr
    } // if DEBUG_RAND

/***** RNG #2 constant *****/
    if (DEBUG_RAND == 0) {
       if ((int)ptr == 2) { // required to run all four threads at the same time, ptr to con2
          for (i=0; i<NUM_TO_TEST; i++) {
             pthread_mutex_lock(&mutex2);   // get exclusive access to buffer
             while (numOfNodes2 == 0) {
                pthread_cond_wait(&condvar2, &mutex2);   // block if nothing in buffer
             } // while
             if (DEBUG2 == 0) {
                printf("Consumer 2 retrieving the number %d, on loop %d\n", buffer2[front], i);  // print loop # and 42
             } // if
             /*** Examination ***/
             digit = buffer2[front] % 10; // get last digit of rng #
             digitCount2[digit]++;        // increment count of that digit
             /*** Examination ***/
             /*** Dequeue ***/
             front = (front + 1) % SIZE;    // cirlce queue
             numOfNodes2 = numOfNodes2 - 1; // decrement # of nodes
             /*** Dequeue ***/
             pthread_cond_signal(&condvar2);   // wake up for producer
             pthread_mutex_unlock(&mutex2);    // release access to buffer
          } // for
          pthread_exit(0); // exit thread
       } // if ptr
    } // if DEBUG_RAND

/***** RNG #3 randu *****/
    if (DEBUG_RAND == 0) {
       if ((int)ptr == 3) { // required to run all four threads at the same time, ptr to con3
          for (i=0; i<NUM_TO_TEST; i++) {
             pthread_mutex_lock(&mutex3);  // get exclusive access to buffer
             while (numOfNodes3 == 0) {
                pthread_cond_wait(&condvar3, &mutex3);  // block if nothing in buffer
             } // while
             if (DEBUG2 == 0) {
                printf("Consumer 3 retrieving the number %d, on loop %d\n", buffer3[front], i);  // print loop # and randu
             } // if
             /*** Examination ***/
             digit = abs(buffer3[front]) % 10; // get last digit of rng # (must be positive so abs is required)
             digitCount3[digit]++;             // increment count of that digit
             /*** Examination ***/
             /*** Dequeue ***/
             front = (front + 1) % SIZE;    // cirlce queue
             numOfNodes3 = numOfNodes3 - 1; // decrement # of nodes
             /*** Dequeue ***/
             pthread_cond_signal(&condvar3);  // wake up for producer
             pthread_mutex_unlock(&mutex3);   // release access to buffer
          } // for
          pthread_exit(0); // exit thread
       } // if ptr
    } // if DEBUG_RAND

/***** RNG #4 Middle-Square Method *****/
    if (DEBUG_RAND == 0) {
       if ((int)ptr == 4) { // required to run all four threads at the same time, ptr to con4
          for (i=0; i<NUM_TO_TEST; i++) {
             pthread_mutex_lock(&mutex4);  // get exclusive access to buffer
             while (numOfNodes4 == 0) {
                pthread_cond_wait(&condvar4, &mutex4); // block if nothing in buffer
             } // while
             if (DEBUG2 == 0) {
                printf("Consumer 4 retrieving the number %d, on loop %d\n", buffer4[front], i); // print loop # and value
             } // if
             /*** Examination ***/  // need abs otherwise data flows into buffer3 due to negatives
             digit = abs(buffer4[front]) % 10; // get last digit of rng #
             if (DEBUG2 == 0) {
                printf("digit = %d\n", digit);
             } // if
             digitCount4[digit]++;        // increment count of that digit
             /*** Examination ***/
             /*** Dequeue ***/
             front = (front + 1) % SIZE;    // circle queue
             numOfNodes4 = numOfNodes4 - 1; // decrement # of nodes
             /*** Dequeue ***/
             pthread_cond_signal(&condvar4); // wake up for producer
             pthread_mutex_unlock(&mutex4);  // release access to buffer
          } // for
          pthread_exit(0);
       } // if ptr
    } // if DEBUG_RAND

} // consumer()

int main(int arg) {
    int i=0;          // counting variable
    float poccur=0.0; // digit count
    float pdiff=0.0;  // % difference from average

/***** RNG #1 random() *****/
    if (DEBUG == 0) {
       pthread_t pro1;                        // thread pro1
       pthread_t con1;                        // thread con1
       pthread_t pro2;                        // thread pro2
       pthread_t con2;                        // thread con2
       pthread_t pro3;                        // thread pro3
       pthread_t con3;                        // thread con3
       pthread_t pro4;                        // thread pro4
       pthread_t con4;                        // thread con4
       pthread_mutex_init(&mutex1, 0);        // init mutex1
       pthread_cond_init(&condvar1, 0);       // init condvar1
       pthread_mutex_init(&mutex2, 0);        // init mutex2
       pthread_cond_init(&condvar2, 0);       // init condvar2
       pthread_mutex_init(&mutex3, 0);        // init mutex3
       pthread_cond_init(&condvar3, 0);       // init condvar3
       pthread_mutex_init(&mutex4, 0);        // init mutex4
       pthread_cond_init(&condvar4, 0);       // init condvar4
       pthread_create(&con1, 0, consumer, (void*)1); // create con1 thread
       pthread_create(&pro1, 0, producer, (void*)1); // create pro1 thread
       pthread_create(&con2, 0, consumer, (void*)2); // create con2 thread
       pthread_create(&pro2, 0, producer, (void*)2); // create pro2 thread
       pthread_create(&con3, 0, consumer, (void*)3); // create con3 thread
       pthread_create(&pro3, 0, producer, (void*)3); // create pro3 thread
       pthread_create(&con4, 0, consumer, (void*)4); // create con4 thread
       pthread_create(&pro4, 0, producer, (void*)4); // create pro4 thread
       pthread_join(pro1, 0);                 // parent process will wait for thread to exit
       pthread_join(con1, 0);                 // "            " "           " "            "
       pthread_join(pro2, 0);                 // parent process will wait for thread to exit
       pthread_join(con2, 0);                 // "            " "           " "            "
       pthread_join(pro3, 0);                 // parent process will wait for thread to exit
       pthread_join(con3, 0);                 // "            " "           " "            "
       pthread_join(pro4, 0);                 // parent process will wait for thread to exit
       pthread_join(con4, 0);                 // "            " "           " "            "
       pthread_cond_destroy(&condvar1);       // free resource
       pthread_mutex_destroy(&mutex1);        // free resource
       pthread_cond_destroy(&condvar2);       // free resource
       pthread_mutex_destroy(&mutex2);        // free resource
       pthread_cond_destroy(&condvar3);       // free resource
       pthread_mutex_destroy(&mutex3);        // free resource
       pthread_cond_destroy(&condvar4);       // free resource
       pthread_mutex_destroy(&mutex4);         // free resource

/***** Display Results *****/
       if (DEBUG3 == 0) {
          printf("\nResults for process A:\n");
          printf("\tMod i\tNumber of Occurances\t %% Difference from average\n");
          for (i=0; i<SIZE; i++) {
             poccur = digitCount1[i];       // calculate % diff from avg
             poccur = poccur / NUM_TO_TEST;
             pdiff = ((poccur - 0.1) / ((poccur + 0.1) / 2)) * 100;
             printf("\t%d \t%d \t\t\t\t%.2f%%\n", i, digitCount1[i], pdiff); // print results
          } // for
       } // if DEBUG3
       if (DEBUG3 == 0) {
          printf("\nResults for process B:\n");
          printf("\tMod i\tNumber of Occurances\t %% Difference from average\n");
          for (i=0; i<SIZE; i++) {
             poccur = digitCount2[i];       // calculate % diff from avg
             poccur = poccur / NUM_TO_TEST;
             pdiff = ((poccur - 0.1) / ((poccur + 0.1) / 2)) * 100;
             printf("\t%d \t%d \t\t\t\t%.2f%%\n", i, digitCount2[i], pdiff); // print results
          } // for
       } // if DEBUG3
       if (DEBUG3 == 0) {
          printf("\nResults for process C:\n");
          printf("\tMod i\tNumber of Occurances\t %% Difference from average\n");
          for (i=0; i<SIZE; i++) {
             poccur = digitCount3[i];       // calculate % diff from avg
             poccur = poccur / NUM_TO_TEST;
             pdiff = ((poccur - 0.1) / ((poccur + 0.1) / 2)) * 100;
             printf("\t%d \t%d \t\t\t\t%.2f%%\n", i, digitCount3[i], pdiff); // print results
          } // for
       } // if DEBUG3
       if (DEBUG3 == 0) {
          printf("\nResults for process D:\n");
          printf("\tMod i\tNumber of Occurances\t %% Difference from average\n");
          for (i=0; i<SIZE; i++) {
             poccur = digitCount4[i];       // calculate % diff from avg
             poccur = poccur / NUM_TO_TEST;
             pdiff = ((poccur - 0.1) / ((poccur + 0.1) / 2)) * 100;
             printf("\t%d \t%d \t\t\t\t%.2f%%\n", i, digitCount4[i], pdiff); // print results
          } // for
       } // if DEBUG3
    } // if DEBUG

} // main 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
using namespace std;

#define BUFFER_SIZE 5
typedef int item;
item buffer[BUFFER_SIZE] = {0};

int counter_limit = 0; // how many times the threads are allowed to run, in total
int counter_track = 0; // keeps track of runs

int runner = 0; // initialize the threads position in the buffer
int chaser = 0;

void *producer_function (void *param); // prototypes
void *consumer_function (void *param);

void print_buffer();
void generate_random(int &num);

int main(int argc, const char * argv[]) {
    
    cout << "Enter counter limit: ";
    cin >> counter_limit;
    cout << endl;

    // thread 1
    pthread_t producer_t;                   // thread identifier
    pthread_attr_t default_attributes;       // set default thread attributes
    pthread_attr_init(&default_attributes);  // initialize thread with attribute
    pthread_create(&producer_t, &default_attributes, producer_function, argv); // create thread
    
    // thread 2
    pthread_t consumer_t;
    pthread_attr_t default_attributes_2;
    pthread_attr_init(&default_attributes_2);
    pthread_create(&consumer_t, &default_attributes_2, consumer_function, argv);
    
    
    pthread_join(producer_t, NULL); // main thread waits until child threads are done
    pthread_join(consumer_t, NULL);
    
    return 0;
}

void print_buffer() // prints the buffer
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        cout << buffer[i] << " ";
    }
    cout << endl;
}

void generate_random(int &num)
{
    num = rand() % 10 + 1; // random number 1-10
}

void *producer_function (void *param)
{
    item next_produced;

    while (counter_track <= counter_limit)
    {
        generate_random(next_produced);
        buffer[runner] = next_produced; // assign buffer index an item
        counter_track++;
        print_buffer();
        cout << "Item " << next_produced << " produced" << endl << endl;

        while ( ((runner + 1) % BUFFER_SIZE) == chaser )
        {
            ; // do nothing while the next index is occupied by second thread
        }

        runner = (runner + 1) % BUFFER_SIZE; // shift forward to next index
    }
    pthread_exit(0);
}

void *consumer_function (void *param)
{
    item next_consumed;
    
    while (counter_track <= counter_limit)
    {
        while (runner == chaser)
        {
            ; // do nothing while first thread's position in the buffer is the same as the consumer (can't consume the same index if the producer is at the same index)
        }
        
        next_consumed = buffer[chaser]; // retrieved item
        buffer[chaser] = 0; // re-initialized retrieved index to 0
        counter_track++;
        print_buffer();
        cout << "Item retrieved: " << next_consumed << endl << endl;
        
        chaser = (chaser + 1) % BUFFER_SIZE; // shift forward to next index
    }
    pthread_exit(0);
}

#ifndef COUNTER_H
#define COUNTER_H

#include <stdio.h>
#include "linearalg.h"

typedef struct CounterItem { 
    double label; 
    int count;
} CounterItem;

typedef struct Counter {
    CounterItem* data;
    size_t capacity;
    size_t size;
} Counter;

typedef struct CounterResult {
    CounterItem* data;
    size_t size; 
} CounterResult; 


Counter createCounter(size_t capacity);


Counter _counter_from_array(double* data, size_t cap);
Counter counterFromVector(Vector* vec);


int counterAdd(Counter* cnt, double val);
void counterAddArray(Counter* cnt, double* arr, size_t size);
void counterAddVector(Counter* cnt, Vector* vec);
CounterResult counterMostCommon(Counter* cnt, size_t nBiggest);

int counterWinnerCount(Counter* cnt);
void counterOrderValues(Counter* cnt);

Counter counterFromVector(Vector* vec);

// Macros
#define counterFromArray(arr)  _counter_from_array(arr, sizeof(arr)/sizeof(arr[0]))

#endif
#include <stdio.h>
#include <stdlib.h>
#include "counter.h"
#include "linearalg.h"
#include "utils.h"


// Sorting comparing function for descending sorting
static int __cnt_cmp_sort(const void* ptrA, const void* ptrB) {
    const CounterItem *ci1 = (const CounterItem*) ptrA;
    const CounterItem *ci2 = (const CounterItem*) ptrB;

    // Descending order by count
    int diff = (ci2->count - ci1->count);
    if (diff == 0) { // If the count is the same, order the labels ascending
        if (ci1->label < ci2->label) 
            return -1;
        if (ci1->label > ci2->label) 
            return 1;
        return 0;
    }

    return diff;
}

// Get the index of a value inside the counter
static size_t __cnt_val_index(Counter* cnt, double val) {
    for (size_t i = 0; i < cnt->size; i++) {
        if (cnt->data[i].label == val) {
            return i;
        }
    }

    return -1; 
}

// Expand the array to fit new items
static void __cnt_expd_arr(Counter* cnt) {
    size_t newSize = cnt->capacity + (cnt->capacity * 0.5);
    CounterItem* newPtr = (CounterItem*) realloc(cnt->data, newSize * sizeof(CounterItem));

    if (!newPtr) {
        perror("ERROR: Realloc did not return a valid memory address in Counter expand array");
        return;
    }

    cnt->data = newPtr;
}

static void __cnt_add_val(Counter* cnt, double val) {
    if (cnt->size + 1 > cnt->capacity) {
        __cnt_expd_arr(cnt);
    }

    CounterItem cntIt;
    cntIt.count = 1;  
    cntIt.label = val;
    
    cnt->size += 1;  
    cnt->data[cnt->size - 1] = cntIt;
}


// Adds the value and returns it's count (1 if new, "count" if the val is already in the counter)
int counterAdd(Counter* cnt, double val) {
    size_t idx = __cnt_val_index(cnt, val);
    if (idx == -1) {
        __cnt_add_val(cnt, val);
        return 1;
    }

    cnt->data[idx].count += 1; 
    return cnt->data[idx].count;
}

void counterAddArray(Counter* cnt, double* arr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        counterAdd(cnt, arr[i]);
    }
}

void counterAddVector(Counter* cnt, Vector* vec) {
    for (size_t i = 0; i < vec->len; i++) {
        counterAdd(cnt, vec->data[i]);
    }
}

CounterResult counterMostCommon(Counter* cnt, size_t nBiggest) {
    if (nBiggest < 1) 
        return (CounterResult){0};

    // Descending order sorting
    qsort(cnt->data, cnt->size, sizeof(CounterItem), __cnt_cmp_sort);

    nBiggest = nBiggest > cnt->size ? cnt->size : nBiggest; // Failsafe 
    CounterItem* new_data = ALLOC_ARRAY(new_data, nBiggest);
    for (size_t i = 0; i < nBiggest; i++){
        new_data[i] = cnt->data[i];
    }

    CounterResult cntr; 
    cntr.data = new_data; 
    cntr.size = nBiggest;
    return cntr;
}

// Returns how many labels have the highest value count
int counterWinnerCount(Counter* cnt) {
    CounterResult results = counterMostCommon(cnt, 1);
    int biggest = results.data[0].count;

    size_t winners = 0;
    for (size_t i = 0; i < cnt->size; i++) {
        if(cnt->data[i].count == biggest)
            winners++;
    }
    
    return winners; 
}

// Order the counter to give the most common ones
void counterOrderValues(Counter* cnt) {
    qsort(cnt->data, cnt->size, sizeof(CounterItem), __cnt_cmp_sort);
}

Counter createCounter(size_t capacity) {
    Counter cnt; 
    cnt.data = (CounterItem*) malloc(sizeof(CounterItem) * capacity);
    cnt.capacity = capacity; 
    cnt.size = 0;

    return cnt; 
}

Counter _counter_from_array(double* data, size_t cap) {
    Counter cnt = createCounter(cap);
    counterAddArray(&cnt, data, cap);

    return cnt;
}

Counter counterFromVector(Vector* vec) {
    Counter cnt = createCounter(vec->len);
    counterAddVector(&cnt, vec);

    return cnt;
}


// int main() {
//     double data[] = {1.0, 3.0, 4.0, 5.0, 1.0, 1.0, 2.0, 3.0, 10.0, 3.0, 11.0};
//     Counter counter = counterFromArray(data);

//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 5.0);
//     counterAdd(&counter, 5.0);
//     counterAdd(&counter, 5.0);
//     counterAdd(&counter, 5.0);
//     counterAdd(&counter, 5.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 1.0);
//     counterAdd(&counter, 112.0);
//     counterAdd(&counter, 12.0);
//     counterAdd(&counter, 12.0);
//     counterAdd(&counter, 12.0);
//     counterAdd(&counter, 12.0);

//     CounterResult bigs = counterMostCommon(&counter, 30);
//     for (size_t i = 0; i < bigs.size; i++){
//         printf("%f -> %d\n", bigs.data[i].label, bigs.data[i].count);
//     }
    
// }
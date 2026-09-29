#include <set.h>
#include <stdlib.h>
#include <stdio.h> 

#include "set.h"
#include "utils.h"
#include "linearalg.h"

static int __set_int_cmp(const void *a, const void *b) {
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

// Get the index of a value inside the Set
static size_t __set_val_index(Set* set, int val) {
    if (set->sorted) { // If it is sorted, do a binary search
        int* item = (int*) bsearch(&val, set->data, set->size, sizeof(int), __set_int_cmp);
        if (item) {
            return (size_t)(item - set->data);
        }
        return (size_t)-1;
    }

    // If not, do a bitch-ass simple search 
    for (size_t i = 0; i < set->size; i++) {
        if (set->data[i] == val) {
            return i;
        }
    }

    return (size_t)-1; 
}

// Expand the array to fit new items
static void __set_expd_arr(Set* set) {
    size_t newSize = set->capacity == 0 ? 4 : set->capacity + (set->capacity / 2);
    int* newPtr = (int*) realloc(set->data, newSize * sizeof(int));

    if (!newPtr) {
        perror("ERROR: Realloc did not return a valid memory address in Set expand array");
        return;
    }

    set->data = newPtr;
    set->capacity = newSize;
}

static size_t __set_add_val(Set* set, int val) {
    if (set->size + 1 > set->capacity) {
        __set_expd_arr(set);
    }

    size_t idx = set->size;
    set->data[idx] = val;
    set->size += 1;

    // Se o elemento não for maior que o último, o conjunto perde a ordenação
    if (set->size > 1 && set->sorted && set->data[idx - 1] > val) {
        set->sorted = 0;
    }

    return idx;
}

Set* setNewFull(int* data, size_t capacity, size_t size, int sorted) {
    Set* set = (Set*) malloc(sizeof(Set));
    if (!set) return NULL;
    
    set->data = data; 
    set->capacity = capacity; 
    set->size = size;
    set->sorted = sorted; 

    return set; 
}

// Create a set with given capacity
Set* setNew(size_t capacity) {
    Set* set = (Set*) malloc(sizeof(Set));
    if (!set) return NULL;

    set->data = capacity > 0 ? (int*) malloc(sizeof(int) * capacity) : NULL;
    set->capacity = capacity; 
    set->size = 0;
    set->sorted = 1; 

    return set; 
}

void setFree(Set* set) {
    if (!set) return;
    free(set->data); 
    free(set);
}

// Adds the value to the set and returns its index
size_t setAdd(Set* set, int val) {
    size_t idx = __set_val_index(set, val);
    if (idx == (size_t)-1) {
        return __set_add_val(set, val);
    }

    return idx; 
}

void setAddArray(Set* set, int* arr, size_t size) {
    for (size_t i = 0; i < size; i++) {
        setAdd(set, arr[i]);
    }
    setSort(set);
}

void setSort(Set* set) { 
    if (!set || set->sorted)
        return;

    qsort(set->data, set->size, sizeof(int), __set_int_cmp);
    set->sorted = 1; 
}

static int checkSets(Set* setA, Set* setB) {
    if (!setA || !setB) return 0;

    if (!setA->sorted) {
        qsort(setA->data, setA->size, sizeof(int), __set_int_cmp);
        setA->sorted = 1;
    }

    if (!setB->sorted) {
        qsort(setB->data, setB->size, sizeof(int), __set_int_cmp);
        setB->sorted = 1;
    }

    return 1; 
}

Set* setIntersection(Set* setA, Set* setB) {
    if (!checkSets(setA, setB))
        return NULL; 

    size_t maxSize = (setA->size < setB->size) ? setA->size : setB->size;
    if (maxSize == 0) {
        return setNewFull(NULL, 0, 0, 1);
    }

    int* newData = ALLOC_ARRAY(newData, maxSize);
    if (!newData) return NULL;

    // Two pointers algo O(N + M)
    size_t i = 0, j = 0, pointer = 0;
    while (i < setA->size && j < setB->size) {
        if (setA->data[i] == setB->data[j]) {
            newData[pointer++] = setA->data[i];
            i++;
            j++;
        } else if (setA->data[i] < setB->data[j]) {
            i++;
        } else {
            j++;
        }
    }

    if (pointer == 0) {
        free(newData);
        newData = NULL;
    } else if (pointer < maxSize) {
        int* temp = (int*)realloc(newData, pointer * sizeof(int));
        if (temp) {
            newData = temp;
        }
    }

    return setNewFull(newData, pointer, pointer, 1);
}

Set* setUnion(Set* setA, Set* setB) {
    if (!checkSets(setA, setB))
        return NULL;

    size_t maxSize = setA->size + setB->size;
    if (maxSize == 0) {
        return setNewFull(NULL, 0, 0, 1);
    }

    int* newData = ALLOC_ARRAY(newData, maxSize);
    if (!newData) return NULL;

    size_t i = 0, j = 0, pointer = 0;

    // Two pointers algo O(N + M)
    while (i < setA->size && j < setB->size) {
        if (setA->data[i] < setB->data[j]) {
            newData[pointer++] = setA->data[i++];
        } else if (setB->data[j] < setA->data[i]) {
            newData[pointer++] = setB->data[j++];
        } else { // Igual em ambos
            newData[pointer++] = setA->data[i];
            i++;
            j++;
        }
    }

    while (i < setA->size) newData[pointer++] = setA->data[i++];
    while (j < setB->size) newData[pointer++] = setB->data[j++];

    if (pointer == 0) {
        free(newData);
        newData = NULL;
    } else if (pointer < maxSize) {
        int* temp = (int*)realloc(newData, pointer * sizeof(int));
        if (temp) {
            newData = temp;
        }
    }

    return setNewFull(newData, pointer, pointer, 1);
}

Set* setDifference(Set* setA, Set* setB) {
    if (!checkSets(setA, setB))
        return NULL; 

    if (setA->size == 0) {
        return setNewFull(NULL, 0, 0, 1);
    }

    int* newData = ALLOC_ARRAY(newData, setA->size);
    if (!newData) return NULL;

    size_t i = 0, j = 0, pointer = 0;
    while (i < setA->size && j < setB->size) {
        if (setA->data[i] < setB->data[j]) {
            newData[pointer++] = setA->data[i++];
        } else if (setA->data[i] == setB->data[j]) {
            i++;
            j++;
        } else {
            j++;
        }
    }
    
    while (i < setA->size) {
        newData[pointer++] = setA->data[i++];
    }

    if (pointer == 0) {
        free(newData);
        newData = NULL;
    } else if (pointer < setA->size) {
        int* temp = (int*)realloc(newData, pointer * sizeof(int));
        if (temp) {
            newData = temp;
        }
    }

    return setNewFull(newData, pointer, pointer, 1);
}

Set* setSimmDifference(Set* setA, Set* setB) {
    if (!checkSets(setA, setB)) 
        return NULL; 

    size_t maxSize = setA->size + setB->size;
    if (maxSize == 0) {
        return setNewFull(NULL, 0, 0, 1);
    }

    int* newData = ALLOC_ARRAY(newData, maxSize);
    if (!newData) return NULL;

    size_t i = 0, j = 0, pointer = 0;
    while (i < setA->size && j < setB->size) {
        if (setA->data[i] < setB->data[j]) {
            newData[pointer++] = setA->data[i++];
        } else if (setB->data[j] < setA->data[i]) {
            newData[pointer++] = setB->data[j++];
        } else {
            i++;
            j++;
        }
    }

    while (i < setA->size) newData[pointer++] = setA->data[i++];
    while (j < setB->size) newData[pointer++] = setB->data[j++];

    if (pointer == 0) {
        free(newData);
        newData = NULL;
    } else if (pointer < maxSize) {
        int* temp = (int*)realloc(newData, pointer * sizeof(int));
        if (temp) {
            newData = temp;
        }
    }

    return setNewFull(newData, pointer, pointer, 1);
}
#ifndef SET_H
#define SET_H

#include <stddef.h>

typedef struct {
    int *data;
    size_t capacity;
    size_t size;
    int sorted;
} Set;

// --- Constructors and Destructor ---

/**
 * Creates a new empty set allocating initial capacity.
 */
Set* setNew(size_t capacity);

/**
 * Instantiates a Set struct associating an existing data pointer.
 */
Set* setNewFull(int* data, size_t capacity, size_t size, int sorted);

/**
 * Frees the internal integer array and the Set structure.
 */
void setFree(Set* set);

// --- Modifiers and Utilities ---

/**
 * Adds a value to the set (if it does not already exist) and returns its index.
 */
size_t setAdd(Set* set, int val);

/**
 * Adds multiple elements and ensures the set remains sorted.
 */
void setAddArray(Set* set, int* arr, size_t size);

/**
 * Sorts the set in-place via qsort (if it is not already sorted).
 */
void setSort(Set* set);

// --- Set Operations (Return a NEW Set) ---

/**
 * Returns the intersection between sets (A ∩ B) 
 */
Set* setIntersection(Set* setA, Set* setB);

/**
 * Returns the union (A ∪ B) in O(N + M).
 */
Set* setUnion(Set* setA, Set* setB);

/**
 * Returns the relative difference between sets (A - B) 
 */
Set* setDifference(Set* setA, Set* setB);

/**
 * Returns the symmetric difference between sets(A Δ B) 
 */
Set* setSimmDifference(Set* setA, Set* setB);

#endif // SET_H
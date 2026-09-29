#ifndef STRING_SET_H
#define STRING_SET_H

#include <stddef.h>

typedef struct {
    char **data;
    size_t capacity;
    size_t size;
    int sorted;
} SetString;

// --- Constructors and Destructor ---

/**
 * Creates a new empty string set with the specified capacity.
 */
SetString* setStringNew(size_t capacity);

/**
 * Instantiates the SetString structure using an existing char** pointer.
 */
SetString* setStringNewFull(char** data, size_t capacity, size_t size, int sorted);

/**
 * Frees each stored string individually and the SetString structure.
 */
void setStringFree(SetString* set);

// --- Modifiers and Utilities ---

/**
 * Adds a copy of the string to the set (via strdup, if it does not exist).
 */
size_t setStringAdd(SetString* set, const char* val);

/**
 * Adds multiple elements to the set and sorts it lexicographically.
 */
void setStringAddArray(SetString* set, const char** arr, size_t size);

/**
 * Sorts the strings lexicographically via qsort (if not already sorted).
 */
void setStringSort(SetString* set);

// --- Set Operations (Return a NEW SetString) ---

/**
 * Returns the intersection (A ∩ B) in O(N + M).
 */
SetString* setStringIntersection(SetString* setA, SetString* setB);

/**
 * Returns the union (A ∪ B) in O(N + M).
 */
SetString* setStringUnion(SetString* setA, SetString* setB);

/**
 * Returns the relative difference (A - B) in O(N + M).
 */
SetString* setStringDifference(SetString* setA, SetString* setB);

/**
 * Returns the symmetric difference (A Δ B) in O(N + M).
 */
SetString* setSimmStringDifference(SetString* setA, SetString* setB);

#endif // SET_STRING_H8
#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>

// Overflow-safe size_t multiplication
size_t st_mult(size_t a, size_t b);

// #define ALLOC_ARRAY(x, alloc) (x) = malloc(st_mult(sizeof(*(x)), (alloc)))
#define ALLOC_ARRAY(x, alloc) malloc(st_mult(sizeof(*(x)), alloc))
#define max2(x, y) (x > y ? x : y)


#endif
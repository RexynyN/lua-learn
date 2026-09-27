#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


#include "utils.h"

size_t st_mult(size_t a, size_t b) {
    if (a > ULONG_MAX / b) {
        printf("ERROR: Overflow trying to multiply size_t's: %zu * %zu", a, b);
        exit(1);
    }

    return a * b;
}

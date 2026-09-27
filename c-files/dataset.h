#ifndef DATASET_H
#define DATASET_H

#include "linearalg.h"

typedef struct Dataset {
    Matrix features; 
    Vector targets; 
} Dataset;

typedef struct CSVFile {
    char* filename; 
    size_t lineCount;
    size_t rows; 
    size_t columns; 
    char separator;
    int hasHeader;
} CSVFile; 


#endif
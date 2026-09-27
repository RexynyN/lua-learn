#ifndef KNN_H
#define KNN_H

#include "linearalg.h"

typedef double (*KNNDistanceFunc) (Vector*, Vector*);

enum KNNDistance {
    KNN_DIST_EUCLID = 0, 
    KNN_DIST_MANHATTAN, 
    KNN_DIST_MINKOWSKI, 
    KNN_DIST_COSINE, 
    KNN_DIST_HAMMING
};

typedef struct KNN {
    Dataset data; 
    int kNeighbors; 
    KNNDistanceFunc distance; 
} KNN;

KNN createKNNModel(Dataset data, int kNeighbors, enum KNNDistance dist);
double knnClassify(KNN* model, Vector* newPoint);

#endif

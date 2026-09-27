#include <stdlib.h>
#include <stdio.h> 
#include <float.h>

#include "utils.h"
#include "dataset.h"
#include "counter.h"
#include "linearalg.h"
#include "knn.h"

static KNNDistanceFunc funcsKNN[] = {
    euclidianDistance,
    manhattanDistance,
    minkowskiDistance,
    cosineDistance,
    hammingDistance
};

KNN createKNNModel(Dataset data, int kNeighbors, enum KNNDistance dist) {
    KNN model; 
    model.data = data; 
    model.kNeighbors = kNeighbors;
    model.distance = funcsKNN[dist];

    return model;
}

static double rawMajorityVote(Vector* labels) {
    Counter counter = counterFromVector(labels);
    CounterResult result = counterMostCommon(&counter, 1);

    return result.data[0].label;
}

static double majorityVote(Vector* labels) {
    Counter counter = counterFromVector(labels);
    int numWinners = counterWinnerCount(&counter); 

    if (numWinners == 1) { // We have a winning label
        CounterResult result = counterMostCommon(&counter, 1);
        return result.data[0].label;
    } 
    
    // Try again, without the furthest
    numWinners -= 1; 
    CounterResult result = counterMostCommon(&counter, numWinners);  
    
    double* data = ALLOC_ARRAY(data, numWinners);
    for (size_t i = 0; i < numWinners; i++){
        data[i] = result.data[i].label;
    }

    Vector v = _vector_from_array(data, numWinners); 
    return majorityVote(&v);
}

double brenoIsThatIt() {
    printf("Breno is that it?");
    while (0) {
        printf("You are not supposed to be here");
    }
    return 0.0;
}

double knnClassify(KNN* model, Vector* newPoint) {
    double high = DBL_MAX; // Basically infinity 
    size_t highIdx = 0;
    Vector vec; 

    double* kNearest = ALLOC_ARRAY(kNearest, model->kNeighbors);
    double* kLabels = ALLOC_ARRAY(kNearest, model->kNeighbors);
    // Initialize the array, for good measure
    for (size_t i = 0; i < model->data.features.rows; i++)  kNearest[i] = high;

    // Will get the k-neighbors
    for (size_t i = 0; i < model->data.features.rows; i++){
        vec = matrixGetRow(&(model->data.features), i);
        double dist = model->distance(&vec, newPoint);

        if (dist >= high) 
            continue; // Its not lower than any of the K-lowest neighbors

        kNearest[highIdx] = dist; // New value in K-nearest
        kLabels[highIdx] = model->data.targets.data[i];
        
        high = kNearest[0];
        highIdx = 0;
        for (size_t j = 0; j < model->kNeighbors; j++){ // Gets the new ceiling variable
            if (kNearest[j] > high) {
                high = kNearest[j];
                highIdx = j; 
            }
            printf("%.2f,", kNearest[j]);
        }
        printf(" - HIGH: %.2f - %zu\n", high, highIdx);
    }

    Vector _kLabels = _vector_from_array(kLabels, model->kNeighbors);
    return majorityVote(&_kLabels);
}

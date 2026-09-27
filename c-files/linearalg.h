#ifndef LINEARALG_H
#define LINEARALG_H

#include <stdlib.h>
#include <math.h>

typedef struct Vector {
    double* data;
    size_t len;
} Vector;

typedef struct Matrix {
    double** data;
    size_t rows;
    size_t columns;
} Matrix;


// ============================= FUNÇÕES AUXILIARES =============================
Vector matrixGetRow(Matrix* mat, size_t row);
Vector matrixGetColumn(Matrix* mat, size_t col);
Vector _vector_from_array(double* data, size_t len);
Matrix _matrix_from_vectors(Vector* vecs, size_t len, size_t vecLen);
Matrix _matrix_from_double(double** doubles, size_t len, size_t vecLen);
int _validate_matrix_vectors(Vector* vecs, size_t len);
int _validate_matrix_data(double** data, size_t rows, size_t columns);

// ============================= MACROS =============================
#define createVector(arr) _vector_from_array(arr, sizeof(arr) / sizeof(arr[0]))
#define createMatrix(vec) _matrix_from_double(vec, sizeof(vec) / sizeof(vec[0]), sizeof(vec[0]) / sizeof(vec[0][0])) 
#define createMatrixVector(vec) (_validate_matrix_vectors(vec, sizeof(vec) / sizeof(vec[0])) ? _matrix_from_vectors(vec, sizeof(vec) / sizeof(vec[0]), (vec[0]).len) : (Matrix){0})
#define POW2(x) x * x

// ============================= OPERAÇÕES DE VETOR =============================
int sameVectorLen(Vector* v, Vector* w);
void print_vector(Vector v);
void print_matrix(Matrix m);
Vector addVector(Vector* v, Vector* w);
Vector subtractVector(Vector* v, Vector* w);
Vector scalarMultiplyVector(double scalar, Vector* v);
double dotProduct(Vector* v, Vector* w);
double sumOfSquares(Vector* v);
double vectorMagnitude(Vector* v);
double squaredDistance(Vector* v, Vector* w);



double euclidianDistance(Vector* v, Vector* w);
double manhattanDistance(Vector* v, Vector* w);
double minkowskiDistance(Vector* v, Vector* w);
double cosineDistance(Vector* v, Vector* w);
double hammingDistance(Vector* v, Vector* w);


// Nova função para liberar memória
void freeVector(Vector* v);

// ============================= OPERAÇÕES DE MATRIZ =============================
Vector vectorSum(Matrix* m);
Vector vectorMean(Matrix* m);
Vector getRow(Matrix* m, int i);
Vector getColumn(Matrix* m, int j);
Matrix identityMatrix(long int n);

// Nova função para liberar memória
void freeMatrix(Matrix* m);

#endif

#include "linearalg.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Vector _vector_from_array(double* data, size_t len) {
    Vector v;
    v.len = len;
    v.data = (double*) malloc(sizeof(double) * len);

    for (size_t i = 0; i < len; i++) {
        v.data[i] = data[i];
    }
    
    return v;

}

Matrix _matrix_from_vectors(Vector* vecs, size_t len, size_t vecLen) {
    double** mat = (double**) malloc(sizeof(double*) * len);
    for(size_t i = 0; i < len; i++) {
        mat[i] = (double*) malloc(sizeof(double) * vecLen);
        for(size_t j = 0; j < vecLen; j++) {
            mat[i][j] = vecs[i].data[j]; 
        }
    }

    Matrix v; 
    v.data = mat; 
    v.rows = len; 
    v.columns = vecLen; 
    return v;
}

Matrix _matrix_from_double(double** doubles, size_t len, size_t vecLen) {
    // Make a deep copy
    double** mat = (double**) malloc(sizeof(double*) * len);
    for(size_t i = 0; i < len; i++) {
        mat[i] = (double*) malloc(sizeof(double) * vecLen);
        for(size_t j = 0; j < vecLen; j++) {
            mat[i][j] = doubles[i][j]; 
        }
    }

    Matrix v; 
    v.data = mat; 
    v.rows = len; 
    v.columns = vecLen;
    return v;
}

int _validate_matrix_data(double** data, size_t rows, size_t columns) {
    if (rows == 0) return 1;
    // Nota: Validar NULL em double** vindo de array estático é complexo, 
    // mas mantive sua lógica original para arrays dinâmicos.
    if (data == NULL) return 0;
    
    return 1;
}

int _validate_matrix_vectors(Vector* vecs, size_t len) {
    if (len == 0) return 1;
    size_t expected_len = vecs[0].len;
    for (size_t i = 1; i < len; i++) {
        if (vecs[i].len != expected_len) {
            printf("ERRO: Vetores com tamanhos diferentes na matriz.\n");
            return 0;
        }
    }
    return 1;
}


Vector matrixGetRow(Matrix* mat, size_t row) {
    double* new_data = ALLOC_ARRAY(new_data, mat->columns);
    for (size_t i = 0; i < mat->columns; i++){
        new_data[i] = mat->data[row][i];
    }
    
    return _vector_from_array(new_data, mat->columns);
}

Vector matrixGetColumn(Matrix* mat, size_t col) {
    double* new_data = ALLOC_ARRAY(new_data, mat->rows);
    for (size_t i = 0; i < mat->rows; i++){
        new_data[i] = mat->data[i][col];
    }
    
    return _vector_from_array(new_data, mat->rows);
}


#define createVector(arr) _vector_from_array(arr, sizeof(arr) / sizeof(arr[0]))
#define createMatrix(vec) _matrix_from_double(vec, sizeof(vec) / sizeof(vec[0]), sizeof(vec[0]) / sizeof(vec[0][0])) 
#define createMatrixVector(vec) (_validate_matrix_vectors(vec, sizeof(vec) / sizeof(vec[0])) ? _matrix_from_vectors(vec, sizeof(vec) / sizeof(vec[0]), (vec[0]).len) : (Matrix){0})


void freeVector(Vector* v) {
    // Só damos free se o ponteiro não for NULL
    if (v->data != NULL) {
        free(v->data);
        v->data = NULL; // Boa prática: evitar "dangling pointer"
        v->len = 0;
    }
}

void freeMatrix(Matrix* m) {
    if (m->data != NULL) {
        for (size_t i = 0; i < m->rows; i++) {
            if (m->data[i] != NULL) {
                free(m->data[i]);
            }
        }
        free(m->data);
        m->data = NULL;
        m->rows = 0;
        m->columns = 0;
    }
}

int sameVectorLen(Vector* v, Vector* w) {
    return v->len == w->len;
}

void print_vector(Vector v) {
    if (v.data == NULL) {
        printf("Vetor Vazio/Invalido\n");
        return;
    }
    printf("[ ");
    for (size_t i = 0; i < v.len; i++) {
        printf("%.1f, ", v.data[i]);
    }
    printf("]\n");
}

void print_matrix(Matrix m) {
    if (m.data == NULL) {
        printf("Matriz Vazia/Invalida\n");
        return;
    }
    for (size_t i = 0; i < m.rows; i++) {
        printf("[ ");
        for (size_t j = 0; j < m.columns; j++) {
            printf("%.1f ", m.data[i][j]);
        }
        printf("]\n");
    }
}

Vector addVector(Vector* v, Vector*  w) {
    if (!sameVectorLen(v, w)) {
        printf("ERRO: Vetores de tamanhos diferentes.\n");
        return (Vector){0};
    }

    double new_data[v->len];
    for(size_t i = 0; i < v->len; i++) {
        new_data[i] = v->data[i] + w->data[i];
    }

    return _vector_from_array(new_data, v->len);
}

Vector subtractVector(Vector* v, Vector* w) {
    if (!sameVectorLen(v, w)) {
        printf("ERRO: Vetores de tamanhos diferentes.\n");
        return (Vector){0};
    }

    double new_data[v->len];
    for(size_t i = 0; i < v->len; i++) {
        new_data[i] = v->data[i] - w->data[i];
    }

    return _vector_from_array(new_data, v->len);
}


Vector scalarMultiplyVector(double scalar, Vector* v) {
    double new_data[v->len];
    for(size_t i = 0; i < v->len; i++) {
        new_data[i] = scalar * v->data[i];
    }

    return _vector_from_array(new_data, v->len);
}

Vector vectorSum(Matrix* m) {
    double sum = 0; 
    double new_data[m->columns];
    for(size_t i = 0; i < m->columns; i++) {
        for(size_t j = 0; j < m->rows; j++) {
            sum += m->data[j][i];
        }

        new_data[i] = sum;
        sum = 0;
    }

    return _vector_from_array(new_data, m->columns);
}

Vector vectorMean(Matrix* m) {
    size_t n = m->rows; 
    Vector summed = vectorSum(m);

    // R = [1/len * v[0], 1/len * v[1], ... 1/len * v[i]]
    // R being the mean vector of the bunch
    Vector mean = scalarMultiplyVector(1.0/n, &summed);
    freeVector(&summed);
    return mean; 
}

double dotProduct(Vector* v, Vector* w) {
    if (!sameVectorLen(v, w)) {
        printf("ERROR (dotProduct): Different size Vectors\n");
        return NAN;
    }   

    double dotProd = 0; 
    for(size_t i = 0; i < v->len; i++) {
        dotProd += v->data[i] * w->data[i];
    }

    return dotProd;
}

double sumOfSquares(Vector* v) {
    return dotProduct(v, v);
}

//  Return the length (or magnitude) of an vector
double vectorMagnitude(Vector* v) {
    return sqrt(sumOfSquares(v));
}

double squaredDistance(Vector* v, Vector* w) {
    Vector subtracted = subtractVector(v, w);
    return sumOfSquares(&subtracted);
}

double euclidianDistance(Vector* v, Vector* w) {
    return sqrt(squaredDistance(v, w));
}

double manhattanDistance(Vector* v, Vector* w) {
    if (!sameVectorLen(v, w)) {
        printf("ERROR (manhattanDistance): Different size Vectors\n");
        return NAN;
    }   

    double distance = 0.0; 
    for(size_t i = 0; i < v->len; i++) {
        distance += fabs(v->data[i] - v->data[i]);
    }

    return distance;
}

double minkowskiDistance(Vector* v, Vector* w) {
    if (!sameVectorLen(v, w)) {
        printf("ERROR (manhattanDistance): Different size Vectors\n");
        return NAN;
    }   

    double maxDiff = 0.0, diff = 0.0; 
    for(size_t i = 0; i < v->len; i++) {
        diff = fabs(v->data[i] - v->data[i]);

        if(diff > maxDiff) {
            maxDiff == diff;
        }
    }

    return maxDiff;
}

double cosineDistance(Vector* v, Vector* w) {

}

// Uses the integer value of the doubles
double hammingDistance(Vector* v, Vector* w) {
        if (!sameVectorLen(v, w)) {
        printf("ERROR (manhattanDistance): Different size Vectors\n");
        return NAN;
    }   

    double mismatch = 0.0; 
    for(size_t i = 0; i < v->len; i++) {
        if( ((int)v->data[i]) != ((int)w->data[i])) {
            mismatch++;
        }
    }

    return mismatch;
}



// ============================= MATRIX FUNCTIONS =============================


// size_t* matrixShape(Matrix* m) {
//     return (size_t*) { m->rows, m->columns };
// }

Vector getRow(Matrix* m, int i) {
    return _vector_from_array(m->data[i], m->columns);
}

Vector getColumn(Matrix* m, int j) {
    double* col = (double*) malloc(sizeof(double) * m->rows);
    for(size_t i = 0; i < m->rows; i++) {
        col[i] = m->data[i][j];
    }

    return _vector_from_array(col, m->rows);
}

Matrix identityMatrix(long int n) {
    double** mat = (double**) malloc(sizeof(double) * (n * n));
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++) {
            mat[i][j] = i == j ? 1 : 0; 
        }
    }

    return _matrix_from_double(mat, n, n);
}




// int main() {
//     double a_data[] = { 1.0, 2.0, 3.0 };
//     Vector a = createVector(a_data);

//     double b_data[] = { 1.0, 2.0, 3.0 };
//     Vector b = createVector(b_data);


//     printf("--- Teste 1 ---\n");
//     Vector vectors1[] = { a, b };
//     Matrix m1 = createMatrixVector(vectors1);
//     Vector v = vectorSum(&m1);

//     double c_data[] = { 1.0, 2.0, 3.0 };
//     Vector c = createVector(c_data);
    
//     double d_data[] = { 1.0, 2.0, 3.0, 4.0, 5.0 };
//     Vector d = createVector(d_data);

//     printf("\n--- Teste 2 ---\n");
//     Vector vectors2[] = { c, d };
//     Matrix m2 = createMatrixVector(vectors2);
//     vectorSum(&m2);

//     print_vector(v);


//     double dot = dotProduct(&a, &b);
//     printf("\n%.1f\n", dot);

//     dot = sumOfSquares(&a);
//     printf("%.1f\n", dot);


//     Vector vm = getColumn(&m1, 1);
//     print_vector(vm);

//     return 0;

// }
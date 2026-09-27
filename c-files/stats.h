#ifndef STATS_H
#define STATS_H

#include "linearalg.h" // Necessário para reconhecer o tipo Vector
#include <math.h>

const double PI = 3.14159265358979323846;
// sqrt(2 * PI);
const double SQRT_TWO_PI = 2.5066282746310002;


double* createDoubleArray(size_t len); 

// ============================= TENDÊNCIA CENTRAL =============================
double min(Vector* v);
double max(Vector* v);
double mean(Vector* v);
double median(Vector* v);
Vector mode(Vector* v); // Retorna um Vector pois pode haver multimodalidade
double quantile(Vector* v, double p); // p deve ser entre 0.0 e 1.0

// ============================= DISPERSÃO =============================
double dataRange(Vector* v);
Vector deMean(Vector* v); // Retorna um novo Vector com os desvios da média
double variance(Vector* v);
double standardDeviation(Vector* v);
double interquantileRange(Vector* v);

// ============================= RELAÇÃO ENTRE VARIÁVEIS =============================
double covariance(Vector* xs, Vector* ys);
double correlation(Vector* xs, Vector* ys);

#endif
#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>

void inversion
        (
        double **A, int N
        );

void transpon
        (
        double **A, int N
        );

void MatMatMul
        (
        double **In1, double **In2,
        double **Out,
        int N
        );

void MatVecMul
        (
        double **InMat, double *InVec,
        double *Out,
        int N
        );

void VecMatMul
        (
        double *InVec, double **InMat,
        double *Out,
        int N
        );

double VecVecMul
        (
        double *In1, double *In2, int N
        );

double Det
        (
        double **A, int N
        );

void MatSum
        (
        double **In1, double **In2,
        double **Out,
        int N
        );

void MatSclMul
        (
        double **A, double B, int N
        );

#endif // MATRIX_H

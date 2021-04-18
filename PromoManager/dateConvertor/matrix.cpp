#include "matrix.h"

void inversion(double **A, int N)
{
    double temp;

    double **E = new double *[N];

    for (int i = 0; i < N; i++)
        E[i] = new double [N];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            E[i][j] = 0.0;

            if (i == j)
                E[i][j] = 1.0;
        }

    for (int k = 0; k < N; k++)
    {
        temp = A[k][k];

        for (int j = 0; j < N; j++)
        {
            A[k][j] /= temp;
            E[k][j] /= temp;
        }

        for (int i = k + 1; i < N; i++)
        {
            temp = A[i][k];

            for (int j = 0; j < N; j++)
            {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }

    for (int k = N - 1; k > 0; k--)
    {
        for (int i = k - 1; i >= 0; i--)
        {
            temp = A[i][k];

            for (int j = 0; j < N; j++)
            {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = E[i][j];

    for (int i = 0; i < N; i++)
        delete [] E[i];

    delete [] E;
}

void transpon(double **A, int N)
{
    double **E = new double *[N];

    for (int i = 0; i < N; i++)
        E[i] = new double [N];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = E[j][i];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = E[i][j];

    for (int i = 0; i < N; i++)
        delete [] E[i];

    delete [] E;
}

void MatMatMul(double **In1, double **In2, double **Out, int N)
{
    int i, j, k;
    double s;

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            {
            s = 0;
            for (k = 0; k < N; k++)
                s += In1[i][k] * In2[k][j];
            Out[i][j] = s;
            }
}

void MatVecMul(double **InMat, double *InVec, double *Out, int N)
{
    int i, j;
    double s;

    for (i = 0; i < N; i++)
    {
        s = 0;
        for (j = 0; j < N; j++)
            s += InMat[i][j] * InVec[j];
        Out[i] = s;
    }
}

void VecMatMul(double *InVec, double **InMat, double *Out, int N)
{
    int i, j;
    double s;

    for (i = 0; i < N; i++)
    {
        s = 0;
        for (j = 0; j < N; j++)
            s += InVec[j] * InMat[j][i];
        Out[i] = s;
    }
}

double VecVecMul(double *In1, double *In2, int N)
{
    int i;
    double s;

    s = 0;
    for (i = 0; i < N; i++)
        s += In1[i] * In2[i];
    return (s);
}

double Det(double **A, int N)
{
    double det = 0;
    int i, j;
    double s1, s2;

    for (i = 0; i < N; i++)
    {
        s1 = 1;
        s2 = 1;
        for (j = 0; j < N; j++)
        {
            s1 *= A[j][(j + i >= N) ? j + i - N : j + i];
            s2 *= A[N - j - 1][(i + j >= N) ? i + j - N : i + j];
        };
       det += s1 - s2;
    };
    return(det);
}

void MatSum(double **In1, double **In2, double **Out, int N)
{
    int i, j;

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            Out[i][j] = In1[i][j] + In2[i][j];
}

void MatSclMul(double **A, double B, int N)
{
    int i, j;

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            A[i][j] *= B;
}

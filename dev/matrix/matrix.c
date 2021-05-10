#include <stdlib.h>
#include <string.h>

#include "matrix.h"

static matrix_t* Create(unsigned rows, unsigned cols, double* vals) 
{
    matrix_t* this = malloc(sizeof(*this) + (sizeof(double) * rows * cols));
    this->_rows = rows;
    this->_cols = cols;

    if (vals == NULL)
    {
        memset(this->_at, 0, (sizeof(double) * rows * cols));
    }
    else
    {
        memmove(this->_at, vals, (sizeof(double) * rows * cols));
    }


    return this;
}

static void Free(matrix_t* this)
{
    free(this);
}

static matrix_t* Add(matrix_t* a, matrix_t* b, matrix_t* dest)
{
    if (dest == NULL) dest = a;
    if (a->_cols != b->_cols || a->_rows != b->_rows) return;

    for (size_t i = 0; i < dest->_rows; i++)
    {
        for (size_t j = 0; j < dest->_cols; j++)
        {
            const unsigned index = i * dest->_cols + j;
            dest->_at[index] = a->_at[index] + b->_at[index];
        }
    }

    return dest;
}

static double WSum(matrix_t* a, unsigned row, matrix_t* b, unsigned col)
{
    double sum = 0.0;

    for (size_t i = 0; i < a->_cols; i++)
    {
        for (size_t j = 0; j < b->_rows; j++)
        {
            sum += (a->_at[row * a->_cols + i] * b->_at[col * b->_rows + j]);
        }        
    }

    return sum;
}

static matrix_t* Multiply(matrix_t* a, matrix_t* b, matrix_t* dest)
{    
    if (a->_cols != b->_rows) return NULL;

    if (dest != NULL)
    {
        if (dest->_rows != a->_rows || dest->_cols != b->_cols) return NULL;
    }
    else
    {
        dest = Create(a->_rows, b->_cols, NULL);
        if (dest == NULL) return NULL;
    }

    for (size_t row = 0; row < a->_rows; row++)
    {
        for (size_t col = 0; col < b->_cols; col++)
        {
            dest->_at[row * b->_cols + col] = WSum(a, row, b, col);
        }
    }

    return dest;
}

static matrix_t* Transpose(matrix_t* this)
{

    matrix_t* t = Create(this->_cols, this->_rows, NULL);

    // t[i, j] = this[j, i]

    for (size_t i = 0; i < t->_rows; i++)
    {
        for (size_t j = 0; j < t->_cols; j++)
        {
            unsigned tindex = i * t->_cols + j;
            unsigned thisindex = j * this->_cols + i;

            t->_at[tindex] = this->_at[thisindex];
        }
    }

    return t;
}

static void ScalarMultiply(matrix_t* this, double val)
{

    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j < this->_cols; j++)
        {
            this->_at[i * this->_cols + j] *= val;
        }
    }
}

static void ScalarAdd(matrix_t* this, double val)
{

    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j < this->_cols; j++)
        {
            this->_at[i * this->_cols + j] += val;
        }
    }
}

static matrix_t* Set(matrix_t* this, double(*fn)(double), matrix_t* dest)
{
    matrix_t* res = dest == NULL ? this : dest;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j < this->_cols; j++)
        {
            res->_at[i * this->_cols + j] = fn(this->_at[i * this->_cols + j]);
        }
    }

    return res;
}

static void ElemMultiply(matrix_t* a, matrix_t* b)
{
    if (a->_cols != b->_cols || a->_rows != b->_rows) return;

    for (size_t i = 0; i < a->_rows; i++)
    {
        for (size_t j = 0; j < a->_cols; j++)
        {
            size_t index = i * a->_cols + j;
            a->_at[index] *= b->_at[index];
        }
    }
}

static int Eq(const matrix_t* a, const matrix_t* b)
{
    return a->_cols == b->_cols && 
        a->_rows == b->_rows && 
        memcmp(a->_at, b->_at, sizeof(double) * a->_cols * a->_rows) == 0;
}

static void Print(const matrix_t* this, int(*p)(const char*, ...))
{
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j < this->_cols; j++)
        {
            p("%lf, ", this->_at[i * this->_cols + j]);
        }
        p("\n");
    }
    p("\n\n");
}


const matrix_api_t Matrix = {Create, Free, Multiply, Transpose, ScalarAdd, ScalarMultiply, ElemMultiply, Set, Eq, Print}; 

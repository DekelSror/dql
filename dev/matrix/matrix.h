#ifndef dql_matrix_h
#define dql_matrix_h

typedef struct
{
    unsigned _rows;
    unsigned _cols;
    double _at[];
} matrix_t;

typedef struct 
{
    matrix_t*(*create)(unsigned rows, unsigned cols, double* vals);
    void(*free)(matrix_t*);

    // pass NULL as dest to add in-place to a. return dest || a
    matrix_t*(*add)(matrix_t* a, matrix_t* b, matrix_t* dest);

    // pass NULL as dest for this to create a fitting matrix
    matrix_t*(*multiply)(matrix_t* a, matrix_t* b, matrix_t* dest);

    matrix_t*(*transpose)(matrix_t*);

    void(*scalar_add)(matrix_t*, double);
    void(*scalar_multiply)(matrix_t*, double);

    // a and b must be of same dimanesions
    void(*elem_multiply)(matrix_t* a, matrix_t* b);

    // pass NULL as dest to set in-place
    matrix_t*(*set)(matrix_t* m, double(*fn)(double), matrix_t* dest);
    int(*eq)(const matrix_t* a, const matrix_t* b);

    void(*print)(const matrix_t*, int(*)(const char*, ...));
} matrix_api_t;

extern const matrix_api_t Matrix;

#endif // dql_matrix_h

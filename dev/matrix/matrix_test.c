#include <stdio.h>

#include "matrix.h"


void TestMultiply(void)
{
    double a_vals[] = {1.0, 2.0, 3.0};
    double b_vals[] = {10.0, 20.0};

    matrix_t* a = Matrix.create(3, 1, a_vals);
    matrix_t* b = Matrix.create(1, 2, b_vals);

    Matrix.print(a, printf);
    Matrix.print(b, printf);


    matrix_t* dest = Matrix.create(3, 2, NULL);
    Matrix.print(dest, printf);

    Matrix.multiply(a, b, dest);
    Matrix.print(dest, printf);

    Matrix.free(a);
    Matrix.free(b);
    Matrix.free(dest);

}

int main(void)
{
    double vals[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    matrix_t* original = Matrix.create(2, 3, vals);
    matrix_t* t = Matrix.transpose(original);

    Matrix.print(original, printf);
    Matrix.print(t, printf);

    Matrix.free(original);
    Matrix.free(t);

    return 0;
}

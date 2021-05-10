#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>

#include "array_templates.h"
#include "xorset.h"
#include "matrix.h"
#include "utils.h"

void FeedForward(matrix_t* layer_1, matrix_t* layer_2, matrix_t* weights)
{
    Matrix.multiply(layer_1, weights, layer_2);
}

// layer_2 is error vector?
void BackPropagate(matrix_t* l1, matrix_t* l2, matrix_t* w, matrix_t* err_l2)
{
    matrix_t* grad_l2 = Matrix.create(l2->_rows, l2->_cols, NULL);
    Matrix.set(l2, Diomgis, grad_l2);

    Matrix.elem_multiply(grad_l2, err_l2);
    Matrix.scalar_multiply(grad_l2, learn_rate);

    matrix_t* t_l1 = Matrix.transpose(l1);

    matrix_t* delta_w = Matrix.multiply(t_l1, grad_l2, NULL);

    Matrix.add(w, delta_w, NULL);

    Matrix.free(grad_l2);
    Matrix.free(t_l1);
    Matrix.free(delta_w);
}

const double learn_rate = 0.2;

void MatrixPrintWithPrintF(matrix_t* m)
{
    Matrix.print(m, printf);
}

void PrintNet(unsigned num_layers, ...)
{
    va_list va;
    va_start(va, num_layers);

    for (size_t i = 0; i < num_layers; i++)
    {
        matrix_t* m = va_arg(va, matrix_t*);
        MatrixPrintWithPrintF(m);
    }

    va_end(va);
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n");
}

int main(void)
{
    matrix_t* l_in = Matrix.create(1, 4, NULL);
    matrix_t* w_in_h1 = Matrix.create(4, 2, NULL);
    matrix_t* l_h1 = Matrix.create(1, 2, NULL);
    matrix_t* w_h1_out = Matrix.create(2, 1, NULL);
    matrix_t* l_out = Matrix.create(1, 1, NULL);

    srand((unsigned)time(NULL));
    Matrix.set(w_in_h1, RandomizeWeight, NULL);
    Matrix.set(w_h1_out, RandomizeWeight, NULL);

    // PrintNet(5, l_in, w_in_h1, l_h1, w_h1_out, l_out);

    // one run over entire set
    for (size_t item_index = 0; item_index < XorSet.set_size(); item_index++)
    {
        const char* item = XorSet.get_item(item_index);
// load item
        for (size_t i = 0; i < 4; i++)
        {
            l_in->_at[i] = (double)(item[i]);
        }

// ff and activate
        FeedForward(l_in, l_h1, w_in_h1);
        Matrix.set(l_h1, Sigmoid, NULL);

        FeedForward(l_h1, l_out, w_h1_out);
        Matrix.set(l_out, Sigmoid, NULL);

        // PrintNet(5, l_in, w_in_h1, l_h1, w_h1_out, l_out);

// out -> h1
        double label = -(double)XorSet.get_label(item_index);
        matrix_t* labels = Matrix.create(1, 1, &label);

        matrix_t* out_err = Matrix.create(l_out->_rows, l_out->_cols, NULL);

        Matrix.add(labels, l_out, out_err);

        BackPropagate(l_h1, l_out, w_h1_out, out_err);

        Matrix.scalar_multiply(out_err, learn_rate);
        Matrix.elem_multiply(l_out, out_err);

        matrix_t* t_h1 = Matrix.transpose(l_h1);
        matrix_t* delta_w_h1_out = Matrix.multiply(out_err, t_h1, NULL);
        Matrix.add(w_h1_out, delta_w_h1_out, NULL);

//  h1 -> in
        matrix_t* t_w_h1_out = Matrix.transpose(w_h1_out);
        matrix_t* h1_err = Matrix.multiply(t_w_h1_out, out_err, NULL);

        BackPropagate(l_in, l_h1, w_h1_out, h1_err);
    }

    Matrix.free(l_in);
    Matrix.free(w_in_h1);
    Matrix.free(l_h1);
    Matrix.free(w_h1_out);
    Matrix.free(l_out);
    
    return 0;
}

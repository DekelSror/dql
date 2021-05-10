#include <stdarg.h>
#include <stdlib.h>

#include "matrix.h"
#include "array_templates.h"
#include "utils.h"


typedef struct
{
    matrix_t* _layer;
    matrix_t* _err;
    matrix_t* _weights;
} layer_t;

typedef struct 
{
    double _learn_rate;
    unsigned _num_layers;
    unsigned _defined_layers;
    layer_t _layers[];    
} nnet_t;


#define mcalloc(s) calloc(1, (s))

// Create(784, 10, 1, 800)
static nnet_t* Create(unsigned num_layers)
{
    nnet_t* this = mcalloc(sizeof(*this) + (num_layers * sizeof(layer_t)));

    this->_learn_rate = 0.2;
    this->_num_layers = num_layers;
    this->_defined_layers = 0;

    return this;
}

static int AddLayer(nnet_t* this, unsigned layer_size)
{
    if (this->_defined_layers == this->_num_layers) return 1;

    matrix_t* layer = Matrix.create(1, layer_size, NULL);
    matrix_t* err = Matrix.create(1, layer_size, NULL);
    matrix_t* weights = NULL;
    
    unsigned prev_layer_cols;
    if (this->_defined_layers != 0)
    {
        prev_layer_cols = this->_layers[this->_defined_layers - 1]._layer->_cols;
        weights = Matrix.create(prev_layer_cols, layer_size, NULL);
        Matrix.set(weights, RandomizeWeight, NULL);
    }
    
    this->_layers[this->_defined_layers]._layer = layer;
    this->_layers[this->_defined_layers]._err = err;
    this->_layers[this->_defined_layers]._weights = weights;

    this->_defined_layers++;

    return 0;
} 

static void ff(nnet_t* this, double* input)
{
    // load input
    for (size_t i = 0; i < this->_layers[0]._layer->_cols ; i++)
    {
        this->_layers[0]._layer->_at[i] = input[i];
    }

    // feed forward
    for (size_t i = 0; i < this->_num_layers; i++)
    {
        Matrix.multiply(this->_layers[i]._layer, this->_layers[i]._weights, this->_layers[i]._layer);
        Matrix.set(this->_layers[i]._layer, Sigmoid, NULL);
    }
}


static void FeedForward(matrix_t* layer_1, matrix_t* layer_2, matrix_t* weights)
{
    Matrix.multiply(layer_1, weights, layer_2);
}

static void bp(nnet_t* this, matrix_t* labels)
{
    matrix_t* out_err = Matrix.add(this->_layers[this->_num_layers - 1]._layer, labels, NULL);

    for (size_t i = this->_num_layers; i > 0; i--)
    {
        // set err vector
    }
    

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

// layer_2 is error vector?
static void BackPropagate(matrix_t* l1, matrix_t* l2, matrix_t* w, matrix_t* err_l2)
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
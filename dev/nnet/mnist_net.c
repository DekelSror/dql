#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#include "mnist_accessor.h"
#include "array_templates.h"

static const char* fp_train_images = "mnist/train-images.idx3-ubyte";
static const char* fp_train_labels = "mnist/train-labels.idx1-ubyte";

declare_array(double)

double_arr* layer_in;
double_arr* weight_in_1;
double_arr* layer_h1;
double_arr* weight_1_out;
double_arr* layer_out;


void init(void)
{
    layer_in = doubleArrayCreate(784);
    weight_in_1 = doubleArrayCreate(784*800);
    layer_h1 = doubleArrayCreate(800);
    weight_1_out = doubleArrayCreate(800*10);
    layer_out = doubleArrayCreate(10);
}

void RandomizeWeights(double_arr* layer)
{
    srand((unsigned)time(NULL));

    for (size_t i = 0; i < layer->_size; i++)
    {
        layer->_at[i] = 1.0 / (double)(rand() % 10000 + 1);
    }    
}

double Sigmoid(double val)
{
    return 1 / (1 + exp(-val));
}

void FeedForward(double_arr* layer_1, double_arr* layer_2, double_arr* weights)
{
    for (size_t l2_node = 0; l2_node < layer_2->_size; l2_node++)
    {
        double sum = 0.0;

        for (size_t l1_node = 0; l1_node < layer_1->_size; l1_node++)
        {
            sum += layer_1->_at[l1_node] * weights->_at[l2_node + layer_2->_size * l1_node];
        }

        layer_2->_at[l2_node] = Sigmoid(sum);
    }
}

void BackPropagate(double_arr* layer_1, double_arr* layer_2, double_arr* weights)
{

}


int main(void)
{
    init();
    mnist_accessor_t mset = Mnist.create();

    int status = Mnist.load_images(mset, fp_train_images);
    if (status != 0)
    {
        printf("bad status for ufile %d\n", status);
    }

    status = Mnist.load_labels(mset, fp_train_labels);
    if (status != 0)
    {
        printf("bad status for ufile %d\n", status);
    }

    RandomizeWeights(weight_in_1);
    RandomizeWeights(weight_1_out);

    char* image = Mnist.get_image(mset, 0);

    // load input
    for (size_t i = 0; i < layer_in->_size; i++)
    {
        layer_in->_at[i] = (double)(image[i]) / 255.0;
    }

    FeedForward(layer_in, layer_h1, weight_in_1);
    FeedForward(layer_h1, layer_out, weight_1_out);


    double_arr* out_err = doubleArrayCreate(layer_out->_size);

    char label = Mnist.get_label(mset, 0);



    for (size_t i = 0; i < layer_out->_size; i++)
    {
        printf("prob. for %lu - %lf\n", i, layer_out->_at[i]);
        out_err->_at[i] = 1.0 - layer_out->_at[i];
    }    

    printf("\nlabel %d\n", label);

    const double error = 1.0 - layer_out->_at[(unsigned)label];

    for (size_t h1_node = 0; h1_node < layer_h1->_size; h1_node++)
    {        
        for (size_t out_node = 0; out_node < layer_out->_size; out_node++)
        {
            weight_1_out->_at[]
        }
    }
    


    printf("error is  %lf \n", error);

    Mnist.free(mset);

    free(layer_in);
    free(weight_in_1);
    free(layer_h1);
    free(weight_1_out);
    free(layer_out);

    return 0;
}























#include <math.h>
#include <stdlib.h>

double RandomizeWeight(double val)
{
    (void)val;
    return 1.0 / (double)(rand() % 10000 + 1);
}

// undo sigmoid on val
double Diomgis(double val)
{
    return val * (1 - val);
}

double Sigmoid(double val)
{
    return 1 / (1 + exp(-val));
}

double ReLu(double val)
{
    return val > 0.0 ? val : 0.0;
}

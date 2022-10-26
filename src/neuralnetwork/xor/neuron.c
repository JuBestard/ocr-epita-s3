#include "neuron.h"
#include <stdio.h>
#include <stdlib.h>

double sigmoid(double a)
{
    return 1.0 / (1.0 + exp(-a));
}

double dsigmoid(double a)
{
    return a * (1 - a);
}

double calculateOutput(Neuron* n)
{
    return sigmoid(n->weights[0] * n->inputs[0] + 
            n->weights[1] * n->inputs[1] + n->biasWeight);
}

double rand01()
{
    return ((double)(rand()+1) / (double)(RAND_MAX+1.0));
}

void randWeights(Neuron* n)
{
    n->weights[0] = rand01();
    n->weights[1] = rand01();
    n->biasWeight = rand01();
}

void adjustWeights(Neuron* n)
{
    n->weights[0] += n->error * n->inputs[0];
    n->weights[1] += n->error * n->inputs[1];
    n->biasWeight += n->error;
}

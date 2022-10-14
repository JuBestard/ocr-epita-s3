#ifndef NEURON_H
#define NEURON_H

#include "math.h"
#include <stdlib.h>


typedef struct Neuron
{
    double inputs[2];
    double weights[2];
    double error;

    double biasWeight;
} Neuron;

double sigmoid(double a);
double dsigmoid(double a);

double calculateOutput(Neuron* neuron);

double rand01();

void randWeights(Neuron* neuron);
void adjustWeights(Neuron* neuron);

#endif

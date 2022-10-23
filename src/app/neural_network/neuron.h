#pragma once

typedef struct Neuron
{
    double inputs[2];
    double weights[2];
    double error;
    double biasWeight;
} Neuron;

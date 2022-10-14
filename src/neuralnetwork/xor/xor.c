#include <stdio.h>

#include "xor.h"
#include "neuron.h"

int train_inputs[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
int train_outputs[4] = {0, 1, 1, 0};

void train(int epoch) 
{
    Neuron* hidden1 = NULL;
    Neuron* hidden2 = NULL;
    Neuron* output = NULL;

    for (int j = 0; j < epoch; j++)
    {
        for (int i = 0; i < 4; i++) 
        {
            hidden1->inputs[0]=train_inputs[i][0];
            hidden1->inputs[1]=train_inputs[i][0];

            hidden2->inputs[0]=train_inputs[i][0];
            hidden2->inputs[1]=train_inputs[i][0];

            output->inputs[0] = calculateOutput(hidden1);
            output->inputs[1] = calculateOutput(hidden2);

            double result = calculateOutput(output);

            printf("%f xor %f = %f", hidden1->inputs[0], hidden2->inputs[1],
                    result);

            output->error = dsigmoid(result) * (train_outputs[i] - result);
            adjustWeights(output);

            hidden1->error = dsigmoid(calculateOutput(hidden1)) * output->error*output->weights[0];
            hidden2->error = dsigmoid(calculateOutput(hidden2)) * output->error*output->weights[1];

            adjustWeights(hidden1);
            adjustWeights(hidden2);
       }
    }
}

int main(int argc, char** argv) 
{
    if(argc != 2)
        return EXIT_FAILURE;

    train(atof(argv[1]));
}

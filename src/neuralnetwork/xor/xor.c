#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "neuralnet/xor.h"
#include "neuralnet/neuron.h"

double train_inputs[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
int train_outputs[4] = {0, 1, 1, 0};

void train(int epoch)
{

    Neuron hidden1 = {{0.0, 0.0}, {0.0, 0.0}, 0, 0};
    Neuron hidden2 = {{0.0, 0.0}, {0.0, 0.0}, 0, 0};
    Neuron output = {{0.0, 0.0}, {0.0, 0.0}, 0, 0};

    randWeights(&hidden1);
    randWeights(&hidden2);
    randWeights(&output);

    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < epoch; j++)
        {
            hidden1.inputs[0]=train_inputs[i][0];
            hidden1.inputs[1]=train_inputs[i][1];
            hidden2.inputs[0]=train_inputs[i][0];
            hidden2.inputs[1]=train_inputs[i][1];

            output.inputs[0] = calculateOutput(&hidden1);
            output.inputs[1] = calculateOutput(&hidden2);

            double result = calculateOutput(&output);

            output.error = dsigmoid(result) * (train_outputs[i] - result);
            adjustWeights(&output);

            hidden1.error = dsigmoid(calculateOutput(&hidden1))
                * output.error * output.weights[0];
            hidden2.error = dsigmoid(calculateOutput(&hidden2))
                * output.error * output.weights[1];

            adjustWeights(&hidden1);
            adjustWeights(&hidden2);

            printf("\rEPOCH : %i -> %f xor %f = %f", j+1, train_inputs[i][0], train_inputs[i][1],
                    result);
            sleep(0.1);
            fflush(stdout);
        }
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    if(argc != 2)
        return EXIT_FAILURE;
    train(atof(argv[1]));
}

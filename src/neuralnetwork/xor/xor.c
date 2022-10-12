#include <stdio.h>
#include <stdlib.h>

#include "math.h"
#include "xor.h"


int train_inputs[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
int train_outputs[4] = {0, 1, 1, 0};

double hiddenLayer[2][2]; //2 neuronnes cachés avec 2 input
double hiddenLayerOut[2]; //2 neuronnes chachés avec 2 output
double inputsOut[2]; //2 input pour 1 output
double output;

double weights[2][2];
double weightsOut[2];

double biases[2];
double biasesOut[1];

double errors[3]; //Input 1 | Input 2 | Output
double biasWeight[3];

double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }

double dsigmoid(double x) { return x * (1 - x); }

double initWB() 
{ 
    return (rand()) / (RAND_MAX); 
}

double calculateOutputHidden(int input)
{
    return sigmoid(weights[input][0] * hiddenLayer[input][0] + weights[input][1] * hiddenLayer[input][1] + biasWeight[input]);
}

double calculateOutputOut()
{
    return sigmoid((weightsOut[0] * inputsOut[0]) + (weightsOut[1] * inputsOut[1]) + biasWeight[2]);
}

void train(int epoch) 
{
  for (int j = 0; j < epoch; j++) 
  {

    for (int i = 0; i < 4; i++) 
    {
        hiddenLayer[0][0] = train_inputs[i][0];
        hiddenLayer[0][1] = train_inputs[i][1];
        hiddenLayer[1][0] = train_inputs[i][0];
        hiddenLayer[1][1] = train_inputs[i][1];
        
        inputsOut[0] = calculateOutputHidden(0);
        inputsOut[1] = calculateOutputHidden(1);
        
        output = calculateOutputOut();

        printf("%d xor %d = %f\n", train_inputs[i][0], train_inputs[i][1], output);
        //Adjust weights
        //Output
        errors[2] = dsigmoid(output) * (train_outputs[i] - output);
        weightsOut[0] += errors[2] * inputsOut[0];
        weightsOut[1] += errors[2] * inputsOut[1];
        biasWeight[2] += errors[2];

        //Hidden 1 Error
        errors[0] = dsigmoid(hiddenLayerOut[0]) * errors[2] * weightsOut[0];
        //Hidden 2 Error
        errors[1] = dsigmoid(hiddenLayerOut[1]) * errors[2] * weightsOut[1];

        //Hidden weights
        for(int k = 0; k < 2; k++)
        {
            weights[k][0] = errors[k] * hiddenLayer[k][0];
            weights[k][1] = errors[k] * hiddenLayer[k][1];
            biasWeight[k] += errors[k];
        }

    }
    printf("######################\n");
  }
}

int main(int argc, char** argv) 
{
    if(argc != 2)
        return EXIT_FAILURE;

    for(int i = 0; i < 2; i++)
    {
        weights[i][0] = initWB();
        weights[i][1] = initWB();
        weightsOut[i] = initWB();
        biasWeight[i] = initWB();
    }
    biasWeight[2] = initWB();
    train(atof(argv[1]));
}

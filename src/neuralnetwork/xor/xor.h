#ifndef XOR_H
#define XOR_H

double sigmoid(double x);
double dsigmoid(double x);

double calculateOutputHidden(int input);
double calculateOutputOut();

double initWB();
void train(int epoc);

#endif

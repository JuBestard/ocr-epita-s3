#include <stdio.h>
#include <stdlib.h>
#include <err.h>

const int w_and = 2;
const int threshold_and = 3;

const int w_or= 2;
const int threshold_or = 1;

int nand(int i1, int i2)
{
	return i1*w_and + i2*w_and >= threshold_and ? 0 : 1;
}

int or(int i1, int i2)
{
	return i1*w_or + i2*w_or >= threshold_or ? 1 : 0;
}

int xor(int i1, int i2)
{
	return or(i1,i2)*w_and + nand(i2,i1) * w_and >= threshold_and ? 1 : 0; 
}

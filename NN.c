#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NN.h"

//Defining layers
//#define nbInput 256
//#define nbHidden 256

//Possible outputs
//#define nbOutput 72
const float eta = 0.02f;

const char chars[] =
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,.!?:'-()0123456789";

//Export/import
typedef struct Network
{
        float ws[nbInput*nbHidden*nbOutput];

} Network;

int FileExists(const char *fname)
{
  FILE *f;
  if ((f=fopen(fname, "r")))
  {
    fclose(f);
    return 1;
  }
  return 0;
}

float Sigmoid(float x, int deriv)
{
	if (deriv == 1)
		return x * (1.0f - x);
	else
		return 1.0f/(1.0f + exp(-x));
}

float Softmax(float x[], float out[])
{
	float sum = 0;
	for (size_t i = 0; i < nbOutput; i++)
	{
		sum += x[i];
	}
	for (size_t i = 0; i < nbOutput; i++) {
		out[i] = x[i]/sum;
	}
	return sum;
}

float SoftmaxDeriv(float output)
{
	return output * (1 - output);
}


Network *CreateNetwork(float w1[], float w2[])
{
	size_t totalW1 = nbInput * nbHidden + nbHidden;
	size_t totalW2 = nbHidden * nbOutput + nbOutput;
	size_t maxLen=totalW1+totalW2;

	Network *network=malloc(sizeof(Network));

	size_t k=totalW1;

	for (size_t i=0; i<=totalW1; i++)
		network->ws[i]=w1[i];

	for (size_t i=0; i< totalW2; i++)
	{
		network->ws[k]=w2[i];
		k++;
	}


	return network;
}

int SaveNetwork(float w1[], float w2[])
{
	char *fname="NetSave";
	Network *network=CreateNetwork(w1, w2);
	if(FileExists(fname)) //check if file exists
	{
		rename(fname, "NetSave.bak"); //backup previous network
	}
	FILE *f = fopen(fname, "wb");
	if (f!=NULL)
	{
		fwrite(network, sizeof(Network), 1, f); //save new network
		fclose(f);
		free(network);
		return 0;
	}
	free(network);
	return 1;
}


Network *LoadNetwork(char *fname)
{
	Network *network=malloc(sizeof(Network));

	FILE *f = fopen(fname, "rb");
	if (f!=NULL)
	{
		if(fread(network, sizeof(Network), 1, f)!=1)
			   printf("Error reading file, not a network ?");
	}
	fclose(f);


	return network;
}


char Interpret(float output[])
{
	size_t max = 0;
	for (size_t i = 0; i < nbOutput; i++) {
		if (output[max] < output[i])
			max = i;
	}
	return chars[max];
}

size_t GetIndex(char letter)
{
	for (size_t i = 0; i < nbOutput; i++) {
		if (chars[i] == letter)
     return i;
	}
  return 0;
}

float Error(float output, unsigned char derivate)
{
  if (derivate == 1)
    return -1.0f/output / nbOutput;
  else
    return -log(output + 1e-10) / nbOutput;

}

void Backpropagation(unsigned char m[], float w1[], float w2[], float output[],
        size_t target, float outh[], float sumExp, float outexp[])
{
  size_t i = target;
	//for (size_t i = 0; i < nbOutput; i++) {

    //Update w2 biases
    float deltab2 = eta * Error(output[target], 1) * SoftmaxDeriv(output[i]);
    w2[nbHidden * nbOutput + i] -= deltab2;

    for (size_t j = 0; j < nbHidden; j++) {

    //Update w1 biases
    float deltab1 = deltab2 * w2[i * nbHidden + j] * Sigmoid(outh[j], 1);
    w1[nbHidden * nbInput + j] -= deltab1;

    for (size_t k = 0; k < nbInput; k++) {
        //Update w1
        float deltaw1 = deltab1 * m[k];
        w1[j * nbInput + k] -= deltaw1;
    }

        //Update w2
        float deltaw2 = deltab2 * outh[j];
        w2[i * nbHidden + j] -= deltaw2;
    }


	//}
}

char Prediction(unsigned char m[], float w1[], float w2[], unsigned char letter)
{
	//Forward propagation
	float sum;
	float outh[nbHidden];
	float totalOutSum[nbOutput];
	float outexp[nbOutput];
	float output[nbOutput];

	//input -> hidden
	for (size_t i = 0; i < nbHidden; i++) {
		//Get the ith bias
		sum = w1[nbHidden * nbInput + i];
		for (size_t j = 0; j < nbInput; j++) {
			sum += m[j] * w1[i * nbInput + j];
		}

		outh[i] = Sigmoid(sum, 0);
	}

	//hidden -> output
	for (size_t i = 0; i < nbOutput; i++) {
		//Get initial bias
		sum = w2[nbHidden * nbOutput + i];
		for (size_t j = 0; j < nbHidden; j++) {
			sum += outh[j] * w2[i * nbHidden + j];
		}
		totalOutSum[i] = sum;
	}

	//Get max value for sigmoid
	float max = 0;
	for (size_t i = 0; i < nbOutput; i++)
	{
		if (max < totalOutSum[i])
			max = totalOutSum[i];
	}
	for (size_t i = 0; i < nbOutput; i++) {
		outexp[i] = exp(totalOutSum[i] - max);
	}

	//applies the Softmax to the output and returns de sum of exponentials
	float expSum = Softmax(outexp, output);

	if (letter == 0)
	{
		return Interpret(output);
	}
	else
	{
		size_t target = GetIndex(letter);
    //printf("%f\n", Error(output[target], 0));
		Backpropagation(m, w1, w2, output, target, outh, expSum, outexp);
		return Interpret(output);
	}
}

//Initialization
void Initialization(float w1[], float w2[], unsigned char import)
{
	//nbInputs weights per nbHidden + nbHidden biases
	const size_t totalW1 = nbInput * nbHidden + nbHidden;
	const size_t totalW2 = nbHidden * nbOutput + nbOutput;

	if (import && FileExists("NetSave"))
	{
		Network *network=LoadNetwork("NetSave");
		for (size_t i=0; i< totalW1; i++)
			w1[i]=network->ws[i];

		size_t k=totalW1;
		for (size_t i = 0; i<totalW2; i++)
		{
			w2[i]=network->ws[k];
			k++;
		}

		free(network);
	}
	else
	{
		//Creating random weights
		for (size_t i = 0; i < totalW1; i++)
			w1[i] = 2.0f*rand()/RAND_MAX - 1;

		for (size_t i = 0; i < totalW2; i++)
			w2[i] = 2.0f*rand()/RAND_MAX - 1;
	}
}


int test()
{
	//
	// Initialization
	//
	float w1[nbInput * nbHidden + nbHidden];
	float w2[nbHidden * nbOutput + nbOutput];
	Initialization(w1, w2, 1);

	//
	// Testing
	/*

	unsigned char m[16*16];
	for (size_t i = 0; i < 256; i++) {
		m[i] = 1;
	}

	for (size_t i = 0; i < 10000; i++)
		Prediction(m, w1, w2, '?');
	*/


	printf("w1[5]=%f", w1[5]);
	printf(", w2[5]=%f\n", w2[5]);

	//
	//TEST SAVE
	/*
	Network *network=CreateNetwork(w1, w2);
	printf("w1[5]=%f, w2[5]=%f\n", network->ws[5],
	 network->ws[(nbInput+1)*nbHidden+5]);
	if (SaveNetwork(w1, w2))
	{
		printf("error saving network\n");
		return 1;
	}
	*/
	return 0;
}

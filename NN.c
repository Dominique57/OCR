#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const float eta = 0.025f;
const size_t epochs = 15000;

//Defining layers
const size_t nbInput = 256;
const size_t nbHidden = 256;
const size_t nbOutput = 72;
const char train = 1;

//Possible outputs
const char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,.!?:'-()$0123456789";

//Export/import
typedef struct Network
{
	size_t nbInput;
	size_t nbHidden;
	size_t size;
	float ws[];
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

float SoftmaxDeriv(float x, float sum)
{
	return (sum - x) * x / sum * sum;
}

/*
Network *CreateNetwork(size_t nbInput, size_t nbHidden, float w1[][nbHidden], float w2[])
{
	struct Network *network = malloc(sizeof(Network)+sizeof(float)*((nbInput+1)*nbHidden + nbHidden+1));
	network->size=sizeof(Network)+sizeof(float)*((nbInput+1)*nbHidden + nbHidden+1);
        network->nbInput=nbInput;
        network->nbHidden=nbHidden;

	size_t length=nbInput+1;
	size_t k=0;
	for (size_t i=0; i<length; i++)
	{
		for (size_t j=0; j<nbHidden; j++)
		{
			network->ws[k]=w1[i][j];
			k++;
		}
	}
	length=nbHidden+1;
	for (size_t i=0; i<length; i++)
	{
		network->ws[k]=w2[i];
		k++;
	}

	return network;
}

int SaveNetwork(Network *network)
{
	char *fname="NetSave";
	if(FileExists(fname)) //check if file exists
	{
		rename(fname, "NetSave.bak"); //backup previous network
	}
	FILE *f = fopen(fname, "wb");
	if (f!=NULL)
	{
		fwrite(network, network->size, 1, f); //save new network
		fclose(f);
		return 0;
	}
	return 1;
}

Network *LoadNetwork(char *fname, const size_t nbInput, const size_t nbHidden)
{
	Network *network = malloc(sizeof(struct Network)+sizeof(float)*((nbInput+1)*nbHidden + nbHidden+1));
	FILE *f = fopen(fname, "rb");
	if (f!=NULL)
	{
		fread(network, sizeof(Network), 1, f); //get previous network dimensions
		fread(network->ws, network->size, 1, f); //now we can get the array
	}
	fclose(f);


	return network;
}
*/

char Interpret(float output[])
{
	size_t max = 0;
	for (size_t i = 0; i < nbOutput; i++) {
		if (output[max] < output[i])
			max = i;
	}
	return chars[max];
}

void ConvertChar(char letter, float target[])
{
	for (size_t i = 0; i < nbOutput; i++) {
		target[i] = (chars[i] == letter) ? 1 : 0;
	}
}

//Backpropagation
void Backpropagation(unsigned char m[], float w1[], float w2[], float output[], float target[], float outh[], float sumExp, float outexp[])
{
	for (size_t i = 0; i < nbOutput; i++) {
		for (size_t j = 0; j < nbHidden; j++) {
			for (size_t k = 0; k < nbInput; k++) {
				//Update w1
				float deltaw1 = (output[i] - target[i]) * SoftmaxDeriv(outexp[i], sumExp) * w2[i * nbHidden + j] * Sigmoid(outh[j], 1) * m[k];
				w1[j * nbInput + k] -= deltaw1 * eta;
			}
			//Update w1 biases
			float deltaw1 = (output[i] - target[i]) * SoftmaxDeriv(outexp[i], sumExp) * w2[i * nbHidden + j] * Sigmoid(outh[j], 1);
			w1[nbHidden * nbInput + j] -= deltaw1 * eta;

			//Update w2
			float deltaw2 = (output[i] - target[i]) * SoftmaxDeriv(outexp[i], sumExp) * outh[j];
			w2[i * nbOutput + j] -= deltaw2 * eta;
		}
		//Update w2 biases
		float deltaw2 = (output[i] - target[i]) * SoftmaxDeriv(outexp[i], sumExp);
		w2[nbHidden * nbOutput + i] -= deltaw2 * eta;

	}
}

char Prediction(unsigned char m[], float w1[], float w2[], unsigned letter)
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

	if (!train)
	{
		return Interpret(output);
	}
	else
	{
		float target[nbOutput];
		ConvertChar(letter, target);
		Backpropagation(m, w1, w2, output, target, outh, expSum, outexp);
		return Interpret(output);
	}
}


int main()
{
	//
	// Initialization
	//

	//Creating random weights
	//nbInputs weights per nbHidden + nbHidden biases
	const size_t totalW1 = nbInput * nbHidden + nbHidden;
	const size_t totalW2 = nbHidden * nbOutput + nbOutput;
	float w1[totalW1];
	float w2[nbHidden+1];

	for (size_t i = 0; i <= totalW1; i++)
		w1[i] = 2.0f*rand()/RAND_MAX - 1;

	for (size_t i = 0; i < totalW2; i++)
		w2[i] = 2.0f*rand()/RAND_MAX - 1;

	//
	// Testing
	//
	unsigned char m[16*16];
	for (size_t i = 0; i < 256; i++) {
		m[i] = 1;
	}

	for (size_t i = 0; i < 10000; i++)
		printf("%c\n", Prediction(m, w1, w2, '?'));
	/*
	Network *network=CreateNetwork(nbInput, nbHidden, w1, w2);
	if (SaveNetwork(network))
		printf("error saving network\n");
	free(network);*/
}

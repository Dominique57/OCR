#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>

const float eta = 0.9f;
const size_t epochs = 15000;

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
	//
	//[DEBUG] Print loaded array
	//
	/*
	size_t len=(network->nbInput+1) * network->nbHidden + network->nbHidden + 1;
	for (size_t i=0; i<len; i++)
	{
		printf("array[%ld]=%f\n", i, network->ws[i]);
	}
	*/

	return network;
}

int main(int argc, char *argv[])
{
	//
	// Initialization
	//
	

	if (argc <2 || argc>3)
	{
		errx(1, "Invalid input.");
	}

	const size_t nbInput = strtoul(argv[1], NULL, 10);
	const size_t nbHidden = nbInput;

	//getting number of possible combinations
	const size_t inputLen = 1<<nbInput;
	
	//generate every possible inputs
	float inputs[inputLen][nbInput];
	float targets[inputLen];

	if (argc==3) //Load previous network
        {
                char *fname=argv[2];
                if (!FileExists(fname))
                {
                        errx(1, "file \"%s\" does not exist.", fname);
                }
                Network *network=LoadNetwork(fname, nbInput, nbHidden);

        }
	
	for (size_t i = 0; i < inputLen; i++)
	{	
		unsigned int tmp = i;
		unsigned int res = 0;
		for (size_t j = 0; j < nbInput; j++)
		{
			inputs[i][j] = tmp % 2;
			res += tmp % 2;
			tmp = tmp/2;
		}
		targets[i] = res % 2;
		
		//Print generated input and output
		for (size_t j = 0; j < nbInput; j++)
			printf("%.0f", inputs[i][j]);
		printf(" %.0f\n", targets[i]);
	}


  	//Creating random weights
	float w1[nbInput+1][nbHidden];
  	float w2[nbHidden+1];

  	for (size_t i = 0; i <= nbInput+1; i++) 
	{
    		for (size_t j = 0; j < nbHidden; j++) 
		{
      			w1[i][j] = 2.0f*rand()/RAND_MAX - 1;
    		}
  	}

	for (size_t i = 0; i < nbHidden+1; i++)
	    	w2[i] = 2.0f*rand()/RAND_MAX - 1;
	
	
	//
	// Training
	//	
	
	
	float outh[nbHidden];
	float output;
	float sum;

	for (size_t epoch = 0; epoch < epochs; epoch++)
	{
		//Iterate through every inputs
		for (size_t input = 0; input < inputLen; input++)
		{

			//Forward propagation
			for (size_t j = 0; j < nbHidden; j++)
			{	
				//Start with bias
				sum = w1[nbInput][j];
				for (size_t i = 0; i < nbInput; i++)
				{
					//Add every weights * inputs
					sum += inputs[input][i] * w1[i][j];
				}
				outh[j] = Sigmoid(sum, 0);
			}

			sum = w2[nbHidden];
			for (size_t i = 0; i < nbHidden; i++)
				sum += outh[i] * w2[i];
			
			output = Sigmoid(sum, 0);

			

			//Backpropagation
			for (size_t j = 0; j < nbHidden; j++)
			{
				for (size_t i = 0; i < nbInput; i++)
				{
					//Update input weights
					float deltaw1 = (output - targets[input]) * Sigmoid(output, 1) * w2[j] * Sigmoid(outh[j], 1) * inputs[input][i];
          				w1[i][j] -= deltaw1 * eta;
				}
				
				//Update input bias
				float deltaw1 = (output - targets[input]) * Sigmoid(output, 1) * w2[j] * Sigmoid(outh[j], 1);
        			w1[nbInput][j] -= deltaw1 * eta;
				
				//Update hidden weights
				float deltaw2 = (output - targets[input]) * Sigmoid(output, 1) * outh[j];
        			w2[j] -= deltaw2 * eta;
			}
			//Update hidden bias
			w2[nbHidden] -=  eta * (output - targets[input]) * Sigmoid(output, 1);
		}
	}


	//
	// Testing
	//
	


	Network *network=CreateNetwork(nbInput, nbHidden, w1, w2);

	if (SaveNetwork(network))
	{
		printf("error saving network\n");
	}
	//[DEBUG] print created network
	/*
	
	size_t len=(network->nbInput+1) * network->nbHidden + network->nbHidden + 1;
	for (size_t i = 0; i<len; i++)
	{
		printf("array[%ld]=%f\n",i,network->ws[i]);
	}
	*/


	for (size_t input = 0; input < inputLen; input++)
                {
			printf("Prediction for { ");
			//print input values
			for (size_t i = 0; i < nbInput; i++)
				printf("%.0f ", inputs[input][i]);

                        //Forward propagation
                        for (size_t j = 0; j < nbHidden; j++)
                        {
                                //Start with bias
                                sum = w1[nbInput][j];
                                for (size_t i = 0; i < nbInput; i++)
                                {
                               		//Add every weights * inputs
                                        sum += inputs[input][i] * w1[i][j];
                                }
                                outh[j] = Sigmoid(sum, 0);
                        }

                        sum = w2[nbHidden];
                        for (size_t i = 0; i < nbHidden; i++)
                                sum += outh[i] * w2[i];

                        float output = Sigmoid(sum, 0);
			printf("} : %f\n", output);
			
		}

	//[DEBUG] print w1 and w2
	/*
	for (size_t i=0; i<nbInput+1; i++)
	{
		for (size_t j=0; j<nbHidden; j++)
		{
			printf("w1[%ld][%ld]=%f\n", i, j, w1[i][j]);
		}
	}
	for (size_t i=0; i<nbHidden+1; i++)
	{
		printf("w2[%ld]=%f\n", i, w2[i]);
	}
	*/




	free(network);
}

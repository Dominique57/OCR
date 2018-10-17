#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int nbInput = 2;
const int nbHidden = 3;
const float eta = 0.9f;
const int epochs = 10000;


float Sigmoid(float x, int deriv)
{
  if (deriv == 1)
  {
    return x * (1.0f - x);
  }
    return 1.0f/(1.0f + exp(-x));
}

int main()
{
	//
	// Initialization
	//
	

	//getting number of possible combinations
	const int inputLen = 1<<nbInput;
	
	//generate every possible inputs
	float inputs[inputLen][nbInput];
	float targets[inputLen];
	
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
}

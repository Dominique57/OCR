#ifndef NN_H
#define NN_H

char Prediction(unsigned char m[], float w1[], float w2[], unsigned char letter);
void Initialization(float w1[], float w2[], unsigned char import);
int SaveNetwork(Network *network);

#endif NN_H

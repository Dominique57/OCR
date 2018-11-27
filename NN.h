#ifndef NN_H
#define NN_H

//Export/import
typedef struct Network
{
    size_t nbInput;
    size_t nbHidden;
    size_t size;
    float ws[];
} Network;

char Prediction(unsigned char m[], float w1[], float w2[], unsigned char letter);
void Initialization(float w1[], float w2[], unsigned char import);
int SaveNetwork(Network *network);

#endif
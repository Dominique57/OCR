#ifndef NN_H
#define NN_H

#define nbInput 256
#define nbHidden 256
#define nbOutput 72

//Export/import
typedef struct Network
{
    float ws[nbInput*nbHidden*nbOutput];
} Network;

char Prediction(unsigned char m[], float w1[], float w2[], unsigned char letter);
void Initialization(float w1[], float w2[], unsigned char import);
int SaveNetwork(float w1[], float w2[]);

#endif

#include <stdio.h>

/* For given Secret generate Share corresponding to given share_number */
void getShare(char *secret, int secret_length, int share_number, char *share);

/* For given two shares reconstruct the secret and store at the given pointer */
void getSecret(char *share1, int share_number1, char *share2, int share_number2, char *secret, int secret_length);

/* contruct linear polynomial and get its y(x) mod p */
int y_x(int x, int a0);

/* Construct lagranges polynomial of degree 1 and return the value of constant after mod p */
int lagranges_constant(int x0, int y0, int x1, int y1);
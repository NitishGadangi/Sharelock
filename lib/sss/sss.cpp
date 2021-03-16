#include <stdio.h>
#include <sss.h>
#include <string.h>

/* For given Secret generate Share corresponding to given share_number */
void getShare(char *secret, int secret_length, int share_number, char *share)
{
    int secret_ascii[secret_length];
    //extract ascii values of each character of the secret
    for (int i = 0; i < secret_length; i++)
    {
        secret_ascii[i] = secret[i];
    }

    //encrypt each character
    for (int i = 0; i < secret_length; i++)
    {
        int a0 = secret_ascii[i];
        share[i] = y_x(share_number, a0);
    }
}


/* contruct linear polynomial and get its y(x) mod p */
int y_x(int x, int a0)
{
    int p = 127;  //nearest prime greater than secret values
    int a1 = 112; //random number less than 9, lets assume it to be 112
    return ((a0 + a1 * x) % p);
}


/* For given two shares reconstruct the secret and store at the given pointer */
void getSecret(char *share1, int share_number1, char *share2, int share_number2, char *secret, int secret_length)
{
    int x0 = share_number1;
    int x1 = share_number2;

    int share1_ascii[secret_length];
    //extract ascii values of each character of the share1
    for (int i = 0; i < secret_length; i++)
    {
        share1_ascii[i] = share1[i];
    }

    int share2_ascii[secret_length];
    //extract ascii values of each character of the share2
    for (int i = 0; i < secret_length; i++)
    {
        share2_ascii[i] = share2[i];
    }

    //decrypt secrect using both the shares
    for (int i = 0; i < secret_length; i++)
    {
        int y0 = share1_ascii[i];
        int y1 = share2_ascii[i];
        secret[i] = lagranges_constant(x0, y0, x1, y1);
    }
}


/* Construct lagranges polynomial of degree 1 and return the value of constant after mod p */
int lagranges_constant(int x0, int y0, int x1, int y1)
{
    int p = 127; //nearest prime greater than secret values
    int constant = (((-x1 / (x0 - x1)) * y0) + ((-x0 / (x1 - x0)) * y1));
    if (constant < 0)
    {
        constant = constant + p;
    }
    return (constant % p);
}
#include <stdio.h>

/* For given Secret generate Share corresponding to given share_number */
void getShare(char *secret, int secret_length, int share_number, char *share);

/* For given two shares reconstruct the secret and store at the given pointer */
void getSecret(char *share1, int share_number1, char *share2, int share_number2, char *secret, int secret_length);


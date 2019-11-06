#include <stdio.h>
#include <stdlib.h>
#include "devoir1.h"

int main() {

  char truekey[] = "sacha";
  char permkey[] = "hacas";
  char pattern[] = "economiepolitique";
  char message[] = "jedetesteleconomiepolitiqueetjesuiscontentdeneplusenrevoirdemavie";
   
  printf("CHIFFREMENT\n----------\n");
  char *encryptedmess = encrypt(message, truekey);
  printf("Message original:\n%s\n", message);
  printf("Message chiffré:\n%s\n", encryptedmess);

  printf("\nDECHIFFREMENT\n----------\n");
  char *decmessage = decrypt(encryptedmess, truekey);
  printf("Message déchiffré:\n%s\n",decmessage);
  free(decmessage);

  /* generate and test all keys */
  printf("\nCRYPTANALYSE\n----------------\n");
  char *foundkey = test_all_key_permutations(encryptedmess, pattern, permkey);
  if (foundkey) {
    printf("Clé trouvée = %s\n", foundkey);
    char *decmessage = decrypt(encryptedmess, foundkey);
    printf("Message déchiffrés (contenant le pattern %s):\n%s\n", pattern, decmessage);
    free(decmessage);
  } else
    printf("Clé non trouvée\n");

  exit(0);

}

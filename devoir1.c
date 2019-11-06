
#include "devoir1.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *lettres = "abcdefghijklmnopqrstuvwxz";

/**
 * Chiffre le message.
 */
char *encrypt(char *message, char *key) {
  int keylength = strlen(key);
  int messlength = strlen(message);
  int nrows = ceil(messlength * 1.0 / keylength);

  /* allocation d'un tableau pour stocker le message déchiffré */
  char *tab = malloc((nrows * keylength + 1) * sizeof(char));

  if (!tab) {
    printf("Erreur d'allocation de mémoire dans encrypt\n");
    exit(-1);
  }

  /* remplissage du vecteur selon la règle */
  int i;
  for (i = 0; i < messlength; i++)
    tab[(i % keylength) * nrows + (i / keylength)] = message[i];

  /* remplissage de la fin aléatoirement */
  for (; i < nrows * keylength; i++)
    tab[(i % keylength) * nrows + (i / keylength)] = lettres[rand() % 26];
  tab[i] = '\0';

  /* rotation des colonnes selon la clé */
  for (i = 0; i < keylength; i++) {
    rotate(tab + i * nrows, (key[i] - 'm'), nrows);
  }

  return tab;
}

/**
 * Cherche si message contient pattern, si oui renvoie l'index auquel commence
 * pattern dans message.
 */
int search_pattern(char *pattern, char *message) {
  unsigned count = 0; // Compteur qui compte le nombre de lettres courantes en
                  // commun pendant les itérations.
  unsigned m_length = strlen(message);
  unsigned p_length = strlen(pattern);
  char m_current;
  for (unsigned i = 0; i < m_length; i++) {
    m_current = *(message + i);

    if (m_current == *(pattern + count)) {
      count++;
    } else if (m_current == *pattern) {
      // Si count > 0 quand on arrive dans notre pattern,
      // on ne doit pas repartir à 0 !
      count = 1;
    } else {
      count = 0;
    }

    if (count == p_length) {
      return i - count + 1;
    }
  }
  return -1;
}

/**
 * Déchiffre le message donné avec la clé spécifiée.
 */
char *decrypt(char *message, char *key) {
  int keylength = strlen(key);
  int messlength = strlen(message);
  int nrows = ceil(messlength * 1.0 / keylength);

  char *tab =
      malloc(messlength + 1); // On alloue un premier tableau qui sera une copie
                              // de message sur lequel on travaillera.

  if (!tab) {
    printf("Erreur d'allocation de mémoire dans encrypt\n");
    exit(-1);
  }

  strcpy(tab, message);

  // Etape 1. Remplir dans le bon sens.
  // OK car le message est déjà "dans le bon" sens pour déchiffrer.
  // Il faudra lire dans l'autre sens à la fin cependant.

  // Etape 2 : rotations.
  // Il suffit de tourner dans l'autre sens. Donc nombre de colonnes - la lettre
  // de la clé.
  for (int i = 0; i < keylength; i++) {
    rotate(tab + i * nrows, nrows - (*(key + i) - 'm'), nrows);
  }

  // Etape 3 : Lire le message dans l'autre sens.
  char *decrypted_msg = malloc(messlength + 1); // On alloue.

  if (!decrypted_msg) {
    printf("Erreur d'allocation de mémoire dans encrypt\n");
    exit(-1);
  }

  for (int i = 0; i < nrows; i++) {
    for (int j = 0; j < keylength; j++) {
      *(decrypted_msg + i * keylength + j) = *(tab + j * nrows + i);
    }
  }
  *(decrypted_msg + messlength) = '\0';

  free(tab);

  return decrypted_msg;
}

/**
 * Permute 2 lettres.
 */
static void swap(char *a, char *b) {
  char temp = *a;
  *a = *b;
  *b = temp;
}

/**
 * Réalise le parcourt de l'arbre combinatoire des permutations de la clé.
 * encmessage et pattern sont transmis pour pouvoir tester la condition.
 * key est la clé reçue (ou sa permutation)
 * keylength sa longueur. On le passe en argument pour économiser des appels
 * inutiles à strlen. start représente où on en est dans les permutations (i.e.
 * la position de la lettre permutée)
 *
 * On a atteint une permutation finale seulement en bout d'arbre, c'est-à-dire
 * quand la position de la lettre à permuter est celle de la dernière lettre.
 */
static char *do_perm(char *encmessage, char *pattern, char *key, int keylength,
                     int start) {
  if (start == keylength - 1) { // On atteint un bout de l'arbre combinatoire,
                                // donc une permutation finale.
    char *decrypted_msg = decrypt(encmessage, key);
    if (search_pattern(pattern, decrypted_msg) > -1) {
      // Il y a correspondance, donc on return.
      return key;
    } else {
      return NULL;
    }
  }

  for (int i = start; i < keylength; i++) {
    swap(key + start, key + i); 
    char *d = do_perm(encmessage, pattern, key, keylength, start + 1);
    if (d != NULL) { // La clé a été trouvée.
      return key;
    }
    swap(key + i, key + start); 
  }

  return NULL;
}

char *test_all_key_permutations(char *encmessage, char *pattern, char *key) {
  return do_perm(encmessage, pattern, key, strlen(key), 0);
}

void rotate(char *arr, int d, int n) {
  // Si d < 0, on le rend positif car avec n=5, faire -3 = faire +2
  while (d < 0) {
    d += n;
  }

  for (int j = 0; j < d; j++) {
    char last = *(arr + n - 1);
    for (int i = n - 1; i > 0; i--) {
      *(arr + i) = *(arr + i - 1);
    }
    *arr = last;
  }
}

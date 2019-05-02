/**
 * dict.h
 *
 * Declaration des fonctions de manipulation d'un dictionnaire
 */

#ifndef DICT_H
#define DICT_H

#include <stdbool.h>

#define NB_CARAC 27 // TODO 27 si gestion de l'apostrophe

/*
 * La structure principale du programme est un trie ou chaque Noeud possède jusqu'a
 * 26 enfants ainsi qu'un booleen indiquant si le noeud termine un mot valide.
 */
typedef struct Noeud {
    bool mot_fini;
    struct Noeud *enfants[NB_CARAC];
} Noeud;

// Longueur maximum d'un mot :
// Ex peur des mots long = Hippopotomonstrosesquippedaliophobie, 36 Lettres
#define LONG_MAX 36

/*
 * Retourne True si le mot est dans le dictionnaire, False sinon.
 */
bool appartient(const char *mot);

/*
 * Charge le dictionnaire, retourne True si le dictionnaire est charge, False sinon
 */
bool importer_dict(const char *nom_dic);

/*
 * Retourne le nombre de mots d'un dictionnaire.
 */
unsigned int taille_dic(void);

/*
 * Quitte le dictionnaire, retourne True si le dictionnaire est quitte, False sinon
 */
bool decharger(void);

/*
 * Affiche les mots mal orthographies dans le fichier
 */
void print_erreurs(FILE *fp, int *malorthographies, int *mots);

#endif

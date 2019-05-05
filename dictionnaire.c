/**
 * dictionnaire.c
 *
 * Implémente l'importation et la libération d'un dictionnaire en mémoire
 * depuis un fichier texte.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/* ctype.h pour les fonctions isalpha, tolower, isdigit et isspace */
#include <ctype.h>
#include <string.h>

#include "dictionnaire.h"



/* Variables globales */

// Declaration du Noeud racine
Noeud *racine;

// Déclaration de reserve_noeud : permet d'allouer la taile du dictionnaire en une fois, simplifie la gestion
// mémoire.
Noeud *reserve_noeuds;

// Déclaration du compteur de nombre de mots dans le dictionnaire.
// Est utilise par la fonction taille_dic().
unsigned int nbr_mots;

// Variable global permettant de construire une  reserve de noeuds.
// Type "long" pour ne pas être limité.
long taille_fic;

/**
 * Teste si un mot appartient au dictionnaire.
 * Retourne True si le mot est dans le dictionaire ou False sinon
 */
bool appartient(const char *mot) // Const pour que mot soit en lecture seule, il n'y a pas besoin de le modifier
{
    // Déclaration du pointeur parcourant le trie et initialisation a NULL
    Noeud *nd_parcours_tmp = NULL;

    // Fais pointer nd_parcours_tmp sur la racine
    // Pour chaque nouveau nd_parcours_tmp pointe sur la racine pour verifier le mot
    nd_parcours_tmp = racine;

    // Variable pour iterer char par char dans un mot
    int i = 0;

    // Vérifie que le le mot entré correspond à un parcours de l'arbre se terminant par mot_fini = True
    while (mot[i] != '\0') {
        // On convertit le mot en minuscule pour se préoccuper uniquement de l'orthographe
        char c;
        if (isalpha(mot[i])) {
            c = tolower(mot[i]);
        } else {
            c = mot[i];
        }

        // Gestion de l'apostrophe
        if (c == '\'') {
            // On regarde s'il existe un mot comprenant l'apostrophe, sinon le mot n'appartient pas au dico
            if (nd_parcours_tmp->enfants[NB_CARAC - 1] == NULL) {
                return false;
            }
            // Sinon on continue le parcours en allant au noeud suivant
            nd_parcours_tmp = nd_parcours_tmp->enfants[NB_CARAC - 1];
        }

        // Gestion des lettres
        else if (isalpha(c)) {
            // Verifie que le parcours correspondant au lettres du mot existe jusqu'ici.
            // Retourne False si ce n'est pas le cas.
            if (nd_parcours_tmp->enfants[c - 'a'] == NULL) {
                return false;
            }
            // Sinon on passe à la lettre suivante dans le dico
            nd_parcours_tmp = nd_parcours_tmp->enfants[c - 'a'];
        }
        // On avance dans le mot à vérifier
        i++;
    }

    // Retourne True si le mot existe dans le dictionnaire
    return nd_parcours_tmp->mot_fini;
}

/**
 * Importe le dictionnaire en mémoire.
 * Retourne True en cas de succes et False sinon.
 */
bool importer_dict(const char *nom_dic)
{
    // Ouvre le fichier du dictionnaire
    // 'b' permet d'avoir un stream de byte et non l'interpretation des caractères d'indentation en fonction du système.
    // Cela garantie que fseek determine correctement la taille du fichier.
    FILE *dict = fopen(nom_dic, "rb");

    // Erreur sur le fichier?
    if (dict == false) {
        printf("Impossible d'ouvrir le fichier (dictionnaire.c)");
        return false;
    }

    // Determination de la taille_dic du fichier (en bytes)
    fseek(dict, 0, SEEK_END);
    taille_fic = ftell(dict);

    // Remise du pointeur de fichier au debut
    fseek(dict, 0, SEEK_SET);

    // Initalise reserve_noeuds avec suffisament de mémoire pour différents dictionnaires
    reserve_noeuds = calloc(taille_fic, sizeof(Noeud));

    // Premier Noeud du dictionnaire
    Noeud *prochain_noeud_vide = reserve_noeuds;

    // On transfère le fichier depuis un stream dans un tableau
    char *buffer = malloc(taille_fic+1); // +1 pour le caractere de terminaison
    fread(buffer, 1, taille_fic, dict);

    // Marque la fin du fichier
    buffer[taille_fic] = '\0';

    // Initialisation du Noeud racine du trie
    racine = prochain_noeud_vide + 1;

    // Déclare et initialise le pointeur sur le Noeud courant
    Noeud *nd_courant = NULL;

    // Nouveau string permettant de charger les mots dans le trie
    char *mots_fichier = buffer;

    // Initialise le compteur pour la fonction taille()
    nbr_mots = 0;

    // On boucle tant que le caractere n'est pas  '\0' ou 'NULL'
    while (*mots_fichier) {
        // Départ du Noeud racine pour chaque mot
        nd_courant = racine;

        // Insère les mot dans l'espace mémoire pré-allouée.
        // Si le char est un saut de ligne le mot est terminé fin d'un mot
        for (; *mots_fichier != '\n' && *mots_fichier; mots_fichier++) {
            // Gestion des mots avec apostrophe
            if (*mots_fichier == '\'') {
                // Si le mot avec apostrophe n'existe pas, on l'ajoute
                if (nd_courant->enfants[NB_CARAC - 1] == NULL) {
                    nd_courant->enfants[NB_CARAC - 1] = prochain_noeud_vide++;
                }
                // Passage au noeud suivant
                nd_courant = nd_courant->enfants[NB_CARAC - 1];
            }

            // Gestion des lettres
            else {
                // Si le mot n'existe pas, on l'ajoute en prenant le prochain noeud dans la réserve
                if (nd_courant->enfants[*mots_fichier - 'a'] == NULL) {
                    nd_courant->enfants[*mots_fichier - 'a'] = prochain_noeud_vide++;
                }
                // Passage au noeud suivant
                nd_courant = nd_courant->enfants[*mots_fichier - 'a'];
            }
        }

        // On est arrivé sur '\n' donc le mot est fini
        nd_courant->mot_fini = true;
        // Un mot de plus dans le dico
        nbr_mots++;

        // On absorbe le saut de ligne
        if (*mots_fichier == '\n') {
            mots_fichier++;
        }
    }

    // Fermeture proprement du fichier et libération de la mémoire
    fclose(dict);
    free(buffer);

    // Chargement avec succès
    return true;
}

/**
 * Retourne le nombre de mot dans un dictionnaire (0 si non chargé)
 */
unsigned int taille_dic()
{
    // Déjà calculée lors l'importation du dictionnaire
    return nbr_mots;
}

/**
 * Libération du dictionnaire en mémoire.
 * Retourne true en cas de succès et false sinon.
 */
bool decharger()
{
    // Avantage de l'appel unique à calloc :
    // Un seul appel à free est nécéssaire !
    free(reserve_noeuds);
    // Libération mémoire avec succès
    return true;
}

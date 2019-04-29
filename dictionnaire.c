
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


Noeud *racine;

long fileSize;


unsigned int nbr_mots;


Noeud *reserve_noeuds;


bool appartient(const char *mot)
{
    
    Noeud *trav = NULL;

    
    trav = racine;

    
    int i = 0;

    
    while (mot[i] != '\0') {

        char c = tolower(mot[i]);

        if (isalpha(c)) {

            if (trav->enfants[c - 'a'] == NULL) {
                return false;
            }
            trav = trav->enfants[c - 'a'];
        }
        i++;
    }
    return trav->mot_fini;
}

bool charger_dict(const char *dictionary)
{

    FILE *dict = fopen(dictionary, "rb");

    if (dict == false) {
        printf("Could not open this dictionary (dictionary.c file)");
        return false;
    }

    fseek(dict, 0, SEEK_END);
    fileSize = ftell(dict);

    fseek(dict, 0, SEEK_SET);

    reserve_noeuds = calloc((fileSize), sizeof(Noeud));

    Noeud *nextFreeNode = reserve_noeuds;

    char *buffer = malloc(fileSize + 1);
    fread(buffer, 1, fileSize, dict);

    buffer[fileSize] = '\0';

    racine = nextFreeNode + 1;

    Noeud *trav = NULL;

    char *words = buffer;

    nbr_mots = 0;

    while (*words) {
       
        trav = racine;

        for (; *words != '\n' && *words; words++) {
         
            else {
                if (trav->enfants[*words - 'a'] == NULL) {
                    trav->enfants[*words - 'a'] = nextFreeNode++;
                }

                trav = trav->enfants[*words - 'a'];
            }
        }

        trav->mot_fini = true;
        nbr_mots++;

        if (*words == '\n') {
            words++;
        }
    }

    fclose(dict);
    free(buffer);

    return true;
}

unsigned int taille()
{
    return nbr_mots;
}

bool decharger()
{
    free(reserve_noeuds);
    return true;
}

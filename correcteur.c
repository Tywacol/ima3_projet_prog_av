/**
 * correcteur.c
 *
 *
 */

#include <ctype.h>

#include <stdio.h>
#include <stdbool.h>
// Permet d'utiliser EXIT_FAILURE et non exit(1)
// EXIT_FAILURE est plus portable
#include <stdlib.h>

#include "dictionnaire.h"

// Choix du dictionnaire par default
#define DICTIONNAIRE_DEF "words"

int main(int argc, char* argv[])
{
    // Verification du nombre d'arguments
    if (argc != 2 && argc != 3)
    {
        printf("Utilisation: correcteur [fichier_dictionnaire] texte\n");
        return EXIT_FAILURE;
    }

    // Determine le dictionnaire à utiliser
    char* nom_dict = (argc == 3) ? argv[1] : DICTIONNAIRE_DEF;

    // Charge le dictionnaire
    bool est_charge = importer_dict(nom_dict);


    // Verification de l'importation du dictionnaire
    if (!est_charge)
    {
        printf("Erreur lors de l'importation du dictionnaire %s.\n", nom_dict);
        return EXIT_FAILURE;
    }


    // Ouverture du texte
    char* text = (argc == 3) ? argv[2] : argv[1];
    FILE* fp = fopen(text, "r");
    // Vérifie si l'ouverture est possible
    if (fp == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier %s.\n", text);
        decharger();
        return 1;
    }

    printf("\nMots malorthographiés\n\n");
    int nb_malorthographie;
    int nb_mots;
    print_erreurs(fp, &nb_malorthographie, &nb_mots);

    // Vérifie que la lecture s'est terminée sans erreurs
    if (ferror(fp))
    {
        fclose(fp);
        printf("Erreur lors de la lecture du fichier %s.\n", text);
        decharger();
        return 1;
    }

    // Fermeture du fichier
    fclose(fp);



    if (!decharger())
    {
        printf("Erreur lors de la liberation memoire%s.\n", nom_dict);
        return 1;
    }

    // On affiche les informations sur les mots
    printf("\nMots malorthographiés:     %d\n", nb_malorthographie);
    printf("Mots dans le dictionnaire:  %d\n", taille_dic());
    printf("Mots dans le texte:        %d\n", nb_mots);

    return 0;
}

void print_erreurs(FILE *fp, int *malorthographies, int *mots)
{
    (*malorthographies) = 0;
    (*mots) = 0;// initialisation des variables
    int index = 0;
    char mot[LONG_MAX+1];

    // Verification de l'orthographe
    int nb_mots_ligne = 0;
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // On ne traite que les caracteres et les apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // Ajout du caractere dans le mot temporaire en construction
            mot[index] = c;
            index++;

            // On annule la verification du mot s'il depasse la limite
            if (index > LONG_MAX)
            {
                // On passe le reste du mot
                while ((c = fgetc(fp)) != EOF && isalpha(c));

                // Reinitialisation du mot temporaire
                index = 0;
            }
        }

        // On ignore les mots avec des chiffres
        else if (isdigit(c))
        {
            // On passe le reste du mot
            while ((c = fgetc(fp)) != EOF && isalnum(c));

            // Reinitialisation du mot temporaire
            index = 0;
        }

        // On prend un mot en entier
        else if (index > 0)
        {
            // fin d'un mot
            mot[index] = '\0';

            // On incremente le compteur
            (*mots)++;

            // Vérifie si le mot est bien orthographie
            bool malorthographie = !appartient(mot);


            // On affiche les mots mal orthographies
            if (malorthographie)
            {
                printf("%s\n", mot);
                if (nb_mots_ligne == 2) {
                    printf("\n");
                    nb_mots_ligne = 0;
                }
                (*malorthographies)++;
            }

            // On passe au mot suivant
            index = 0;
        }
    }
}
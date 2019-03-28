#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "listechaines.j"

typedef struct node {
	char lettre;   
    bool fdm;
    struct node *fils_suiv;
    struct node *frere_suiv;
}Node, *PtNode, *Tree;

void ajout_tete(Node * pl, char lettre) {
    
    PtNode ptr = malloc(sizeof(*ptr));
    strcpy(ptr->lettre,lettre);
    ptr->frere_suiv = *pl;
    *pl = ptr;    

}

bool est_present(char lettre; struct node *tree) {        
    while(tree->frere_suiv)!=NULL) {
            if (lettre == (tree->lettre)) {
                return true;
            }
    return false;
    
}

void cons_tree(struct node ** ptr_tree, char lettre, struct node *fils_suiv, struct node *frere_suiv)
{
	*ptr_tree = malloc(sizeof(struct node));
    (*ptr_tree)->lettre=lettre;
    (*ptr_tree)->fils_suiv=fils_suiv;
    (*ptr_tree)->frere_suiv=frere_suiv;
}

/*void ajouter_mot(struct node ** ptr_tree, char *mot) {
    int i =0;
    int etage=1;
    if (*ptr_tree == NULL) {
        cons_tree(ptr_tree,mot[i],NULL,NULL);
        etage++;
        return;
    }
    while (mot[i]!= NULL) {
        if (est_present(mot[i],&ptr_tree) && (etage == i-1)) {
            i++;
            continue;
            } else {ajout_tete(&ptr_tree,mot[i])}
        
    i++;
    }
    
 */   
    
}
void charger_dicho() {}
void afficher_arbre() {}
void free_tree() {}

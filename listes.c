#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "listes.h"

/* fichier à compléter au besoin */

void init_liste_vide(liste_t* L) {
    L->tete = NULL;
}


int ajouter_tete(liste_t* L, string c) { /* retourne 0 si OK, 1 sinon  */
    cellule_t *nouv = malloc(sizeof(cellule_t));
    if (nouv == NULL) {
        return 1;
    }

    nouv->val = c;
    nouv->suivant = L->tete;
    L->tete = nouv;

    return 0;
}

void enlever_tete(liste_t* L) {
  if (L == NULL) { return ; }
  if (L->tete==NULL) { return ;}
  cellule_t *tmp = L->tete->suivant;
  free(L->tete);
  L->tete = tmp;
}

void afficher_elt(cellule_t *cell, int premier) {
	if (cell != NULL) {
		if (premier) {
			printf("%s", cell->val);
		} else {
			printf(", %s", cell->val);
		}
		afficher_elt(cell->suivant, 0);
	}
}

void afficher_pile(liste_t* L) {
	afficher_elt(L->tete, 1);
	printf("\n");
}

int list_len(cellule_t *l) {
	int len = 0;
	while (l != NULL) {
		len++;
		l = l->suivant;
	}
	return len;
}

bool inclusion(cellule_t *a, cellule_t *b) {
	while (a != NULL) {
		cellule_t *c = b;
		while (c != NULL) {
			if (!strcmp(c->val, a->val)) {
				c = NULL;
			} else {
				c = c->suivant;
				if (c == NULL) {
					return false;
				}
			}
		}
		a = a->suivant;
	}
	return true;
}

bool list_eq(cellule_t *a, cellule_t *b) {
	return inclusion(a, b) && inclusion(b, a);
}
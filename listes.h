#ifndef _LISTES_H
#define _LISTES_H

typedef struct liste liste_t;
/* Type de liste à compléter selon votre besoin. */

typedef char* string;

struct cellule {
	string val;
	struct cellule* suivant;
};

typedef struct cellule cellule_t;

struct liste {
	cellule_t *tete;
};

typedef struct liste liste_t;

/* cree une nouvelle liste, initialement vide */
void init_liste_vide(liste_t* L);

/* Ajouter une nouvelle cellule contenant c
 * en tête de la liste L.
 * Si l'ajout est réussi, le résultat est 0,
 * et 1 sinon (échec de l'ajout)
 */
int ajouter_tete(liste_t *L, string c);

/**
 * Supprime l'élément en tête de la liste.
 */
void enlever_tete(liste_t *L);

/**
 * Affiche une liste (et pas une pile comme le nom
 * pourrait le laisser penser (même si en pratique on
 * utilise les listes comme des piles)).
 */
void afficher_pile(liste_t* L);

int list_len(cellule_t *l);
bool list_eq(cellule_t *a, cellule_t *b);

#endif /* _LISTES_H */



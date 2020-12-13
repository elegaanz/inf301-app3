#include "arbres.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

noeud *nouveau_noeud(void) {
    noeud *n = (noeud *)malloc(sizeof(noeud));
    assert(n != NULL);
    n->valeur = NULL;
    n->gauche = NULL;
    n->droit = NULL;
    return n;
}

/* buffer pour lire les caractères des espèces sous forme de "mots" (words) */
#define MAX_WORD_SIZE 255
char buffer[MAX_WORD_SIZE + 1];

/* Variable globale qui contient le prochain caractère à traiter */
static char next_char = ' ';

/* Supprime tous les espaces, tabulations, retour à la ligne */
#define GLOB(f)                                                                \
    while (isspace(next_char)) {                                               \
        next_char = fgetc(f);                                                  \
    }

/* Fonction récursive qui lit un sous-arbre */
/* Appelée une fois à la racine (debut du fichier), puis récursivement
 * pour chaque nœud interne rencontré. */
arbre lire_arbre(FILE *f) {
    arbre racine;

    GLOB(f); /* lit dans next_char le premier caractère non vide */

    if (next_char == '/') {
        next_char = ' '; /* on ne garde pas '/' en mémoire */
        return NULL;
    }

    if (next_char == ')') {
        return NULL;
    }

    if (next_char != '(') {
        fprintf(
            stderr,
            "Error while reading binary tree : '(' or ')' expected at position "
            "%ld\n",
            ftell(f));
        exit(1);
    }

    /* On remplit le buffer tant qu'on lit des caractères alphanumériques */
    char *p = buffer; /* début du buffer */
    next_char = ' ';
    GLOB(f);

    do {
        *p = next_char; /* sauvegarde du char courant */
        next_char = fgetc(f);
        p++;
        assert(p < buffer + MAX_WORD_SIZE);
    } while (!isspace(next_char) && next_char != '(' && next_char != ')');
    /* on arrète si le char suivant est un espace ou une parenthèse */
    *p = '\0'; /* on ferme la chaîne de caractères dans le buffer */

    racine = nouveau_noeud();
    racine->valeur = strdup(buffer); /* dupliquer le mot lu */

    GLOB(f);

    if (next_char == ')') {
        next_char = ' '; /* on est sur une feuille, on prépare la lecture du
                            prochain nœud */
    } else {
        racine->gauche = lire_arbre(f); /* appel récursif pour le fils gauche */
        racine->droit = lire_arbre(f);  /* idem pour le droit */

        GLOB(f); /* lit jusqu'au ')' fermant */

        if (next_char != ')') {
            fprintf(stderr, "Error while reading binary tree: ')' expected\n");
            exit(1);
        }
        next_char = ' '; /* on ne garde pas la parenthèse en mémoire */
    }
    return racine;
}
int est_feuill(arbre a) { return ((a->droit == NULL) && (a->gauche == NULL)); }

void affiche_sous_arbre(FILE *f, noeud *a) {
    if (est_feuill(a)) {
        fprintf(f, "%s [shape=box, color=pink, colorfill=pink, style=filled]\n",
                a->valeur);
    } else {
        if (a->gauche != NULL) {
            fprintf(f, "%s -> %s [label=non]\n", a->valeur, a->gauche->valeur);
            affiche_sous_arbre(f, a->gauche);
        }
        if (a->droit != NULL) {
            fprintf(f, "%s -> %s [label=oui]\n", a->valeur, a->droit->valeur);
            affiche_sous_arbre(f, a->droit);
        }
    }
}

void affiche_arbre(noeud *racine) {
    FILE *f = fopen("arbre.dot", "w");
    if (f == NULL) {
        printf("oh no, impossible de générer un manifik graf\n");
        return;
    }
    fprintf(f, "digraph {\n");
    affiche_sous_arbre(f, racine);
    fprintf(f, "}\n");
    fclose(f);
}

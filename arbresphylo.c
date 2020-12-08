#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "arbres.h"
#include "arbresphylo.h"
#include "listes.h"


int est_feuille(arbre a){
  return ((a->droit == NULL) && (a->gauche == NULL));
}

void analyse_arbre_rec (arbre racine, int* nb_esp, int* nb_carac) {
    if (racine == NULL){
        return;
    } else if (est_feuille(racine)) {
     *nb_esp += 1;
    }
  else{
    *nb_carac += 1;
    analyse_arbre_rec(racine->gauche, nb_esp, nb_carac);
    analyse_arbre_rec(racine->droit, nb_esp, nb_carac);
  }
}

void analyse_arbre (arbre racine, int* nb_esp, int* nb_carac)
{
    *nb_esp = 0;
    *nb_carac = 0;
    analyse_arbre_rec(racine, nb_esp, nb_carac);
}

/* ACTE II */

int rechercher_espece_rec(arbre racine, char *espece, liste_t* seq) {
    if (racine == NULL) {
        return 1;
    }

    if (!strcmp(racine->valeur, espece)) {
        return 0;
    } else {
        if (rechercher_espece_rec(racine->gauche, espece, seq) == 0) {
            return 0;
        }
        if (rechercher_espece_rec(racine->droit, espece, seq) == 0) {
            ajouter_tete(seq, racine->valeur);
            return 0;
        }
    }

    return 1;
}

/* Recherche l'espece dans l'arbre. Modifie la liste passée en paramètre pour y mettre les
 * caractéristiques. Retourne 0 si l'espèce a été retrouvée, 1 sinon.
 */
int rechercher_espece(arbre racine, char *espece, liste_t* seq) {
    int res = rechercher_espece_rec(racine, espece, seq);

    // on inverse seq
    // on pourrait aussi construire la liste dans l'autre sens, mais on
    // multiplierait la complexité par n
    /*if (seq->tete != NULL) {
        cellule_t *prev = NULL;
        cellule_t *cell = seq->tete;
        cellule_t *next = NULL;
        while (cell != NULL) {
            next = cell->suivant;
            cell->suivant = prev;
            prev = cell;
            cell = next;
        }
        seq->tete = prev;
    }*/

    return res;
}

char *top_seq(cellule_t *seq) {
    if (seq != NULL) {
    	return seq->val;
    } else {
        return "";
    }
}

/* Doit renvoyer 0 si l'espece a bien ete ajoutee, 1 sinon, et ecrire un 
 * message d'erreur.
 */
int ajouter_espece(arbre* a, char *espece, cellule_t* seq) {
	if (a == NULL) {
		return 1;
	}

	arbre arb = *a;

	// début de l'algo de TIBO

	if (arb == NULL) {
		*a = malloc(sizeof(noeud));
		arb = *a;
		if (seq != NULL) {
			arb->valeur = seq->val;
			return ajouter_espece(&arb->droit, espece, seq->suivant);
		} else {
			arb->valeur = espece;
			return 0;
		}
	} else if(!est_feuille(arb)){
    	if (!strcmp(arb->valeur, top_seq(seq))) {
      		return ajouter_espece(&arb->droit, espece, seq->suivant);
    	} else {
      		return ajouter_espece(&arb->gauche, espece, seq);
    	}
  	} else if(seq != NULL){
    	arbre esp_tmp = arb;
		*a = malloc(sizeof(noeud));
		arb = *a;
      	arb->valeur = seq->val;
      	arb->gauche = esp_tmp;
      	return ajouter_espece(&arb->droit, espece, seq->suivant);
	} else if (!strcmp(arb->valeur, espece)) {
		printf(
			"Ne peut ajouter %s: déjà présente dans l'arbre.\n",
			espece
		);
		return 1;
  	} else {
		printf(
			"Ne peut ajouter %s: possède les mêmes caractères que %s.\n",
			espece,
			arb->valeur
		);
		return 1;
	}
}

typedef struct elem_file elem_file_t;
typedef struct elem_file {
	arbre arb;
	elem_file_t *suivant;
    int niveau;
} elem_file_t;

typedef struct file {
	elem_file_t *tete;
	elem_file_t *queue;
} file_t;

void init_file(file_t *f) {
	f->queue = NULL;
	f->tete = NULL;
}

elem_file_t *creer_elem(arbre a, int niveau) {
	elem_file_t *elt = malloc(sizeof(elem_file_t));
	elt->suivant = NULL;
	elt->arb = a;
    elt->niveau = niveau;
	return elt;
}

void enfiler(file_t *f, elem_file_t *elem) {
	if (f->tete == NULL) {
		f->tete = elem;
		f->queue = elem;
		elem->suivant = NULL;
	} else {
		elem_file_t *queue = f->queue;
		queue->suivant = elem;
		elem->suivant = NULL;
		f->queue = elem;
	}
}

elem_file_t *defiler(file_t *f) {
	if (f->tete == NULL) {
		return NULL;
	} else {
		elem_file_t *tete = f->tete;
		f->tete = tete->suivant;
		return tete;
	}
}

/* Doit afficher la liste des caractéristiques niveau par niveau, de gauche
 * à droite, dans le fichier fout.
 * Appeler la fonction avec fout=stdin pour afficher sur la sortie standard.
 */
void afficher_par_niveau (arbre a, FILE* fout) {
    if (a == NULL) return;

    file_t f;
    init_file(&f);
    enfiler(&f, creer_elem(a, 0));
    int niveau_act = 0;
    while (f.tete != NULL){
        elem_file_t *elem = defiler(&f);
        arbre carac = elem->arb;
        if (!est_feuille(carac)){
            if (elem->niveau > niveau_act) {
                fprintf(fout, "\n");
                niveau_act = elem->niveau;
            }

            fprintf(fout, "%s ",carac->valeur);
            if (carac->gauche != NULL) {
                enfiler(&f, creer_elem(carac->gauche, niveau_act + 1));
            }
            if (carac->droit != NULL) {
                enfiler(&f, creer_elem(carac->droit, niveau_act + 1));
            }
        }
    }
}

int ajouter_carac(arbre* a, char* carac, cellule_t* seq) {
   printf ("<<<<< À faire: fonction ajouter_carac fichier " __FILE__ "\n >>>>>");
   return 0;
}
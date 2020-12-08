#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listes.h"
#include "arbres.h"
#include "arbresphylo.h"

int DEBUG = 0;

int main(int argc, char* argv[])
{
    char *fichier = NULL;
    char *espece = NULL;

    if (argc < 2) {
        fprintf (stderr, "Usage:  %s [-d] <fichier>\n", argv[0]);
        fprintf (stderr, "\n");
        fprintf (stderr, "Options:\n");
        fprintf (stderr, "\t-d\tmode debug\n");
        exit(1);
    }
    int arg=1;

    while (arg < argc) {
        if (!strncmp(argv[arg], "-d", 2)) {
            DEBUG = 1;
            arg++;
            continue;
        }

        if(!strncmp(argv[arg], "-e", 2)) {
            espece = argv[arg + 1];
            arg += 2;
            continue;
        }

        if (argv[arg][0] == '-') {
            fprintf (stderr, "Option inconnue : '%s'\n", argv[arg]);
            exit(1);
        }
        if (fichier == NULL) {
            fichier = argv[arg];
            arg++;
            continue;
        }
        else {
            fprintf (stderr, "Trop de fichiers sur la ligne de commande : '%s'\n", argv[arg]);
            exit(1);
        }
    }

    debug ("Ouverture de %s\n", fichier);
    FILE* f = fopen (fichier, "r");
    if (!f) {
        fprintf (stderr, "Erreur à l'ouverture du fichier `%s'\n", fichier);
        perror (fichier);
        exit(1);
    }

    arbre mon_arbre = lire_arbre (f);

    int nombre_especes; /* Ne pas initialiser ici, c'est analyse_arbre */
    int nombre_caract;  /* qui doit s'en charger */

    affiche_arbre (mon_arbre);

    analyse_arbre (mon_arbre, &nombre_especes, &nombre_caract);

    printf ("Nombre d'especes de l'arbre: %d\n", nombre_especes);
    printf ("Nombre de caractéristiques de l'arbre: %d\n", nombre_caract);

    if (espece != NULL) {
        liste_t carac;
        init_liste_vide(&carac);
        rechercher_espece(mon_arbre, espece, &carac);
        afficher_pile(&carac);
    }

	/*printf("test de l'ajout\n");
	liste_t carac;
	init_liste_vide(&carac);
	ajouter_tete(&carac, "poil");
	ajouter_tete(&carac, "moustaches");
	ajouter_tete(&carac, "patounes");
	ajouter_tete(&carac, "muzo");
	ajouter_tete(&carac, "vertebres");
	ajouter_espece(&mon_arbre, "chaton", carac.tete);*/
	affiche_arbre(mon_arbre);
	// afficher_par_niveau(mon_arbre);

    return 0;
}



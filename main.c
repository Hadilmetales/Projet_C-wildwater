#include "header.h"

int main(int argc, char* argv[]) {
    // Verification des arguments
    // On attend : ./c-wire fichier_entree.csv
    if (argc != 2) {
        printf("Erreur : Nombre d'arguments incorrect.\n");
        return 1;
    }

    FILE* fic = fopen(argv[1], "r");
    if (fic == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", argv[1]);
        return 2;
    }

    Station* racine = NULL;
    char ligne[1024];
    int id;
    long cap, conso;

    // Lecture du fichier CSV
    // On suppose que le shell a preparé un fichier propre formaté : ID;CAP;CONSO
    while (fgets(ligne, 1024, fic) != NULL) {
        // On essaie de lire 3 valeurs separees par des points-virgules ou deux points
        // Le sscanf permet de parser la ligne
        if (sscanf(ligne, "%d;%ld;%ld", &id, &cap, &conso) == 3) {
            racine = inserer(racine, id, cap, conso);
        }
        else if (sscanf(ligne, "%d:%ld:%ld", &id, &cap, &conso) == 3) {
             // Au cas ou le format serait avec des :
            racine = inserer(racine, id, cap, conso);
        }
    }

    fclose(fic);

    // Ecriture du resultat
    FILE* sortie = fopen("output_c.txt", "w");
    if (sortie == NULL) {
        printf("Erreur creation fichier sortie\n");
        libererArbre(racine);
        return 3;
    }

    // On ecrit les resultats tries (parcours infixe)
    parcoursInfixe(racine, sortie);

    fclose(sortie);
    
    // Nettoyage memoire
    libererArbre(racine);

    return 0;
}
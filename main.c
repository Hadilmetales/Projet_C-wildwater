#include "header.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Erreur : Arguments.\n");
        return 1;
    }

    FILE* fic = fopen(argv[1], "r");
    if (fic == NULL) {
        printf("Erreur ouverture fichier\n");
        return 2;
    }

    Station* racine = NULL;
    char ligne[1024];
    int id;
    long cap, source, traite;

    // Lecture du format : ID;CAP;SOURCE;TRAITE
    while (fgets(ligne, 1024, fic) != NULL) {
        if (sscanf(ligne, "%d;%ld;%ld;%ld", &id, &cap, &source, &traite) == 4) {
            racine = inserer(racine, id, cap, source, traite);
        }
    }
    fclose(fic);

    FILE* sortie = fopen("output_c.txt", "w");
    if (sortie == NULL) {
        libererArbre(racine);
        return 3;
    }

    parcoursInfixe(racine, sortie);
    fclose(sortie);
    libererArbre(racine);
    return 0;
}

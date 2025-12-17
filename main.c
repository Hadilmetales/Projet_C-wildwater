#include "header.h"

int main(int argc, char* argv[]) {
    if (argc != 2) return 1;
    FILE* fic = fopen(argv[1], "r");
    if (!fic) return 2;

    Station* racine = NULL;
    char buffer[256];
    char id[50]; // Buffer pour l'ID textuel
    long v1, v2, v3;

    while (fgets(buffer, 256, fic)) {
        // %[^;] veut dire "lire tout jusqu'au point-virgule"
        if (sscanf(buffer, "%[^;];%ld;%ld;%ld", id, &v1, &v2, &v3) == 4) {
            racine = inserer(racine, id, v1, v2, v3);
        }
    }
    fclose(fic);

    FILE* out = fopen("data_output.txt", "w");
    if (out) {
        parcoursInfixe(racine, out);
        fclose(out);
    }
    libererArbre(racine);
    return 0;
}

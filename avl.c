#include "header.h"

Station* creerStation(char* id) {
    Station* s = malloc(sizeof(Station));
    if (s == NULL) exit(1);
    
    // On copie le texte de l'ID dans la structure
    strcpy(s->id, id);
    
    s->capacite = 0;
    s->consommation = 0;
    s->production = 0;
    s->hauteur = 1;
    s->gauche = NULL;
    s->droite = NULL;
    return s;
}

int max(int a, int b) { return (a > b) ? a : b; }
int hauteur(Station* n) { return (n == NULL) ? 0 : n->hauteur; }
int equilibre(Station* n) { return (n == NULL) ? 0 : hauteur(n->gauche) - hauteur(n->droite); }

Station* rotationDroite(Station* y) {
    Station* x = y->gauche;
    Station* T2 = x->droite;
    x->droite = y;
    y->gauche = T2;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
    return x;
}

Station* rotationGauche(Station* x) {
    Station* y = x->droite;
    Station* T2 = y->gauche;
    y->gauche = x;
    x->droite = T2;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
    return y;
}

Station* inserer(Station* noeud, char* id, long cap, long source, long prod) {
    if (noeud == NULL) {
        Station* n = creerStation(id);
        n->capacite = cap;
        n->consommation = source;
        n->production = prod;
        return n;
    }

    // Comparaison alphabetique avec strcmp
    int cmp = strcmp(id, noeud->id);

    if (cmp < 0) {
        noeud->gauche = inserer(noeud->gauche, id, cap, source, prod);
    } else if (cmp > 0) {
        noeud->droite = inserer(noeud->droite, id, cap, source, prod);
    } else {
        // Meme ID trouvé, on ajoute les valeurs
        if (cap > 0) noeud->capacite = cap;
        noeud->consommation += source;
        noeud->production += prod;
        return noeud;
    }

    noeud->hauteur = 1 + max(hauteur(noeud->gauche), hauteur(noeud->droite));
    int bal = equilibre(noeud);

    // Rotations (utilisant strcmp pour verifier ou on est)
    if (bal > 1 && strcmp(id, noeud->gauche->id) < 0) return rotationDroite(noeud);
    if (bal < -1 && strcmp(id, noeud->droite->id) > 0) return rotationGauche(noeud);
    if (bal > 1 && strcmp(id, noeud->gauche->id) > 0) {
        noeud->gauche = rotationGauche(noeud->gauche);
        return rotationDroite(noeud);
    }
    if (bal < -1 && strcmp(id, noeud->droite->id) < 0) {
        noeud->droite = rotationDroite(noeud->droite);
        return rotationGauche(noeud);
    }
    return noeud;
}

void parcoursInfixe(Station* noeud, FILE* fichier) {
    if (noeud != NULL) {
        parcoursInfixe(noeud->gauche, fichier);
        // %s pour afficher le string ID
        fprintf(fichier, "%s:%ld:%ld:%ld\n", noeud->id, noeud->capacite, noeud->consommation, noeud->production);
        parcoursInfixe(noeud->droite, fichier);
    }
}

void libererArbre(Station* noeud) {
    if (noeud != NULL) {
        libererArbre(noeud->gauche);
        libererArbre(noeud->droite);
        free(noeud);
    }
}

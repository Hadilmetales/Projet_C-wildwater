#include "header.h"

// Crée une nouvelle station avec les 3 valeurs
Station* creerStation(int id, long cap, long source, long traite) {
    Station* s = (Station*)malloc(sizeof(Station));
    if (s == NULL) {
        exit(1);
    }
    s->id = id;
    s->capacite = cap;
    s->vol_source = source;
    s->vol_traite = traite;
    s->hauteur = 1;
    s->gauche = NULL;
    s->droite = NULL;
    return s;
}

int max(int a, int b) { return (a > b) ? a : b; }

int hauteur(Station* n) {
    if (n == NULL) return 0;
    return n->hauteur;
}

int equilibre(Station* n) {
    if (n == NULL) return 0;
    return hauteur(n->gauche) - hauteur(n->droite);
}

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

// Insertion avec gestion des sommes pour le Bonus
Station* inserer(Station* noeud, int id, long cap, long source, long traite) {
    if (noeud == NULL) return creerStation(id, cap, source, traite);

    if (id < noeud->id)
        noeud->gauche = inserer(noeud->gauche, id, cap, source, traite);
    else if (id > noeud->id)
        noeud->droite = inserer(noeud->droite, id, cap, source, traite);
    else {
        // L'usine existe deja : on cumule les donnees
        // Si on recoit une capacite (ligne usine), on met a jour (car c'est pas une somme)
        if (cap > 0) noeud->capacite = cap;
        noeud->vol_source += source;
        noeud->vol_traite += traite;
        return noeud;
    }

    noeud->hauteur = 1 + max(hauteur(noeud->gauche), hauteur(noeud->droite));
    int balance = equilibre(noeud);

    if (balance > 1 && id < noeud->gauche->id) return rotationDroite(noeud);
    if (balance < -1 && id > noeud->droite->id) return rotationGauche(noeud);
    if (balance > 1 && id > noeud->gauche->id) {
        noeud->gauche = rotationGauche(noeud->gauche);
        return rotationDroite(noeud);
    }
    if (balance < -1 && id < noeud->droite->id) {
        noeud->droite = rotationDroite(noeud->droite);
        return rotationGauche(noeud);
    }
    return noeud;
}

// Ecrit : ID:CAPACITE:SOURCE:TRAITE
void parcoursInfixe(Station* noeud, FILE* fichier) {
    if (noeud != NULL) {
        parcoursInfixe(noeud->gauche, fichier);
        fprintf(fichier, "%d:%ld:%ld:%ld\n", noeud->id, noeud->capacite, noeud->vol_source, noeud->vol_traite);
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

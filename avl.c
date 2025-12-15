#include "header.h"

// Crée une nouvelle structure pour une station
Station* creerStation(int id, long cap, long conso) {
    Station* s = (Station*)malloc(sizeof(Station));
    if (s == NULL) {
        printf("Erreur d'allocation memoire\n");
        exit(1);
    }
    s->id = id;
    s->capacite = cap;
    s->consommation = conso;
    s->hauteur = 1; // Un nouveau noeud a toujours une hauteur de 1
    s->gauche = NULL;
    s->droite = NULL;
    return s;
}

// Fonction utilitaire pour avoir le max de deux entiers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Recupere la hauteur d'un noeud
int hauteur(Station* n) {
    if (n == NULL) return 0;
    return n->hauteur;
}

// Calcule le facteur d'equilibre
int equilibre(Station* n) {
    if (n == NULL) return 0;
    return hauteur(n->gauche) - hauteur(n->droite);
}

// Rotation droite pour reequilibrer
Station* rotationDroite(Station* y) {
    Station* x = y->gauche;
    Station* T2 = x->droite;

    // Rotation
    x->droite = y;
    y->gauche = T2;

    // Mise a jour des hauteurs
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;

    return x;
}

// Rotation gauche pour reequilibrer
Station* rotationGauche(Station* x) {
    Station* y = x->droite;
    Station* T2 = y->gauche;

    // Rotation
    y->gauche = x;
    x->droite = T2;

    // Mise a jour des hauteurs
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;

    return y;
}

// Fonction principale : insertion dans l'AVL
Station* inserer(Station* noeud, int id, long cap, long conso) {
    // 1. Insertion normale d'ABR
    if (noeud == NULL) {
        return creerStation(id, cap, conso);
    }

    if (id < noeud->id) {
        noeud->gauche = inserer(noeud->gauche, id, cap, conso);
    } 
    else if (id > noeud->id) {
        noeud->droite = inserer(noeud->droite, id, cap, conso);
    } 
    else {
        // Si l'ID existe deja, on met a jour les valeurs (somme)
        noeud->capacite += cap;
        noeud->consommation += conso;
        return noeud;
    }

    // 2. Mise a jour de la hauteur
    noeud->hauteur = 1 + max(hauteur(noeud->gauche), hauteur(noeud->droite));

    // 3. Verification de l'equilibre
    int balance = equilibre(noeud);

    // Cas Gauche Gauche
    if (balance > 1 && id < noeud->gauche->id)
        return rotationDroite(noeud);

    // Cas Droite Droite
    if (balance < -1 && id > noeud->droite->id)
        return rotationGauche(noeud);

    // Cas Gauche Droite
    if (balance > 1 && id > noeud->gauche->id) {
        noeud->gauche = rotationGauche(noeud->gauche);
        return rotationDroite(noeud);
    }

    // Cas Droite Gauche
    if (balance < -1 && id < noeud->droite->id) {
        noeud->droite = rotationDroite(noeud->droite);
        return rotationGauche(noeud);
    }

    return noeud;
}

// Ecrit les donnees triees dans le fichier de sortie
// Format : ID:CAPACITE:CONSOMMATION
void parcoursInfixe(Station* noeud, FILE* fichier) {
    if (noeud != NULL) {
        parcoursInfixe(noeud->gauche, fichier);
        fprintf(fichier, "%d:%ld:%ld\n", noeud->id, noeud->capacite, noeud->consommation);
        parcoursInfixe(noeud->droite, fichier);
    }
}

// Libere la memoire a la fin du programme
void libererArbre(Station* noeud) {
    if (noeud != NULL) {
        libererArbre(noeud->gauche);
        libererArbre(noeud->droite);
        free(noeud);
    }

}

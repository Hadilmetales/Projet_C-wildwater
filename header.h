#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour representer une usine avec les 3 valeurs du Bonus
typedef struct Station {
    int id;                 // Identifiant
    long capacite;          // Capacité Max (Barre totale)
    long vol_source;        // Eau captée depuis les sources
    long vol_traite;        // Eau réellement traitée (sortie)
    int hauteur;            // Pour l'AVL
    struct Station* gauche;
    struct Station* droite;
} Station;

// Fonctions AVL
Station* creerStation(int id, long cap, long source, long traite);
int max(int a, int b);
int hauteur(Station* n);
int equilibre(Station* n);
Station* rotationDroite(Station* y);
Station* rotationGauche(Station* x);
Station* inserer(Station* noeud, int id, long cap, long source, long traite);
void parcoursInfixe(Station* noeud, FILE* fichier);
void libererArbre(Station* noeud);

#endif

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure modifiée pour gérer les IDs textuels (ex: "Plant #1")
typedef struct Station {
    char id[50];            // Changé de int a char[]
    long capacite;
    long consommation;
    long production;
    
    int hauteur;
    struct Station* gauche;
    struct Station* droite;
} Station;

// Prototypes
Station* creerStation(char* id); // Prend un char* maintenant
int max(int a, int b);
int hauteur(Station* n);
int equilibre(Station* n);

Station* rotationDroite(Station* y);
Station* rotationGauche(Station* x);

// Insertion avec ID textuel
Station* inserer(Station* noeud, char* id, long cap, long source, long prod);
void parcoursInfixe(Station* noeud, FILE* fichier);
void libererArbre(Station* noeud);

#endif

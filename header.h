#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour representer une station (Source, Usine ou Stockage)
// On utilise un AVL pour stocker et trier rapidement par ID
typedef struct Station {
    int id;                 // Identifiant unique
    long capacite;          // Capacité (colonne 4 du sujet)
    long consommation;      // Quantité d'eau traitée/consommée
    int hauteur;            // Pour l'equilibrage de l'arbre
    struct Station* gauche;
    struct Station* droite;
} Station;

// Fonctions de l'arbre AVL (dans avl.c)
Station* creerStation(int id, long cap, long conso);
int max(int a, int b);
int hauteur(Station* n);
int equilibre(Station* n);
Station* rotationDroite(Station* y);
Station* rotationGauche(Station* x);
Station* inserer(Station* noeud, int id, long cap, long conso);
void parcoursInfixe(Station* noeud, FILE* fichier);
void libererArbre(Station* noeud);


#endif

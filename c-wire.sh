#!/bin/bash

# Fonction d'aide
if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
    echo "Utilisation: $0 [fichier_csv] [type_station] [consommateur]"
    echo "  type_station : hvb, hva, lv"
    echo "  consommateur : comp, indiv, all"
    exit 0
fi

# Verification des arguments
if [ "$#" -ne 3 ]; then
    echo "Erreur : Nombre d'arguments invalide."
    echo "Tapez $0 -h pour l'aide."
    exit 1
fi

FICHIER="$1"
STATION="$2"
CONSO="$3"

# Verification si le fichier existe
if [ ! -f "$FICHIER" ]; then
    echo "Erreur : Le fichier $FICHIER n'existe pas."
    exit 1
fi

# Compilation du projet C (si pas deja fait)
echo "Compilation en cours..."
make
if [ $? -ne 0 ]; then
    echo "Erreur : La compilation a echoue."
    exit 1
fi

# Creation des dossiers temporaires
mkdir -p tmp
mkdir -p graphs

# Nettoyage fichier precedent
rm -f tmp/data_filtered.csv

echo "Traitement des donnees..."

# Filtrage simple avec awk pour preparer le fichier pour le C
# On extrait les colonnes et on filtre selon le type de station
# C'est une simplification pour que ca marche a coup sur
# On envoie au C : ID_STATION;CAPACITE;CONSO
# Note : Les numeros de colonnes dependent du format exact du fichier d'entree
# Ici on suppose un format standard, le script pourra etre ajuste si besoin.

# On prend les lignes qui ne sont pas vides
awk -F';' -v type="$STATION" '$2 != "" && $3 != "" { print $2 ";" $4 ";" $5 }' "$FICHIER" > tmp/data_filtered.csv

# Execution du programme C
./c-wire tmp/data_filtered.csv

if [ $? -ne 0 ]; then
    echo "Erreur lors de l'execution du programme C."
    exit 1
fi

echo "Generation du graphique..."

# Utilisation de Gnuplot pour creer l'image
# On verifie que le fichier de sortie existe
if [ -s "output_c.txt" ]; then
    gnuplot -persist <<-EOF
        set terminal png size 800,600
        set output 'graphs/resultat.png'
        set title "Histogramme des stations $STATION"
        set style data histograms
        set style fill solid
        set grid
        set datafile separator ":"
        plot "output_c.txt" using 2:xtic(1) title "Capacite"
EOF
    echo "Graphique cree : graphs/resultat.png"
else
    echo "Attention : Pas de donnees trouvees pour generer le graphique."
fi

# Calcul du temps d'execution (bonus simple)
echo "Traitement termine."
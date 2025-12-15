#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 [csv] [station] [conso]"
    exit 1
fi

FICHIER="$1"
STATION="$2"
CONSO="$3"

if [ ! -f "$FICHIER" ]; then
    echo "Erreur fichier"
    exit 1
fi

make
if [ $? -ne 0 ]; then exit 1; fi

mkdir -p tmp graphs
rm -f tmp/data_filtered.csv

echo "Traitement Bonus..."

# Filtrage intelligent avec AWK pour le Bonus
# On genere 4 colonnes : ID ; CAP ; SOURCE ; TRAITE
# 1. Ligne Usine (Capacité) -> ID;Cap;0;0
# 2. Ligne Source->Usine (Entrée) -> ID;0;Vol;0
# 3. Ligne Usine->Stockage (Sortie) -> ID;0;0;Vol
awk -F';' '
    $2 ~ /Facility/ && $4 != "" { print $2 ";" $4 ";0;0" } 
    $2 ~ /Spring/ && $3 ~ /Facility/ { print $3 ";0;" $4 ";0" }
    $2 ~ /Facility/ && $3 ~ /Storage/ { print $2 ";0;0;" $5 }
' "$FICHIER" | tr -d '[:alpha:] #' > tmp/data_filtered.csv

# Execution du C
./c-wire tmp/data_filtered.csv

if [ $? -ne 0 ]; then exit 1; fi

echo "Generation du graphique Bonus..."

if [ -s "output_c.txt" ]; then
    # Script Gnuplot pour Histogramme Empilé (Stacked)
    gnuplot -persist <<-EOF
        set terminal png size 1000,600
        set output 'graphs/resultat.png'
        set title "Bilan Usines (Capacite vs Source vs Traite)"
        set style data histograms
        set style histogram rowstacked
        set style fill solid
        set boxwidth 0.5
        set grid
        set datafile separator ":"
        # Colonnes C : 1=ID, 2=Max, 3=Source, 4=Real
        # Gnuplot empile : 
        # Bleu = Reel ($4)
        # Rouge = Pertes (Source - Reel) = ($3 - $4)
        # Vert = Capacité Restante (Max - Source) = ($2 - $3)
        plot "output_c.txt" using 4 title "Traite (Reel)" lc rgb "blue", \
             "" using (\$3-\$4) title "Pertes" lc rgb "red", \
             "" using (\$2-\$3):xtic(1) title "Capacite Restante" lc rgb "green"
EOF
    echo "Graphique cree : graphs/resultat.png"
else
    echo "Pas de donnees."
fi

#!/bin/bash

# Verification des arguments
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 [csv] histo [max|src|real]"
    echo "   ou: $0 [csv] leaks [ID]"
    exit 1
fi

CSV="$1"
CMD="$2"
PARAM="$3"

# Verification de l'executable
if [ ! -f "c-wire" ]; then
    make
fi

# Dossiers temporaires
mkdir -p tmp graphs
rm -f tmp/data.csv data_output.txt

if [ "$CMD" == "histo" ]; then
    echo "1. Filtrage du CSV..."
    
    # On prepare les donnees pour le C
    awk -F';' '
    ($2 ~ /Plant/ || $2 ~ /Unit/ || $2 ~ /Facility/) && $4 != "" && $4 != "-" { 
        print $2 ";" $4 ";0;0" 
    }
    ($2 ~ /Source/ || $2 ~ /Well/ || $2 ~ /Spring/ || $2 ~ /Fountain/) && ($3 ~ /Plant/ || $3 ~ /Unit/ || $3 ~ /Module/) { 
        print $3 ";0;" $4 ";0" 
    }
    ($2 ~ /Plant/ || $2 ~ /Unit/ || $2 ~ /Facility/) && $3 ~ /Storage/ { 
        print $2 ";0;0;" $5 
    }
    ' "$CSV" > tmp/data.csv

    echo "2. Calcul C (AVL)..."
    ./c-wire tmp/data.csv

    if [ ! -s "data_output.txt" ]; then
        echo "Erreur: Pas de donnees en sortie du C."
        exit 1
    fi

    echo "3. Tri et Selection (Top 5 + Bottom 5)..."
    
    # On choisit la colonne de tri selon le parametre
    # Colonnes du fichier C : ID:MAX:SRC:REAL
    SORT_COL=2
    if [ "$PARAM" == "src" ]; then SORT_COL=3; fi
    if [ "$PARAM" == "real" ]; then SORT_COL=4; fi

    # On trie numériquement (-n) sur la colonne choisie (-k)
    # -t: definit le separateur
    sort -t":" -k${SORT_COL}n data_output.txt > tmp/sorted.txt

    # On prend les 5 plus petits (debut du fichier)
    head -n 5 tmp/sorted.txt > tmp/graph_data.txt
    
    # On prend les 5 plus grands (fin du fichier)
    tail -n 5 tmp/sorted.txt >> tmp/graph_data.txt

    echo "4. Generation du Graphique..."
    
    gnuplot -persist <<-EOF
        set terminal png size 800,600
        set output 'graphs/graph_${PARAM}.png'
        set style data histograms
        set style fill solid
        set boxwidth 0.5
        set grid
        set datafile separator ":"
        set xtics rotate by -45
        set title "Histogramme : ${PARAM} (Min 5 + Max 5)"
        set ylabel "Volume (m3)"
        set xlabel "Usines"
        
        # On plot le fichier filtré
        plot "tmp/graph_data.txt" using ${SORT_COL}:xtic(1) title "${PARAM}" lc rgb "blue"
EOF
    echo "Graphique généré : graphs/graph_${PARAM}.png"

elif [ "$CMD" == "leaks" ]; then
    echo "Le traitement des fuites (leaks) n'est pas encore fini."
    # Ici, tu pourrais ajouter le grep pour trouver la ligne de l'usine si tu veux
fi

echo "Terminé."

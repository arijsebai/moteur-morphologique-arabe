#!/bin/bash

# Convertir Markdown vers PDF avec sauts de page
echo "🔄 Conversion RAPPORT_TECHNIQUE.md → PDF..."

# Méthode 1: Essayer avec pandoc et LaTeX si disponible
if command -v pdflatex &> /dev/null; then
    pandoc RAPPORT_TECHNIQUE.md -o RAPPORT_TECHNIQUE.pdf \
        -V geometry:margin=2cm \
        -V fontsize=12pt \
        -V papersize=a4 \
        -V colorlinks=true
    echo "✓ PDF généré avec pdflatex"
elif command -v xelatex &> /dev/null; then
    pandoc RAPPORT_TECHNIQUE.md -o RAPPORT_TECHNIQUE.pdf \
        --pdf-engine=xelatex \
        -V geometry:margin=2cm \
        -V fontsize=12pt \
        -V papersize=a4
    echo "✓ PDF généré avec xelatex"
else
    echo "⚠ LaTeX non installé, utilisez RAPPORT_TECHNIQUE_PAGE.html"
fi

ls -lh RAPPORT_TECHNIQUE*.pdf RAPPORT_TECHNIQUE*.html 2>/dev/null | head -10

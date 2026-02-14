#!/bin/bash

# Script pour lancer le moteur morphologique dans un terminal externe
# en français avec support UTF-8

# Couleurs pour l'affichage
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
echo -e "${YELLOW}Lancement du Moteur Morphologique Arabe${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
echo ""

# Chemin du programme
PROGRAM="./moteur_morphologique"
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Vérifier si le programme existe
if [ ! -f "$DIR/$PROGRAM" ]; then
    echo -e "${YELLOW}⚠️  Programme non compilé. Compilation en cours...${NC}"
    cd "$DIR" && make
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Échec de la compilation${NC}"
        exit 1
    fi
fi

echo -e "${GREEN}Détection du terminal disponible...${NC}"
echo ""

# Fonction pour lancer avec GNOME Terminal
launch_gnome_terminal() {
    echo -e "${GREEN}✓ Utilisation de GNOME Terminal${NC}"
    gnome-terminal --geometry=100x30 \
                   --title="Moteur Morphologique Arabe" \
                   --working-directory="$DIR" \
                   -- bash -c "export LANG=fr_FR.UTF-8; export LC_ALL=fr_FR.UTF-8; $PROGRAM --fr; read -p 'Appuyez sur Entrée pour fermer...'"
}

# Fonction pour lancer avec Konsole (KDE)
launch_konsole() {
    echo -e "${GREEN}✓ Utilisation de Konsole${NC}"
    konsole --workdir "$DIR" \
            --title "Moteur Morphologique Arabe" \
            -e bash -c "export LANG=fr_FR.UTF-8; export LC_ALL=fr_FR.UTF-8; $PROGRAM --fr; read -p 'Appuyez sur Entrée pour fermer...'"
}

# Fonction pour lancer avec xterm
launch_xterm() {
    echo -e "${GREEN}✓ Utilisation de xterm${NC}"
    xterm -fa 'Monospace' -fs 12 \
          -title "Moteur Morphologique Arabe" \
          -geometry 100x30 \
          -e bash -c "cd '$DIR'; export LANG=fr_FR.UTF-8; export LC_ALL=fr_FR.UTF-8; $PROGRAM --fr; read -p 'Appuyez sur Entrée pour fermer...'"
}

# Fonction pour lancer avec Tilix
launch_tilix() {
    echo -e "${GREEN}✓ Utilisation de Tilix${NC}"
    tilix -w "$DIR" \
          -t "Moteur Morphologique Arabe" \
          -e "bash -c 'export LANG=fr_FR.UTF-8; export LC_ALL=fr_FR.UTF-8; $PROGRAM --fr; read -p \"Appuyez sur Entrée pour fermer...\"'"
}

# Fonction pour lancer avec terminal par défaut
launch_default() {
    echo -e "${GREEN}✓ Utilisation du terminal par défaut${NC}"
    x-terminal-emulator -e bash -c "cd '$DIR'; export LANG=fr_FR.UTF-8; export LC_ALL=fr_FR.UTF-8; $PROGRAM --fr; read -p 'Appuyez sur Entrée pour fermer...'"
}

# Détecter et lancer le terminal disponible
if command -v gnome-terminal &> /dev/null; then
    launch_gnome_terminal
elif command -v konsole &> /dev/null; then
    launch_konsole
elif command -v tilix &> /dev/null; then
    launch_tilix
elif command -v xterm &> /dev/null; then
    launch_xterm
elif command -v x-terminal-emulator &> /dev/null; then
    launch_default
else
    echo -e "${YELLOW}⚠️  Aucun terminal externe détecté.${NC}"
    echo ""
    echo "Tentative de lancement dans le terminal actuel..."
    cd "$DIR"
    export LANG=fr_FR.UTF-8
    export LC_ALL=fr_FR.UTF-8
    $PROGRAM --fr
fi

echo ""
echo -e "${GREEN}✓ Terminal lancé${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"

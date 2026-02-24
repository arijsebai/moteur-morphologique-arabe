# =====================================================
# Makefile - Moteur Morphologique Arabe
# =====================================================

# =========== Variables ===========
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
CXXFLAGS_DEBUG = -std=c++17 -Wall -Wextra -g -O0

# Répertoires
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = .

# Fichiers - Inclure tous les sous-répertoires
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
EXECUTABLE = $(BIN_DIR)/moteur_morphologique

# =========== Cibles Principales ===========

.PHONY: all build run clean rebuild debug help install

all: build

# Compiler le projet
build: $(EXECUTABLE)
	@echo "✓ Compilation réussie : $(EXECUTABLE)"

# Édition de liens
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "✓ Édition de liens terminée"

# Compilation des fichiers objets
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "✓ Compilé : $<"

# Exécuter le programme
run: build
	@echo "Démarrage du moteur morphologique..."
	./$(EXECUTABLE)

# Mode serveur
server: build
	@echo "Démarrage en mode serveur..."
	./$(EXECUTABLE) --server

# Version debug avec symboles de débogage
debug: CXXFLAGS = $(CXXFLAGS_DEBUG)
debug: clean build
	@echo "✓ Version debug compilée"

# Nettoyer les fichiers générés
clean:
	@rm -rf $(BUILD_DIR) $(EXECUTABLE)
	@echo "✓ Nettoyage terminé"

# Recompiler complètement
rebuild: clean build

# Installation (copie l'exécutable)
install: build
	@cp $(EXECUTABLE) /usr/local/bin/
	@echo "✓ Installé dans /usr/local/bin/moteur_morphologique"

# Afficher l'aide
help:
	@echo ""
	@echo "╔════════════════════════════════════════╗"
	@echo "║  Moteur Morphologique Arabe - Makefile ║"
	@echo "╚════════════════════════════════════════╝"
	@echo ""
	@echo "Cibles disponibles :"
	@echo "  make              - Compile le projet (défaut)"
	@echo "  make build        - Compile le projet"
	@echo "  make run          - Compile et exécute"
	@echo "  make server       - Lance en mode serveur HTTP"
	@echo "  make debug        - Compile en mode debug (symboles inclus)"
	@echo "  make clean        - Supprime les fichiers compilés"
	@echo "  make rebuild      - Nettoyage + compilation"
	@echo "  make install      - Installe l'exécutable (root)"
	@echo "  make help         - Affiche cette aide"
	@echo ""
	@echo "Exemple d'utilisation :"
	@echo "  make              # Compilation simple"
	@echo "  make run          # Compiler et exécuter"
	@echo "  make clean        # Nettoyer"
	@echo ""

# 🌟 Moteur Morphologique Arabe

> Un système intelligent pour analyser et générer des mots en arabe basé sur les structures de données avancées.

---

## 📋 Informations Générales

| Aspect | Détail |
|--------|--------|
| **Année** | 2025-2026 |
| **Responsables** | Narjes Ben Hariz, Sahbi Bahroun |
| **Langage** | C++17 |
| **Paradigme** | POO + Structures de Données |

---

## 🎯 Objectifs du Projet

Ce projet implémente un **moteur morphologique avancé** pour la langue arabe capable de :

✅ **Indexer les racines** - Stockage efficace des racines trilitères dans un arbre AVL équilibré  
✅ **Gérer les schèmes** - Organisation des schèmes morphologiques en table de hachage  
✅ **Générer des dérivés** - Création de mots à partir de racines et schèmes  
✅ **Valider les mots** - Vérification morphologique d'un mot par rapport à une racine  
✅ **Interface intuitive** - Console interactive et interface web responsive  

---

## 📁 Architecture du Projet

```
moteur-morphologique-arabe/
│
├─ src/                      # 💻 Code source C++
│   ├─ main.cpp              # Point d'entrée principal
│   ├─ AVL.h / AVL.cpp       # Arbre AVL pour les racines
│   ├─ TableSchemes.h/.cpp   # Gestion des schèmes
│   └─ Morphologie.h/.cpp    # Moteur morphologique
│
├─ include/                  # 📚 Headers optionnels
│
├─ data/                     # 📊 Données d'entrée
│   ├─ racines.txt           # Racines arabes trilitères
│   └─ schemes.txt           # Schèmes morphologiques
│
├─ web/                      # 🌐 Interface Web
│   ├─ index.html            # Interface utilisateur
│   ├─ style.css             # Styles modernes
│   └─ script.js             # Logique JavaScript
│
├─ tests/                    # 🧪 Tests et données
│   ├─ test_racines.txt
│   └─ test_mots.txt
│
├─ CMakeLists.txt            # Configuration build
└─ README.md                 # Documentation
```

---

## ⚙️ Fonctionnalités Principales

### 1. 🔤 Gestion des Racines Arabes
- ✓ Chargement depuis fichier texte
- ✓ Insertion dynamique dans un arbre AVL
- ✓ Recherche et affichage équilibrés
- ✓ Recherche O(log n) garantie

### 2. 📐 Gestion des Schèmes Morphologiques
- ✓ Stockage en table de hachage
- ✓ Ajout/suppression dynamique
- ✓ Énumération rapide des schèmes
- ✓ Accès O(1) en moyenne

### 3. 🔄 Génération Morphologique
- ✓ Création de dérivés à partir de racine + schème
- ✓ Support des racines régulières et irrégulières
- ✓ Respect des règles phonétiques arabes
- ✓ Génération de formes féminines et plurielles

### 4. ✔️ Validation Morphologique
- ✓ Vérification de l'appartenance d'un mot à une racine
- ✓ Identification du schème utilisé
- ✓ Retour détaillé des analyses
- ✓ Gestion des ambiguïtés

### 5. 🖥️ Interface Console Interactive
- ✓ Menu principal itératif
- ✓ Navigation fluide
- ✓ Affichage formaté en arabe
- ✓ Support UTF-8

### 6. 🌐 Interface Web (Optionnelle)
- ✓ Design moderne et responsive
- ✓ Visualisation des racines en temps réel
- ✓ Analyse interactive des mots
- ✓ Affichage RTL pour l'arabe

---

## 💻 Installation et Compilation

### Prérequis
```
✓ C++17 ou supérieur (g++, clang)
✓ CMake 3.10+ (optionnel)
```

### Méthode 1️⃣ : Compilation directe
```bash
cd /home/hadil/projet/moteur-morphologique-arabe
g++ -std=c++17 src/*.cpp -o moteur_morphologique
./moteur_morphologique
```

### Méthode 2️⃣ : Compilation avec CMake
```bash
mkdir build && cd build
cmake ..
make
./moteur_morphologique
```

### Mode serveur (Backend + Frontend)
Lance un serveur HTTP local qui sert l'interface web et l'API.
```bash
./moteur_morphologique --server
```
Puis ouvre `http://localhost:8080` dans ton navigateur.

---

## 🧪 Tests et Validation

### Fichiers de Test
| Fichier | Description |
|---------|-------------|
| `tests/test_racines.txt` | Ensemble de racines de référence |
| `tests/test_mots.txt` | Mots à analyser et résultats attendus |

### Exécution des Tests
```bash
# Les tests s'exécutent automatiquement au lancement
./moteur_morphologique
# Sélectionner l'option "Tests" dans le menu
```

---

## 📖 Exemple d'Utilisation

### Console Interactive
```
╔════════════════════════════════════════╗
║  🌟 محرك التحليل الصرفي العربي    ║
║  Moteur Morphologique Arabe             ║
╚════════════════════════════════════════╝

[1] 📚 Afficher les racines
[2] ➕ Ajouter une racine
[3] 🔄 Générer dérivés
[4] ✔️  Vérifier un mot
[5] 🌐 Interface web
[0] ❌ Quitter

Votre choix > 4

Entrez le mot à analyser : مكتوب
═══════════════════════════════════════
📊 Résultats :
─────────────────────────────────────
Mot : مكتوب
Racine : كتب
Schème : مفعول
Statut : ✓ Valide
═══════════════════════════════════════
```

---

## 🏗️ Architecture Technique

### Arbre AVL
```
Classe : AVL<T>
├─ Complexité : O(log n) insertions/suppressions/recherches
├─ Caractéristiques : Auto-équilibrage
└─ Usage : Stockage des racines arabes
```

### Table Schèmes (Hash Map)
```
Classe : TableSchemes
├─ Complexité : O(1) accès en moyenne
├─ Caractéristiques : Dynamique
└─ Usage : Gestion des schèmes morphologiques
```

### Moteur Morphologique
```
Classe : Morphologie
├─ Méthodes principales :
│   ├─ analyzeWord(mot)     → Analyse et extraction de racine
│   ├─ generateWords(racine) → Génération de dérivés
│   └─ validateWord(mot)     → Vérification morphologique
└─ Intégration : AVL + TableSchemes
```

---

## 📊 Performance

| Opération | Complexité | Remarques |
|-----------|-----------|-----------|
| Insérer une racine | O(log n) | Arbre AVL |
| Chercher une racine | O(log n) | Binaire équilibré |
| Accéder à un schème | O(1) | Table de hachage |
| Générer un dérivé | O(1) | Concaténation simple |
| Analyser un mot | O(log n) | Recherche de racine |

---

## 🚀 Améliorations Futures

- [ ] Support des racines quadrilitères
- [ ] Analyse de morphologie avancée (suffixes/préfixes)
- [ ] Base de données pour racines irrégulières
- [ ] API REST pour le moteur
- [ ] Interface graphique desktop (Qt/wxWidgets)
- [ ] Diacritisation automatique
- [ ] Support multilingue

---

## 📝 Licence

Ce projet est fourni à titre éducatif. Tous droits réservés.

---

## 👨‍💼 Auteurs

- **Enseignants** : Narjes Ben Hariz, Sahbi Bahroun
- **Année** : 2025-2026
- **Institution** : [À compléter]

---

## 📞 Support et Contribution

Pour toute question ou contribution, veuillez contacter les responsables du projet.

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
| **Interface** | Console interactive (FR/AR) + Web responsive |

---

## 🎯 Objectifs du Projet

Ce projet implémente un **moteur morphologique avancé** pour la langue arabe capable de :

✅ **Gestion des racines** - Stockage efficace en arbre AVL, affichage ordonné  
✅ **Gestion des schèmes** - Organisation en table de hachage, ajout dynamique  
✅ **Générer des dérivés** - Création de mots à partir de racine + schème  
✅ **Valider les mots** - Vérification morphologique rapide (O(1))  
✅ **Console interactive** - Menus bilingues (Français/Arabe)  
✅ **Interface web** - Design moderne et responsive  

---

## 📁 Architecture du Projet

```
moteur-morphologique-arabe/
│
├─ src/                      # 💻 Code source C++
│   ├─ main.cpp              # Point d'entrée + menus console
│   ├─ AVL.h / AVL.cpp       # Arbre AVL pour racines
│   ├─ HashTable.h/.cpp      # Table de hachage
│   ├─ TableSchemes.h/.cpp   # Gestion des schèmes
│   └─ Morphologie.h/.cpp    # Moteur morphologique
│
├─ data/                     # 📊 Données persistantes
│   ├─ racines.txt           # Racines arabes trilitères
│   └─ schemes.txt           # Schèmes morphologiques
│
├─ web/                      # 🌐 Interface Web
│   ├─ index.html            # Interface utilisateur
│   ├─ style.css             # Styles modernes (gradient, animations)
│   └─ script.js             # Logique JavaScript + API calls
│
├─ tests/                    # 🧪 Données de test
│   ├─ test_racines.txt
│   └─ test_mots.txt
│
├─ build/                    # 🔨 Répertoire de compilation
│
├─ lancer_fr.sh              # Lanceur console français
├─ lancer_ar.sh              # Lanceur console arabe
├─ lancer_console.sh         # Lanceur console (GNOME Terminal)
├─ lancer_web.sh             # Lanceur serveur web
├─ CMakeLists.txt            # Configuration CMake
├─ Makefile                  # Compilation simple
└─ README.md                 # Documentation
```

---

## ⚙️ Fonctionnalités Principales

### 1. 📚 Gestion des Racines
- ✓ Stockage efficace en arbre AVL
- ✓ Affichage ordonné et numéroté
- ✓ Ajout dynamique
- ✓ Persistance en fichier

### 2. 📐 Gestion des Schèmes
- ✓ Table de hachage pour accès O(1)
- ✓ Énumération complète
- ✓ Ajout/consultation dynamique
- ✓ Sauvegarde automatique

### 3. 🔄 Génération de Dérivés
- ✓ Création à partir de racine + schème
- ✓ Respect des règles phonétiques arabes
- ✓ Gestion des cas irréguliers
- ✓ Accès rapide en cache

### 4. ✔️ Vérification de Mots
- ✓ Identification de racine O(1)
- ✓ Retour du schème appliqué
- ✓ Gestion des diacritiques
- ✓ Support UTF-8 complet

### 5. 🖥️ Console Interactive
- ✓ Menus colorés et intuitifs
- ✓ **Bilingue** : Français et العربية
- ✓ Pause après résultats
- ✓ Navigation par sous-menus

### 6. 🌐 Interface Web
- ✓ Design responsive (mobile/desktop)
- ✓ RTL automatique pour l'arabe
- ✓ API REST intégrée
- ✓ Pas de dépendances externes

---

## 💻 Installation et Lancement

### Prérequis
```
✓ C++17 (g++ ou clang)
✓ Make (optional)
✓ CMake 3.10+ (optional)
```

### Compilation

**Méthode 1 : Makefile (recommandé)**
```bash
cd moteur-morphologique-arabe
make
```

**Méthode 2 : Compilation directe**
```bash
g++ -std=c++17 src/*.cpp -o moteur_morphologique
```

**Méthode 3 : CMake**
```bash
mkdir build && cd build
cmake .. && make
```

### Lancement

#### Console (avec sélection de langue au démarrage)
```bash
./moteur_morphologique
```
Puis sélectionner : `[1] Français` ou `[2] العربية`

#### Console en Français
```bash
./lancer_fr.sh
# ou
./moteur_morphologique --fr
```

#### Console en Arabe
```bash
./lancer_ar.sh
# ou
./moteur_morphologique --ar
```

#### Console dans GNOME Terminal (meilleur rendu arabe)
```bash
./lancer_console.sh
```
*Remarque : Configure la police à "Amiri" ou "Scheherazade" dans GNOME Terminal.*

#### Interface Web
```bash
./lancer_web.sh
# ou
./moteur_morphologique --server [--port 8080]
```
Puis ouvre `http://localhost:8080` dans le navigateur.

---

## 📖 Exemple d'Utilisation

### Mode Console (Français)
```
════════════════════════════════════════
Moteur Morphologique Arabe
════════════════════════════════════════

[1] 📚 Gestion des racines
[2] 📐 Gestion des schèmes
[3] 🔄 Générer dérivés d'une racine
[4] ✔️  Vérifier un mot
[5] ❌ Quitter
════════════════════════════════════════
Choix : 4

Mot : كتب
════════════════════════════════════════
✅ Mot valide
════════════════════════════════════════
Mot    : كتب
Racine : كتب
Schème : فعل
════════════════════════════════════════

Appuyez sur Entrée pour continuer...
```

### Mode Console (العربية)
```
════════════════════════════════════════
محرك التحليل الصرفي العربي
════════════════════════════════════════

[1] 📚 إدارة الجذور
[2] 📐 إدارة الأوزان
[3] 🔄 توليد مشتقات جذر
[4] ✔️  التحقق من كلمة
[5] ❌ خروج
════════════════════════════════════════
الاختيار : 1
```

### Menu Gestion des Racines
```
════════════════════════════════════════
Gestion des racines
════════════════════════════════════════

[1] 📋 Afficher les racines
[2] ➕ Ajouter une racine
[0] ⏎ Retour
════════════════════════════════════════
Choix : 1

📚 Racines disponibles (12)
════════════════════════════════════════
  1. كتب
  2. درس
  3. علم
  ...
════════════════════════════════════════

Appuyez sur Entrée pour continuer...
```

### Interface Web
- Accès via `http://localhost:8080`
- 5 sections : Analyse mot, Racines, Schèmes, Dérivés
- Stockage local persistant
- Support RTL natif

---

## 🏗️ Architecture Technique

### Arbre AVL
```
Complexité : O(log n) pour insertion/recherche/suppression
Équilibrage : Automatique par facteur d'équilibre
Usage : Racines arabes (données statiques/semi-dynamiques)
```

### Table de Hachage
```
Complexité : O(1) accès moyen
Implémentation : Adressage ouvert
Usage : Schèmes, index d'accès rapide
```

### Index Inversé
```
Structure : map<mot_simplifié, {racine, schème}>
Complexité : O(1) vérification de mot
Optimisation : Construction en phase d'indexation
```

### Moteur Morphologique
```
Fonction clé : appliquer_scheme(schème, racine)
- Remplace ف/ع/ل par lettres réelles
- Applique règles phonétiques arabes
- Gère diacritiques automatiquement
```

---

## 📊 Performance Observée

| Opération | Complexité | Détails |
|-----------|-----------|---------|
| Vérifier un mot | O(1) | Index inversé |
| Ajouter une racine | O(log n) | Insertion AVL |
| Générer dérivés | O(m) | m = nombre de schèmes |
| Afficher racines | O(n) | Traversée en-ordre |
| Rechercher schème | O(1) | Hash table |

**Temps réel** : ~1ms par vérification de mot

---

## 🛠️ Modes de Compilation/Exécution

```bash
# Mode console interactif
./moteur_morphologique              # Menu langue
./moteur_morphologique --fr         # Français direct
./moteur_morphologique --ar         # Arabe direct

# Mode serveur web
./moteur_morphologique --server             # Port 8080 (défaut)
./moteur_morphologique --server --port 3000  # Port personnalisé

# Lanceurs rapides
./lancer_fr.sh              # Console FR
./lancer_ar.sh              # Console AR
./lancer_console.sh         # GNOME Terminal
./lancer_web.sh             # Serveur + navigateur
```

---

## 🚀 Améliorations Possibles

- [ ] Racines quadrilitères
- [ ] Interface graphique (Qt)
- [ ] Base de données (SQLite)
- [ ] Diacritisation automatique
- [ ] Support multilingue (FR/AR/EN)
- [ ] API REST documentée (Swagger)
- [ ] Tests unitaires
- [ ] Compression des données

---

## 🐛 Dépannage

### Lettres arabes déconnectées en console
**Solution** : Utiliser GNOME Terminal avec police "Amiri" ou "Scheherazade"
```bash
./lancer_console.sh
```

### Problème d'affichage UTF-8
**Solution** : Vérifier la locale
```bash
locale
export LC_ALL=en_US.UTF-8
```

### Port 8080 déjà utilisé
**Solution** : Changer le port
```bash
./moteur_morphologique --server --port 3000
```

---

## 📄 Licence

Projet éducatif - Université ISI ARIANA

---

## 👥 Auteurs

- **Enseignants** : Narjes Ben Hariz, Sahbi Bahroun
- **Étudiants** : Sebai Arij, Hanafi Balkis, Hosni Hadil
- **Année** : 2025-2026
- **Institution** : ISI ARIANA
- **Dernière mise à jour** : Février 2026

---

## 📞 Contacts

Pour toute question ou suggestion, contactez les responsables du projet.


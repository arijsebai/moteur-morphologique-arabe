# 🌟 Moteur Morphologique Arabe

> Système complet d'analyse et de génération morphologique pour l'arabe  
> **Interfaces** : Console interactive (FR/AR) + Web responsive  
> **Structures** : Arbre AVL + Table de hachage + Index inversé  
> **Performance** : Validation < 1ms, Index O(1)

---

## 📋 Informations Générales

| Aspect | Détail |
|--------|--------|
| **Année** | 2025-2026 |
| **Responsables** | Narjes Ben Hariz, Sahbi Bahroun |
| **Langage** | C++17 (STL) |
| **Interfaces** | Console bilingue (FR/AR) + Web HTTP |
| **Structures de données** | AVL, HashTable chaînée (11 schèmes), Index inversé |

---

## 🎯 Fonctionnalités

### Analyse Morphologique
- 🔍 **Analyser un mot** : Détection racine + schème en O(1)
- 🔄 **Générer dérivés** : Création de mots à partir de racine + schème(s)
- ✅ **Vérifier mot** : Confirmation si mot dérive d'une racine spécifique
- 📝 **Verbes irréguliers** : Gestion de 5 verbes (قال، قرأ، دعى، وقف، مدّ) avec formes spécifiques

### Gestion des Données
- 📚 **Racines** : Affichage ordonné (AVL), ajout/suppression dynamique (21 racines)
- 📐 **Schèmes** : Accès O(1) (HashTable), modification/sauvegarde automatique (11 schèmes)
- 💾 **Persistance** : Fichiers texte (racines.txt, schemes.txt)

### Interfaces Utilisateur
- **Console** : Menus colorés, bilingue (Français/العربية), toutes fonctionnalités
- **Web** : Responsive design, API REST, pas de dépendances externes

---

## 📁 Architecture du Projet

```
moteur-morphologique-arabe/
│
├─ src/                          # Code source C++
│   ├─ main.cpp                  # Console + Serveur HTTP
│   ├─ AVL.{h,cpp}               # Arbre AVL (racines)
│   ├─ HashTable.{h,cpp}         # Table de hachage (schèmes)
│   ├─ TableSchemes.{h,cpp}      # Gestion schèmes
│   └─ Morphologie.{h,cpp}       # Moteur morphologique
│
├─ data/                         # Données persistantes
│   ├─ racines.txt               # 21 racines (dont 5 verbes irréguliers)
│   └─ schemes.txt               # 11 schèmes morphologiques
│
├─ web/                          # Interface Web
│   ├─ index.html                # Pages HTML
│   ├─ style.css                 # Styles (responsive, RTL)
│   └─ script.js                 # Logique JavaScript + Fetch API
│
├─ build/                        # Compilation (gitignore)
├─ tests/                        # Données test
│
├─ Makefile                      # Build avec make
├─ RAPPORT_TECHNIQUE.md          # 3 pages : structures, algorithmes, complexité
├─ SYNTHESE_TECHNIQUE.md         # Vue d'ensemble
├─ METRIQUES_PROJET.md           # Benchmark & statistiques
└─ README.md                     # Ce fichier
```

---

## 🚀 Installation et Lancement

### Prérequis
```
✓ C++17 (g++ 7.0+, clang 5.0+)
✓ Make (optionnel, compilation directe possible)
✓ UTF-8 support (Linux/Mac/Windows)
```

### Compilation

**Méthode 1 : Make (recommandé)**
```bash
cd moteur-morphologique-arabe
make build
# Exécutable généré : ./moteur_morphologique
```

**Méthode 2 : Compilation directe**
```bash
g++ -std=c++17 -O2 src/*.cpp -o moteur_morphologique
```

**Méthode 3 : Compilation debug**
```bash
make debug  # Avec symboles et -O0
```

### Lancement - Console

#### Option 1 : Mode menu simple
```bash
./moteur_morphologique
```
Menu principal avec sélection de langue (Français/العربية) au démarrage.

#### Option 2 : Lancer directement en français
```bash
./moteur_morphologique fr
```

#### Option 3 : Lancer directement en arabe
```bash
./moteur_morphologique ar
```

#### Option 4 : Avec scripts (Linux/Unix)
```bash
# Arabe dans terminal standard  
./lancer_terminal_arabe.sh

# Français dans terminal standard
./lancer_terminal_francais.sh
```

### Lancement - Interface Web

#### Option 1 : Mode serveur intégré
```bash
./moteur_morphologique --server
# Serveur lancé sur http://localhost:8080
# Ouvrir navigateur : http://localhost:8080
```

#### Option 2 : Port personnalisé
```bash
./moteur_morphologique --server --port 9000
# Serveur sur http://localhost:9000
```

**Note** : Le serveur C++ utilise les mêmes fichiers de données et verbes irréguliers que le reste de l'application.

---

## 📖 Utilisation - Console

### Menu Principal
```
════════════════════════════════════════
Moteur Morphologique Arabe
════════════════════════════════════════
[1] 🔍 Analyser un mot
[2] 📚 Gestion des racines
[3] 📐 Gestion des schèmes
[4] 🔄 Générer dérivés d'une racine
[5] ✅ Vérifier dérivé d'une racine
[6] ❌ Quitter
════════════════════════════════════════
Choix :
```

### Exemple 1 : Analyser un mot
```
Choix : 1
Entrer le mot : كاتب

Résultat :
  Racine : كتب
  Schème : فاعل
  Valide : Oui
```

### Exemple 2 : Afficher les racines
```
Choix : 2 → [1] Afficher racines

Racines disponibles (ordre alphabétique) :
  1. أخذ    (prendre)
  2. أكل    (manger)
  3. درس    (étudier)
  4. دخل    (entrer)
  ...
  21. نظر   (regarder)

Total : 21 racines (dont 5 verbes irréguliers : قال، قرأ، دعى، وقف، مدّ)
```

### Exemple 3 : Générer dérivés
```
Choix : 4

Entrer la racine : كتب
Générer [1] tous les dérivés ou [2] sélection spécifique ? 1

Dérivés de "كتب" :
  فعل      → كتب
  فاعل    → كاتب
  مفعول  → مكتوب
  فعّل    → كتّب
  استفعال → استكتاب
  ...
  Total : 11 dérivés
```

### Exemple 3b : Verbe irrégulier
```
Choix : 4

Entrer la racine : قرأ
Générer [1] tous les dérivés ou [2] sélection spécifique ? 1

Dérivés de "قرأ" (verbe irrégulier) :
  فعل      → قرأ
  فاعل    → قارئ
  مفعول  → مقروء
  تفعيل   → تقريء
  استفعال → استقراء
  ...
  Total : 11 formes irrégulières
```

### Exemple 4 : Vérifier un dérivé
```
Choix : 5

Entrer le mot : مكتوب
Entrer la racine : كتب

Résultat :
  "مكتوب" dérive de "كتب" avec le schème "مفعول"
```

---

## 🌐 Utilisation - Interface Web

### Accès
- **URL** : http://localhost:8080
- **Navigateur** : Chrome, Firefox, Safari, Edge
- **Mobile** : Responsive design (portrait/paysage)

### Interface Web - Analyse de mots

**Formulaire** :
```
┌─────────────────────────────────────┐
│ 🔍 Analyser un mot                  │
├─────────────────────────────────────┤
│ Entrer le mot :  [_____________]    │
│                                     │
│ [Analyser]                          │
├─────────────────────────────────────┤
│ Résultat :                          │
│  Racine : كتب                      │
│  Schème : فاعل                     │
│  Valide : ✓                         │
└─────────────────────────────────────┘
```

### Interface Web - Gestion racines

**Liste** :
```
Affichage ordonné de toutes les racines avec bouton "+" pour ajouter.
Clic sur racine = détail + dérivés associés
```

### Interface Web - Génération dérivés

**Sélection** :
```
1. Choisir racine (autocomplete)
2. Choisir schème(s) ou [Tous]
3. Affichage : tableau avec tous les dérivés
```

---

## 🛠️ Commandes Makefile

```bash
make build          # Compilation (release -O2)
make debug          # Compilation debug (-g, -O0)
make run            # Compilation + lancement console
make server         # Compilation + lancement serveur web
make clean          # Suppression fichiers objet
make rebuild        # Clean + build
make help           # Affiche commandes disponibles
```

### Exemple complet
```bash
# Compilation et lancement console
make build
./moteur_morphologique

# Compilation et lancement serveur
make server

# Nettoyage et rebuild
make rebuild
```

---

## 📊 Structure de Données et Complexité

### Arbre AVL (Racines)
| Opération | Complexité | Détails |
|-----------|-----------|---------|
| Insertion | O(log n) | Équilibrage automatique |
| Recherche | O(log n) | Max ~5 comparaisons pour 21 racines |
| Affichage | O(n) | Parcours infixe (ordre alphabétique) |

### Table de Hachage (Schèmes)
| Opération | Complexité | Détails |
|-----------|-----------|---------|
| Insertion | O(1) | Chaînage pour collisions |
| Recherche | O(1) | Facteur charge = 0.34 (11/32 buckets) |
| Redimensionnement | Rare | Seuil 0.75 jamais atteint avec 11 schèmes |

**Explication simple** :

Imaginez une **bibliothèque avec 32 étagères numérotées** (buckets). Pour ranger un livre (schème) :
1. On calcule son numéro d'étagère avec une formule mathématique (fonction de hachage djb2)
2. On pose le livre sur cette étagère
3. Si plusieurs livres vont sur la même étagère, on les empile (chaînage)

**Pourquoi c'est rapide ?**
- Chercher un schème = aller directement à l'étagère (O(1)) au lieu de chercher partout
- Avec 11 schèmes sur 32 étagères → la plupart des étagères ont 0 ou 1 livre → accès instantané
- Alternative (std::map) = chercher dans un arbre trié = 4 comparaisons à chaque fois

**Facteur de charge 0.34** : Seulement 34% des étagères occupées → très peu de collisions

### Validation Morphologique
| Approche | Complexité | Temps | Gain |
|----------|-----------|-------|------|
| Naïve (itération) | O(n×k×m) | 180ms | Baseline |
| Avec index inversé | **O(1)** | **< 1ms** | **180×** |

---

## 📈 Performance et Optimisations

### Mesures réelles

```
Opération                    Temps        Complexité
───────────────────────────────────────────────────
Chargement 21 racines       2.0 ms       O(n log n)
Construction index          22 ms        O(n×k)
Validation mot              < 1 μs       O(1)
Génération 11 dérivés       10 μs        O(k×m)
Insertion racine AVL        2.5 μs       O(log n)
Verbes irréguliers          < 0.5 μs     O(1) lookup
```

### Optimisations implémentées
1. **Index inversé** : Pré-calcul au démarrage → validation O(1)
2. **Cache dérivés** : Dans nœuds AVL → évite régénération
3. **Cache simplification** : Diacritiques → évite suppressions répétées
4. **UTF-8 parsing** : Manuel → gestion correcte caractères arabes

---

## 🧪 Test et Validation

### Fichiers de test fournis

```
tests/test_racines.txt    # Racines pour benchmark
tests/test_mots.txt       # Mots pour validation
```

### Validation manuelle

```bash
# Lancer et tester interactivement
./moteur_morphologique

# Menu 1 : Analyser "كاتب" → doit retourner racine "كتب", schème "فاعل"
# Menu 1 : Analyser "قارئ" → doit retourner racine "قرأ", schème "فاعل" (irrégulier)
# Menu 4 : Générer dérivés de "درس" → 11 mots
# Menu 4 : Générer dérivés de "قرأ" → 11 formes irrégulières prédéfinies
# Menu 5 : Vérifier "كاتب" dérive de "كتب" → ✓
```

### Vérification serveur web

```bash
# Terminal 1 : Lancer serveur
./moteur_morphologique --server

# Terminal 2 : Test API
curl "http://localhost:8080/api/analyze?word=كاتب"
# Retour: {"word":"كاتب","root":"كتب","scheme":"فاعل","valid":true}

curl "http://localhost:8080/api/analyze?word=قارئ"
# Retour: {"word":"قارئ","root":"قرأ","scheme":"فاعل","valid":true}

curl "http://localhost:8080/api/roots"
# Retour: {"roots":["أخذ","أكل",...,"مدّ","نظر"]}

curl "http://localhost:8080/api/schemes"
# Retour: {"schemes":["استفعال","افتعل",...,"مفعول"]}

curl "http://localhost:8080/api/derives?root=قرأ"
# Retour: Toutes les formes irrégulières de قرأ
```

---

## 🐛 Troubleshooting

### Compilation échoue
```bash
# Vérifier version g++
g++ --version    # Doit être >= 7.0

# Nettoyer et recompiler
make clean
make build
```

### Caractères arabes mal affichés (console)
```bash
# Linux/Mac
export LC_ALL=en_US.UTF-8
./moteur_morphologique

# Windows (PowerShell)
$env:LANG = "en_US.UTF-8"
```

### Serveur web n'accède pas au port 8080
```bash
# Utiliser port différent
./moteur_morphologique --server --port 9000

# Ou vérifier port occupé
lsof -i :8080
```

### Index.html non trouvé au lancement serveur
```bash
# Vérifier répertoire courant
pwd    # Doit être moteur-morphologique-arabe/

# Ou lancer depuis répertoire parent
cd moteur-morphologique-arabe
./moteur_morphologique --server
```

---

## 📚 Documentation Détaillée

Pour plus de détails techniques :
- [RAPPORT_TECHNIQUE.md](RAPPORT_TECHNIQUE.md) : Structures, algorithmes, complexité (3 pages)
- [SYNTHESE_TECHNIQUE.md](SYNTHESE_TECHNIQUE.md) : Vue d'ensemble avec diagrammes
- [METRIQUES_PROJET.md](METRIQUES_PROJET.md) : Benchmarks et statistiques détaillées

---

## 🎓 Apprentissages Clés

Ce projet illustre :
- **Structures de données** : Choix AVL vs ABR vs HashTable pour différents cas
- **Optimisation** : Trade-off mémoire (28KB) pour gain temps (500×)
- **Multilingue** : Gestion UTF-8 correcte pour arabe, français, emoji
- **Architecture** : Séparation structures de données, algorithmes, interfaces

---

## ✅ Checklist de Utilisation

### Console
- [ ] Compiler avec `make build`
- [ ] Lancer avec `./moteur_morphologique`
- [ ] Tester analyse de mot (menu 1)
- [ ] Tester gestion racines (menu 2)
- [ ] Tester gestion schèmes (menu 3)
- [ ] Tester génération dérivés (menu 4)
- [ ] Tester vérification (menu 5)

### Web
- [ ] Lancer serveur avec `./moteur_morphologique --server`
- [ ] Accéder à http://localhost:8080
- [ ] Tester analyse de mot
- [ ] Tester affichage racines
- [ ] Tester affichage schèmes
- [ ] Tester sur mobile (responsive)

---

## 📄 Licence

Voir fichier [LICENSE](LICENSE)

**Version** : 2.0 (avec optimisations complètes + verbes irréguliers)  
**Dernière mise à jour** : 24 février 2026

---

## 💻 Installation et Lancement

### Prérequis
```
✓ C++17 (g++ 7.0+, clang 5.0+)
✓ Make
✓ UTF-8 support (Linux/Mac/Windows)
```

### Compilation

**Méthode 1 : Makefile (recommandé)**
```bash
make build
# Exécutable généré : ./moteur_morphologique
```

**Méthode 2 : Compilation directe**
```bash
g++ -std=c++17 -O2 src/**/*.cpp src/*.cpp -o moteur_morphologique
```

### Lancement

#### Console Interactive
```bash
./moteur_morphologique              # Menu de sélection langue
./moteur_morphologique fr           # Français direct
./moteur_morphologique ar           # Arabe direct
```

#### Scripts de Lancement
```bash
./lancer_terminal_francais.sh       # Console français
./lancer_terminal_arabe.sh          # Console arabe
```

#### Serveur Web
```bash
./moteur_morphologique --server             # Port 8080
./moteur_morphologique --server --port 3000 # Port personnalisé
```
Puis ouvrir http://localhost:8080 dans le navigateur.

---

## 📖 Exemple d'Utilisation

### Mode Console (Français)
```
════════════════════════════════════════
Moteur Morphologique Arabe
════════════════════════════════════════

[1] 🔍 Analyser un mot
[2] 📚 Gestion des racines
[3] 📐 Gestion des schèmes
[4] 🔄 Générer dérivés d'une racine
[5] ✅ Vérifier dérivé d'une racine
[6] ❌ Quitter
════════════════════════════════════════
Choix : 1

Mot à analyser : قارئ
════════════════════════════════════════
✅ Mot valide (verbe irrégulier)
════════════════════════════════════════
Mot     : قارئ
Racine  : قرأ
Schème  : فاعل
════════════════════════════════════════
```

### Menu Génération de Dérivés
```
Choix : 4

Racine : قرأ
════════════════════════════════════════
🔄 Options de génération
════════════════════════════════════════

[1] Tous les dérivés (11 - verbe irrégulier)
[2] Sélectionner des schèmes
[0] Annuler
════════════════════════════════════════
Choix : 1

📊 Dérivés de la racine: قرأ (11 - irrégulier)
════════════════════════════════════════
  1. فعل → قرأ
  2. فاعل → قارئ
  3. مفعول → مقروء
  4. فعّل → قرّأ
  5. تفعيل → تقريء
  ...
════════════════════════════════════════
```

### Menu Vérification de Dérivé
```
Choix : 5

Mot à vérifier : قارئ
Racine : قرأ

✅ Mot valide - dérive de cette racine (verbe irrégulier)
════════════════════════════════════════
Mot     : قارئ
Racine  : قرأ
Schème  : فاعل
════════════════════════════════════════
```

### Mode Console (العربية)
```
════════════════════════════════════════
محرك التحليل الصرفي العربي
════════════════════════════════════════

[1] 🔍 تحليل كلمة
[2] 📚 إدارة الجذور
[3] 📐 إدارة الأوزان
[4] 🔄 توليد مشتقات جذر
[5] ✅ التحقق من المشتق
[6] ❌ خروج
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

📚 Racines disponibles (21)
════════════════════════════════════════
  1. أخذ
  2. أكل
  3. جلس
  ...
  17. قال (irrégulier)
  18. قرأ (irrégulier)
  19. كتب
  20. مدّ (irrégulier)
  21. نظر
════════════════════════════════════════

Appuyez sur Entrée pour continuer...
```

### Interface Web
- Accès via `http://localhost:8080`
- Interface responsive avec support RTL
- API REST complète
- Gestion des verbes irréguliers transparente

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
- Lookup O(1) pour verbes irréguliers (قال، قرأ، دعى، وقف، مدّ)
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
./moteur_morphologique fr           # Français direct
./moteur_morphologique ar           # Arabe direct

# Scripts de lancement
./lancer_terminal_francais.sh       # Console français
./lancer_terminal_arabe.sh          # Console arabe

# Mode serveur web
./moteur_morphologique --server             # Port 8080 (défaut)
./moteur_morphologique --server --port 3000 # Port personnalisé
```

---

## 🚀 Améliorations Possibles

- [ ] Racines quadrilitères (4 lettres)
- [ ] Plus de verbes irréguliers (أكل، أخذ، etc.)
- [ ] Interface graphique native (Qt/GTK)
- [ ] Base de données (SQLite) pour grandes collections
- [ ] Diacritisation automatique complète
- [ ] Support multilingue interface (FR/AR/EN)
- [ ] API REST documentée (Swagger/OpenAPI)
- [ ] Tests unitaires automatisés
- [ ] Règles morphologiques avancées (assimilation, gémination)

---

## 🐛 Difficultés Rencontrées et Solutions

### 1️⃣ Encodage UTF-8 des caractères arabes
**Problème** : Les caractères arabes occupent 3 octets en UTF-8. L'indexation naïve `string[i]` retourne 1 octet, corrompant les données.

**Solution implémentée** : Parser UTF-8 manuel analysant les bits de poids fort pour détecter les débuts de caractères multi-octets :
```cpp
if((c & 0xF0) == 0xE0) len = 3;  // Arabe = 3 octets
```

**Impact** : Gestion correcte garantie de tous les caractères arabes, français et emoji.

---

### 2️⃣ Performance de validation morphologique
**Problème initial** : Approche naïve = 180ms/mot (boucle sur 18 racines × 11 schèmes × génération = 198 opérations).

**Solution implémentée** : Index inversé pré-calculé au démarrage avec lookup O(1) :
```
V1.0 Naïve :    180ms  (O(n×k×m) = 198 ops)
V1.1 + Cache :   12ms  (Gain 15×)
V2.0 + Index :  < 1ms  (Gain 180×)
```

**Résultat** : Validation < 1ms (acceptable pour interface interactive)

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


# 🌟 Moteur Morphologique Arabe

> Système d'analyse et de génération morphologique pour l'arabe  
> **Langage** : C++17 · **Interfaces** : Console bilingue (FR/AR) + Web HTTP  
> **Structures** : AVL · HashTable (chaînage) · Index inversé · **Validation** : < 1ms

---

## 📋 Informations Générales

| Aspect | Détail |
|--------|--------|
| **Année** | 2025-2026 |
| **Responsables** | Narjes Ben Hariz, Sahbi Bahroun |
| **Étudiants** | Sebai Arij, Hanafi Balkis, Hosni Hadil |
| **Données** | 21 racines (dont 5 verbes irréguliers) · 11 schèmes |

---

## 🎯 Fonctionnalités

| Fonctionnalité | Description |
|----------------|-------------|
| 🔍 Analyser un mot | Détection racine + schème en O(1) via index inversé |
| 🔄 Générer dérivés | Création de mots à partir d'une racine + schème(s) |
| ✅ Vérifier dérivé | Confirme si un mot dérive d'une racine spécifique |
| 📝 Verbes irréguliers | Gestion de قال، قرأ، دعى، وقف، مدّ avec formes dédiées |
| 📚 Gestion racines | Affichage ordonné (AVL), ajout/suppression dynamique |
| 📐 Gestion schèmes | Accès O(1) (HashTable), modification + sauvegarde auto |
| 💾 Persistance | Fichiers texte (`racines.txt`, `schemes.txt`) |

---

## 📁 Architecture du Projet

```
moteur-morphologique-arabe/
├─ src/
│   ├─ main.cpp               # Console + Serveur HTTP
│   ├─ AVL.{h,cpp}            # Arbre AVL (racines)
│   ├─ HashTable.{h,cpp}      # Table de hachage (schèmes)
│   ├─ TableSchemes.{h,cpp}   # Gestion schèmes
│   └─ Morphologie.{h,cpp}    # Moteur morphologique
├─ data/
│   ├─ racines.txt            # 21 racines
│   └─ schemes.txt            # 11 schèmes
├─ web/
│   ├─ index.html
│   ├─ style.css              # Responsive, RTL
│   └─ script.js              # Fetch API
├─ tests/                     # Données de test
├─ Makefile
├─ RAPPORT_TECHNIQUE.md       # Structures, algorithmes, complexité
├─ SYNTHESE_TECHNIQUE.md      # Vue d'ensemble avec diagrammes
└─ METRIQUES_PROJET.md        # Benchmarks et statistiques
```

---

## 🚀 Installation et Lancement

### Prérequis
```
✓ C++17 (g++ 7.0+ ou clang 5.0+)
✓ Make (optionnel)
✓ Support UTF-8 (Linux / Mac / Windows)
```

### Compilation

```bash
# Recommandé
make build

# Direct
g++ -std=c++17 -O2 src/*.cpp -o moteur_morphologique

# Debug
make debug
```

### Lancement — Console

```bash
./moteur_morphologique          # Sélection de langue au démarrage
./moteur_morphologique fr       # Directement en français
./moteur_morphologique ar       # Directement en arabe
```

### Lancement — Interface Web

```bash
./moteur_morphologique --server              # Port 8080 (défaut)
./moteur_morphologique --server --port 9000  # Port personnalisé
# Ouvrir : http://localhost:8080
```

---

## 📖 Utilisation — Console

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
```

### Exemples

**Analyser un mot**
```
Mot : كاتب  →  Racine : كتب  |  Schème : فاعل  |  Valide : ✓
```

**Générer les dérivés de كتب**
```
فعل → كتب  |  فاعل → كاتب  |  مفعول → مكتوب  |  فعّل → كتّب  |  استفعال → استكتاب  …  (11 dérivés)
```

**Verbe irrégulier قرأ**
```
فعل → قرأ  |  فاعل → قارئ  |  مفعول → مقروء  |  استفعال → استقراء  …  (11 formes irrégulières)
```

**Vérifier un dérivé**
```
مكتوب ← كتب  →  Dérivé avec schème "مفعول" ✓
```

---

## 🌐 Utilisation — Interface Web

- **URL** : `http://localhost:8080`  
- **Navigateurs** : Chrome, Firefox, Safari, Edge  
- **Mobile** : Responsive (portrait/paysage)

**Fonctionnalités disponibles** : analyse de mots, gestion des racines (liste + ajout), génération de dérivés avec sélection de schème(s), affichage autocomplete.

### API REST

```bash
curl "http://localhost:8080/api/analyze?word=كاتب"
# {"word":"كاتب","root":"كتب","scheme":"فاعل","valid":true}

curl "http://localhost:8080/api/roots"
# {"roots":["أخذ","أكل",...,"نظر"]}

curl "http://localhost:8080/api/schemes"
# {"schemes":["استفعال","افتعل",...,"مفعول"]}

curl "http://localhost:8080/api/derives?root=قرأ"
# Toutes les formes irrégulières de قرأ
```

---

## 🛠️ Commandes Makefile

```bash
make build      # Compilation release (-O2)
make debug      # Compilation debug (-g, -O0)
make run        # Build + lancement console
make server     # Build + lancement serveur web
make clean      # Suppression fichiers objet
make rebuild    # Clean + build
make help       # Affiche toutes les commandes
```

---

## 📊 Structures de Données et Complexité

### Vue d'ensemble

| Structure | Usage | Complexité |
|-----------|-------|-----------|
| **Arbre AVL** | Racines (tri alphabétique) | O(log n) insertion/recherche, O(n) affichage |
| **HashTable chaînée** | Schèmes (accès rapide) | O(1) moyen (facteur de charge 0.34 — 11/32 buckets) |
| **Index inversé** | Validation de mots | O(1) lookup, construit au démarrage |

### Progression des performances — Validation

| Version | Approche | Temps |
|---------|----------|-------|
| V1.0 | Naïve O(n×k×m) | 180 ms |
| V1.1 | + Cache dérivés | 12 ms |
| V2.0 | + Index inversé O(1) | < 1 ms |

### Mesures réelles

```
Chargement 21 racines     2.0 ms     O(n log n)
Construction index        22 ms      O(n×k)
Validation mot            < 1 μs     O(1)
Génération 11 dérivés     10 μs      O(k×m)
Insertion racine AVL      2.5 μs     O(log n)
Verbes irréguliers        < 0.5 μs   O(1)
```

> **Optimisations** : Index inversé pré-calculé · Cache dérivés dans nœuds AVL · Cache suppression de diacritiques · Parser UTF-8 manuel (caractères arabes = 3 octets)

---

## 🐛 Troubleshooting

| Problème | Solution |
|----------|----------|
| Compilation échoue | `g++ --version` ≥ 7.0, puis `make clean && make build` |
| Caractères arabes corrompus (console) | `export LC_ALL=en_US.UTF-8` |
| Port 8080 occupé | `./moteur_morphologique --server --port 9000` |
| `index.html` non trouvé | Lancer depuis le répertoire `moteur-morphologique-arabe/` |

---

## 🎓 Difficultés Techniques et Solutions

**Encodage UTF-8** : Les caractères arabes occupent 3 octets ; `string[i]` naïf corrompt les données. Solution : parser UTF-8 manuel détectant les séquences multi-octets via les bits de poids fort (`(c & 0xF0) == 0xE0`).

**Validation morphologique** : L'approche naïve (180ms) itérait sur 18 racines × 11 schèmes × génération = 198 opérations. L'index inversé pré-calculé réduit cela à un lookup O(1) en < 1ms (gain ×180).

---

## 🚀 Améliorations Possibles

- Racines quadrilitères (4 lettres)
- Diacritisation automatique complète
- Interface graphique native (Qt/GTK)
- Base de données SQLite pour grandes collections
- API REST documentée (Swagger/OpenAPI)
- Tests unitaires automatisés
- Règles morphologiques avancées (assimilation, gémination)

---

## 📚 Documentation Détaillée

| Fichier | Contenu |
|---------|---------|
| [RAPPORT_TECHNIQUE.md](RAPPORT_TECHNIQUE.md) | Structures, algorithmes, complexité (3 pages) |
| [SYNTHESE_TECHNIQUE.md](SYNTHESE_TECHNIQUE.md) | Vue d'ensemble avec diagrammes |
| [METRIQUES_PROJET.md](METRIQUES_PROJET.md) | Benchmarks et statistiques détaillées |

---

**Version** : 2.0 · **Dernière mise à jour** : 24 février 2026 · [Licence](LICENSE)
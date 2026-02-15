#include "HashTable.h"
#include <algorithm>

// Fonction de hachage utilisant l'algorithme djb2 adapté pour UTF-8
int HashTable::hash(const string& cle) const {
    unsigned long hash_val = 5381;
    for (unsigned char c : cle) {
        hash_val = ((hash_val << 5) + hash_val) + c; // hash * 33 + c
    }
    return hash_val % capacite;
}

// Constructeur
HashTable::HashTable(int cap) : capacite(cap), taille_actuelle(0) {
    buckets.resize(capacite, nullptr);
}

// Destructeur
HashTable::~HashTable() {
    vider();
}

// Vider la table
void HashTable::vider() {
    for (int i = 0; i < capacite; i++) {
        Noeud* courant = buckets[i];
        while (courant) {
            Noeud* temp = courant;
            courant = courant->suivant;
            delete temp;
        }
        buckets[i] = nullptr;
    }
    taille_actuelle = 0;
}

// Redimensionnement automatique
void HashTable::redimensionner() {
    int nouvelle_capacite = capacite * 2;
    vector<Noeud*> nouveaux_buckets(nouvelle_capacite, nullptr);
    
    // Réinsérer tous les éléments
    for (int i = 0; i < capacite; i++) {
        Noeud* courant = buckets[i];
        while (courant) {
            Noeud* suivant = courant->suivant;
            
            // Recalculer le hash avec la nouvelle capacité
            unsigned long hash_val = 5381;
            for (unsigned char c : courant->cle) {
                hash_val = ((hash_val << 5) + hash_val) + c;
            }
            int nouvel_index = hash_val % nouvelle_capacite;
            
            // Insérer en tête de la nouvelle liste
            courant->suivant = nouveaux_buckets[nouvel_index];
            nouveaux_buckets[nouvel_index] = courant;
            
            courant = suivant;
        }
    }
    
    buckets = nouveaux_buckets;
    capacite = nouvelle_capacite;
}

// Insertion ou mise à jour
void HashTable::inserer(const string& cle, const string& valeur) {
    int index = hash(cle);
    Noeud* courant = buckets[index];
    
    // Vérifier si la clé existe déjà (mise à jour)
    while (courant) {
        if (courant->cle == cle) {
            courant->valeur = valeur;
            return;
        }
        courant = courant->suivant;
    }
    
    // Insertion d'un nouvel élément en tête
    Noeud* nouveau = new Noeud(cle, valeur);
    nouveau->suivant = buckets[index];
    buckets[index] = nouveau;
    taille_actuelle++;
    
    // Redimensionner si facteur de charge > 0.75
    if (static_cast<double>(taille_actuelle) / capacite > 0.75) {
        redimensionner();
    }
}

// Recherche
string HashTable::rechercher(const string& cle) const {
    int index = hash(cle);
    Noeud* courant = buckets[index];
    
    while (courant) {
        if (courant->cle == cle) {
            return courant->valeur;
        }
        courant = courant->suivant;
    }
    
    return ""; // Clé non trouvée
}

// Vérifier l'existence d'une clé
bool HashTable::contient(const string& cle) const {
    int index = hash(cle);
    Noeud* courant = buckets[index];
    
    while (courant) {
        if (courant->cle == cle) {
            return true;
        }
        courant = courant->suivant;
    }
    
    return false;
}

// Suppression
void HashTable::supprimer(const string& cle) {
    int index = hash(cle);
    Noeud* courant = buckets[index];
    Noeud* precedent = nullptr;
    
    while (courant) {
        if (courant->cle == cle) {
            if (precedent) {
                precedent->suivant = courant->suivant;
            } else {
                buckets[index] = courant->suivant;
            }
            delete courant;
            taille_actuelle--;
            return;
        }
        precedent = courant;
        courant = courant->suivant;
    }
}

// Lister toutes les clés
vector<string> HashTable::lister_cles() const {
    vector<string> cles;
    cles.reserve(taille_actuelle);
    
    for (int i = 0; i < capacite; i++) {
        Noeud* courant = buckets[i];
        while (courant) {
            cles.push_back(courant->cle);
            courant = courant->suivant;
        }
    }
    
    return cles;
}

// Lister toutes les paires clé-valeur
vector<pair<string,string>> HashTable::lister_tout() const {
    vector<pair<string,string>> resultat;
    resultat.reserve(taille_actuelle);
    
    for (int i = 0; i < capacite; i++) {
        Noeud* courant = buckets[i];
        while (courant) {
            resultat.push_back({courant->cle, courant->valeur});
            courant = courant->suivant;
        }
    }
    
    return resultat;
}

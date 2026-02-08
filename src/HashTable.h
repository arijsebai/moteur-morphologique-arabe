#pragma once
#include <string>
#include <vector>
using namespace std;

// Table de hachage implémentée manuellement avec chaînage pour la gestion des collisions
class HashTable {
private:
    // Structure pour stocker les éléments dans la table de hachage
    struct Noeud {
        string cle;
        string valeur;
        Noeud* suivant;
        
        Noeud(const string& k, const string& v) : cle(k), valeur(v), suivant(nullptr) {}
    };
    
    vector<Noeud*> buckets;  // Tableau de listes chaînées
    int capacite;            // Taille de la table
    int taille_actuelle;     // Nombre d'éléments stockés
    
    // Fonction de hachage simple mais efficace
    int hash(const string& cle) const;
    
    // Redimensionnement automatique si facteur de charge > 0.75
    void redimensionner();

public:
    // Constructeur avec capacité initiale
    HashTable(int cap = 16);
    
    // Destructeur pour libérer la mémoire
    ~HashTable();
    
    // Insertion ou mise à jour d'un élément
    void inserer(const string& cle, const string& valeur);
    
    // Recherche d'un élément par sa clé
    string rechercher(const string& cle) const;
    
    // Vérifier si une clé existe
    bool contient(const string& cle) const;
    
    // Suppression d'un élément
    void supprimer(const string& cle);
    
    // Récupérer toutes les clés
    vector<string> lister_cles() const;
    
    // Récupérer toutes les paires clé-valeur
    vector<pair<string,string>> lister_tout() const;
    
    // Obtenir le nombre d'éléments
    int taille() const { return taille_actuelle; }
    
    // Vider la table
    void vider();
};

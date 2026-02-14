#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;

struct NoeudAVL {
    string racine;
    NoeudAVL* gauche;
    NoeudAVL* droite;
    int hauteur;
    vector<pair<string,string>> derives_valides;
    map<string, int> frequences;  // Fréquence d'apparition de chaque mot dérivé

    NoeudAVL(const string& r);
};

class AVL {
public:
    NoeudAVL* racine;
    AVL();
    int hauteur(NoeudAVL* node);
    int facteur_equilibre(NoeudAVL* node);
    NoeudAVL* rotation_gauche(NoeudAVL* x);
    NoeudAVL* rotation_droite(NoeudAVL* y);
    NoeudAVL* inserer(NoeudAVL* node, const string& rac);
    NoeudAVL* rechercher(NoeudAVL* node, const string& rac);
    void extraire_racines(NoeudAVL* node, vector<string>& racines);
    void afficher(NoeudAVL* node, int niveau=0);
    void incrementer_frequence(NoeudAVL* node, const string& mot_derive);
    int obtenir_frequence(NoeudAVL* node, const string& mot_derive);
};
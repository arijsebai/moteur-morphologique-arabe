#include "AVL.h"
#include <iostream>
#include <algorithm>
using namespace std;

NoeudAVL::NoeudAVL(const string& r): racine(r), gauche(nullptr), droite(nullptr), hauteur(1) {}

AVL::AVL(): racine(nullptr) {}

int AVL::hauteur(NoeudAVL* node) { return node ? node->hauteur : 0; }
int AVL::facteur_equilibre(NoeudAVL* node) { return node ? hauteur(node->gauche) - hauteur(node->droite) : 0; }

NoeudAVL* AVL::rotation_gauche(NoeudAVL* x) {
    NoeudAVL* y = x->droite;
    NoeudAVL* T2 = y->gauche;
    y->gauche = x;
    x->droite = T2;
    x->hauteur = 1 + max(hauteur(x->gauche), hauteur(x->droite));
    y->hauteur = 1 + max(hauteur(y->gauche), hauteur(y->droite));
    return y;
}

NoeudAVL* AVL::rotation_droite(NoeudAVL* y) {
    NoeudAVL* x = y->gauche;
    NoeudAVL* T2 = x->droite;
    x->droite = y;
    y->gauche = T2;
    y->hauteur = 1 + max(hauteur(y->gauche), hauteur(y->droite));
    x->hauteur = 1 + max(hauteur(x->gauche), hauteur(x->droite));
    return x;
}

NoeudAVL* AVL::inserer(NoeudAVL* node, const string& rac) {
    if(!node) return new NoeudAVL(rac);
    if(rac < node->racine) node->gauche = inserer(node->gauche,rac);
    else if(rac > node->racine) node->droite = inserer(node->droite,rac);
    else return node;

    node->hauteur = 1 + max(hauteur(node->gauche), hauteur(node->droite));
    int balance = facteur_equilibre(node);

    if(balance>1 && rac<node->gauche->racine) return rotation_droite(node);
    if(balance<-1 && rac>node->droite->racine) return rotation_gauche(node);
    if(balance>1 && rac>node->gauche->racine){ node->gauche=rotation_gauche(node->gauche); return rotation_droite(node);}
    if(balance<-1 && rac<node->droite->racine){ node->droite=rotation_droite(node->droite); return rotation_gauche(node);}
    return node;
}

NoeudAVL* AVL::rechercher(NoeudAVL* node,const string& rac){
    if(!node) return nullptr;
    if(rac==node->racine) return node;
    if(rac<node->racine) return rechercher(node->gauche,rac);
    return rechercher(node->droite,rac);
}

void AVL::extraire_racines(NoeudAVL* node, vector<string>& racines){
    if(!node) return;
    extraire_racines(node->gauche,racines);
    racines.push_back(node->racine);
    extraire_racines(node->droite,racines);
}

void AVL::afficher(NoeudAVL* node,int niveau){
    if(!node) return;
    afficher(node->droite,niveau+1);
    for(int i=0;i<niveau;i++) cout<<"    ";
    cout<<"-> "<<node->racine<<endl;
    afficher(node->gauche,niveau+1);
}

void AVL::incrementer_frequence(NoeudAVL* node, const string& mot_derive) {
    if(!node) return;
    node->frequences[mot_derive]++;
}

int AVL::obtenir_frequence(NoeudAVL* node, const string& mot_derive) {
    if(!node) return 0;
    if(node->frequences.count(mot_derive)) {
        return node->frequences[mot_derive];
    }
    return 0;
}
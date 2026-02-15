#pragma once
#include "AVL.h"
#include "TableSchemes.h"
#include <map>
#include <string>
#include <unordered_map>
using namespace std;

extern map<string,map<string,string>> verbes_irreguliers;
extern map<string,pair<string,string>> mot_to_racine_scheme;
extern unordered_map<string,string> cache_simplification;  // Cache pour simplifier()

string simplifier(const string& mot);
vector<pair<string,string>> generer_derives(NoeudAVL* noeud, TableSchemes& table);
pair<string,string> verifier_mot(const string& mot, AVL& arbre, TableSchemes& table);
pair<bool,string> verifier_mot_racine(const string& mot, const string& racine, TableSchemes& table, AVL& arbre);
void construire_index(AVL& arbre, TableSchemes& table);
void reinitialiser_index(AVL& arbre, TableSchemes& table);
#pragma once
#include "AVL.h"
#include "TableSchemes.h"
#include <map>
#include <string>
using namespace std;

extern map<string,map<string,string>> verbes_irreguliers;
extern map<string,pair<string,string>> mot_to_racine_scheme;

string simplifier(const string& mot);
vector<pair<string,string>> generer_derives(NoeudAVL* noeud, TableSchemes& table);
pair<string,string> verifier_mot(const string& mot, AVL& arbre, TableSchemes& table);
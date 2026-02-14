#pragma once
#include <string>
#include <vector>
#include "HashTable.h"
using namespace std;

class TableSchemes {
private:
    HashTable schemes;  // Table de hachage implémentée manuellement
public:
    TableSchemes();
    void ajouter(const string& nom, const string& regle);
    void modifier(const string& nom, const string& nouvelle_regle);
    void supprimer(const string& nom);
    bool contient(const string& nom);
    string obtenir(const string& nom);
    string obtenir_regle(const string& nom);
    vector<string> lister();
    vector<pair<string,string>> lister_detail();
    bool charger_depuis_fichier(const string& path);
    bool sauvegarder_vers_fichier(const string& path);
};
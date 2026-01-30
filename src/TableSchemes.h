#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;

class TableSchemes {
public:
    map<string,string> schemes;
    TableSchemes();
    void ajouter(const string& nom, const string& regle);
    void supprimer(const string& nom);
    vector<string> lister();
};
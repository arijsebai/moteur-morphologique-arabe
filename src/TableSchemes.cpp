#include "TableSchemes.h"
#include <fstream>
#include <sstream>

TableSchemes::TableSchemes() : schemes(32) {  // Capacité initiale de 32
    string liste[] = {"فعل","فاعل","مفعول","افتعل","تفعيل","مفعل","استفعال","فعّل","مستفعل","انفعل","تفاعل","فعول"};
    for(auto& s: liste) {
        schemes.inserer(s, s);
    }
}

void TableSchemes::ajouter(const string& nom, const string& regle) { 
    schemes.inserer(nom, regle); 
}

void TableSchemes::modifier(const string& nom, const string& nouvelle_regle) {
    schemes.inserer(nom, nouvelle_regle);  // inserer() met à jour si la clé existe
}

void TableSchemes::supprimer(const string& nom) { 
    schemes.supprimer(nom); 
}

bool TableSchemes::contient(const string& nom) {
    return schemes.contient(nom);
}

string TableSchemes::obtenir(const string& nom) {
    return schemes.rechercher(nom);
}

string TableSchemes::obtenir_regle(const string& nom) {
    return schemes.rechercher(nom);
}

vector<string> TableSchemes::lister() { 
    return schemes.lister_cles(); 
}

vector<pair<string,string>> TableSchemes::lister_detail() {
    return schemes.lister_tout();
}

static string nettoyer_ligne(const string& line){
    if(!line.empty() && line.back() == '\r') return line.substr(0, line.size() - 1);
    return line;
}

bool TableSchemes::charger_depuis_fichier(const string& path){
    ifstream f(path);
    if(!f) return false;

    vector<pair<string,string>> items;
    string ligne;
    while(getline(f, ligne)){
        ligne = nettoyer_ligne(ligne);
        if(ligne.empty()) continue;
        size_t sep = ligne.find('|');
        string nom = (sep == string::npos) ? ligne : ligne.substr(0, sep);
        string regle = (sep == string::npos) ? ligne : ligne.substr(sep + 1);
        if(nom.empty()) continue;
        if(regle.empty()) regle = nom;
        items.push_back({nom, regle});
    }

    if(items.empty()) return false;

    schemes.vider();
    for(const auto& p : items){
        schemes.inserer(p.first, p.second);
    }
    return true;
}

bool TableSchemes::sauvegarder_vers_fichier(const string& path){
    ofstream f(path, ios::trunc);
    if(!f) return false;
    auto items = lister_detail();
    for(const auto& p : items){
        f << p.first << "|" << p.second << "\n";
    }
    return true;
}
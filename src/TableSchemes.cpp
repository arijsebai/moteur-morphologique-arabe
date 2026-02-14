#include "TableSchemes.h"

TableSchemes::TableSchemes() : schemes(32) {  // Capacité initiale de 32
    string liste[] = {"فعل","فاعل","مفعول","افتعل","تفعيل","مفعل","استفعال","فعّل","مستفعل","انفعل","تفاعل","فعول"};
    for(auto& s: liste) {
        schemes.inserer(s, "فعل");
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

vector<string> TableSchemes::lister() { 
    return schemes.lister_cles(); 
}
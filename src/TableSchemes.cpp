#include "TableSchemes.h"

TableSchemes::TableSchemes(){
    string liste[] = {"فاعل","مفعول","افتعل","تفعيل","مفعل","استفعال","افعل","فعّل","مستفعل","انفعل","تفاعل","افعلل","فعول","افعلّ"};
    for(auto& s: liste) schemes[s]="فعل";
}

void TableSchemes::ajouter(const string& nom,const string& regle){ schemes[nom]=regle; }
void TableSchemes::supprimer(const string& nom){ if(schemes.count(nom)) schemes.erase(nom); }
vector<string> TableSchemes::lister(){ vector<string> res; for(auto& p: schemes) res.push_back(p.first); return res; }
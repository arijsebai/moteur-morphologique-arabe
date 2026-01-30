#include "Morphologie.h"
#include <algorithm>

map<string,map<string,string>> verbes_irreguliers = {
    {"قرأ", {{"فعل","قرأ"},{"مفعول","مَقْرُوء"},{"فاعل","قارئ"}}},
    {"أخذ", {{"فعل","أخذ"},{"مفعول","مُؤْخَذ"},{"فاعل","آخذ"}}},
    {"قال", {{"فعل","قال"},{"مفعول","مَقُول"},{"فاعل","قائل"}}}
};

map<string,pair<string,string>> mot_to_racine_scheme;

static vector<string> split_utf8(const string& text){
    vector<string> out;
    for(size_t i=0;i<text.size();){
        unsigned char c = static_cast<unsigned char>(text[i]);
        size_t len = 1;
        if((c & 0x80) == 0x00) len = 1;
        else if((c & 0xE0) == 0xC0) len = 2;
        else if((c & 0xF0) == 0xE0) len = 3;
        else if((c & 0xF8) == 0xF0) len = 4;
        out.push_back(text.substr(i, len));
        i += len;
    }
    return out;
}

static void replace_all(string& text, const string& from, const string& to){
    if(from.empty()) return;
    size_t start_pos = 0;
    while((start_pos = text.find(from, start_pos)) != string::npos){
        text.replace(start_pos, from.size(), to);
        start_pos += to.size();
    }
}

static string appliquer_scheme(const string& scheme, const string& racine){
    auto letters = split_utf8(racine);
    if(letters.size() < 3) return scheme;
    string mot = scheme;
    replace_all(mot, "ف", letters[0]);
    replace_all(mot, "ع", letters[1]);
    replace_all(mot, "ل", letters[2]);
    return mot;
}

string simplifier(const string& mot){
    string res = mot;
    vector<string> diacritiques = {
        "\u064B", "\u064C", "\u064D", "\u064E", "\u064F", "\u0650", "\u0651", "\u0652", "\u0670"
    };
    for(const auto& d : diacritiques){
        replace_all(res, d, "");
    }
    return res;
}

vector<pair<string,string>> generer_derives(NoeudAVL* noeud, TableSchemes& table){
    if(!noeud) return {};
    if(!noeud->derives_valides.empty()) return noeud->derives_valides;
    vector<pair<string,string>> derives;
    if(verbes_irreguliers.count(noeud->racine)){
        for(auto& p: verbes_irreguliers[noeud->racine]) derives.push_back(p);
    } else {
        for(auto& sch: table.lister()){
            string mot = appliquer_scheme(sch, noeud->racine);
            derives.push_back({sch, mot});
        }
    }
    noeud->derives_valides=derives;
    return derives;
}

pair<string,string> verifier_mot(const string& mot, AVL& arbre, TableSchemes& table){
    string mot_simpl=simplifier(mot);
    if(mot_to_racine_scheme.count(mot_simpl)) return mot_to_racine_scheme[mot_simpl];
    vector<string> racines;
    arbre.extraire_racines(arbre.racine,racines);
    for(auto& r: racines){
        NoeudAVL* n=arbre.rechercher(arbre.racine,r);
        auto derives=generer_derives(n,table);
        for(auto& p: derives){
            if(simplifier(p.second)==mot_simpl) return {r,p.first};
        }
    }
    return {"",""};
}
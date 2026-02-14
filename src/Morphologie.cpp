#include "Morphologie.h"
#include <algorithm>

map<string,map<string,string>> verbes_irreguliers = {
    {"قرأ", {{"فعل","قرأ"},{"مفعول","مَقْرُوء"},{"فاعل","قارئ"}}},
    {"قال", {{"فعل","قال"},{"مفعول","مَقُول"},{"فاعل","قائل"}}}
};

map<string,pair<string,string>> mot_to_racine_scheme;
// Index avec priorité : conserver le 1er schème trouvé (schèmes prioritaires d'abord)
map<string, vector<pair<string,string>>> mot_to_racine_schemes; // Pour gérer les ambiguïtés
unordered_map<string,string> cache_simplification;  // Cache pour performances

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

// Fonction pour appliquer les règles de grammaire arabe (assimilation phonétique)
static string appliquer_regles_grammaire(string mot, const string& premiere_lettre){
    // Règle : Quand une racine commence par hamza (أ) et un schème commence par voyelle
    // Remplacer "أا" ou "إا" ou "اأ" par "آ" (alif-madda)
    if(premiere_lettre == "أ"){  // Racine commence par hamza
        replace_all(mot, "أا", "آ");  // أا → آ
        replace_all(mot, "اأ", "آ");  // اأ → آ (cas inverse)
        replace_all(mot, "إا", "آ");  // إا → آ (hamza avec kasra)
    }
    return mot;
}

static string appliquer_scheme(const string& scheme, const string& racine){
    auto letters = split_utf8(racine);
    if(letters.size() < 3) return scheme;
    string mot = scheme;
    
    // Utiliser des marqueurs temporaires pour éviter les remplacements multiples
    // Ceci évite le problème quand une lettre de la racine apparaît dans le schème
    replace_all(mot, "ف", "\x01");  // Marqueur temporaire 1
    replace_all(mot, "ع", "\x02");  // Marqueur temporaire 2
    replace_all(mot, "ل", "\x03");  // Marqueur temporaire 3
    
    // Maintenant remplacer les marqueurs par les vraies lettres
    replace_all(mot, "\x01", letters[0]);
    replace_all(mot, "\x02", letters[1]);
    replace_all(mot, "\x03", letters[2]);
    
    // Appliquer les règles grammaticales arabes
    mot = appliquer_regles_grammaire(mot, letters[0]);
    
    return mot;
}

string simplifier(const string& mot){
    // Cache pour éviter de simplifier plusieurs fois le même mot
    auto it = cache_simplification.find(mot);
    if(it != cache_simplification.end()) return it->second;
    
    string res = mot;
    vector<string> diacritiques = {
        "\u064B", "\u064C", "\u064D", "\u064E", "\u064F", "\u0650", "\u0651", "\u0652", "\u0670"
    };
    for(const auto& d : diacritiques){
        replace_all(res, d, "");
    }
    
    cache_simplification[mot] = res;  // Mettre en cache
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
    
    // Mettre à jour l'index inversé pour accès O(1)
    // Garder seulement le PREMIER schème pour éviter les écrasements
    for(auto& p: derives){
        string mot_simpl = simplifier(p.second);
        // Ne mettre à jour que si ce mot n'existe pas encore (1er schème = prioritaire)
        if(mot_to_racine_scheme.find(mot_simpl) == mot_to_racine_scheme.end()) {
            mot_to_racine_scheme[mot_simpl] = {noeud->racine, p.first};
        }
    }
    
    return derives;
}

// Construire l'index inversé pour toutes les racines
void construire_index(AVL& arbre, TableSchemes& table){
    vector<string> racines;
    arbre.extraire_racines(arbre.racine, racines);
    for(auto& r: racines){
        NoeudAVL* n = arbre.rechercher(arbre.racine, r);
        if(n) generer_derives(n, table);  // Génère et indexe automatiquement
    }
}

pair<string,string> verifier_mot(const string& mot, AVL& arbre, TableSchemes& /*table*/){
    string mot_simpl = simplifier(mot);
    
    // Recherche O(1) dans l'index
    if(mot_to_racine_scheme.count(mot_simpl)) {
        auto& result = mot_to_racine_scheme[mot_simpl];
        NoeudAVL* n = arbre.rechercher(arbre.racine, result.first);
        if(n) arbre.incrementer_frequence(n, mot);
        return result;
    }
    
    return {"",""};
}
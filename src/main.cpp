#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <clocale>
#include <locale>
#include "core/AVL.h"
#include "core/TableSchemes.h"
#include "core/Morphologie.h"
#include "cli/CLIHandler.h"
#include "cli/Localization.h"
#include "cli/UI.h"
#include "api/HttpServer.h"
#include "data/DataLoader.h"
#include "utils/Colors.h"
#include "cli/UI.h"

using namespace std;

int main(int argc, char** argv){
    // Set up locale for UTF-8 support
    setlocale(LC_ALL, "");
    try {
        std::locale::global(std::locale(""));
    } catch (...) {
        // Fallback if system locale not available
    }

    // Initialize data structures
    AVL arbre;
    TableSchemes table;
    
    // Load data
    if(!table.charger_depuis_fichier("data/schemes.txt")){
        table.sauvegarder_vers_fichier("data/schemes.txt");
    }
    load_roots("data/racines.txt", arbre);
    
    // Build inverted index for optimization
    construire_index(arbre, table);

    // Initialize irregular verbs - forcer فعل en premier pour chaque racine
    for(auto& [rac, formes]: verbes_irreguliers) {
        // D'abord insérer فعل s'il existe
        if(formes.count("فعل")) {
            mot_to_racine_scheme[simplifier(formes.at("فعل"))] = {rac, "فعل"};
        }
        // Puis les autres schèmes
        for(auto& [sch, mot]: formes) {
            if(sch != "فعل") {
                string mot_simpl = simplifier(mot);
                // Ne pas écraser si déjà présent
                if(mot_to_racine_scheme.find(mot_simpl) == mot_to_racine_scheme.end()) {
                    mot_to_racine_scheme[mot_simpl] = {rac, sch};
                }
            }
        }
    }

    // Parse command line arguments
    bool server_mode = false;
    int port = 8080;
    for(int i=1;i<argc;i++){
        string arg = argv[i];
        if(arg == "--server") server_mode = true;
        else if(arg == "--port" && i+1 < argc) port = stoi(argv[++i]);
        else if(arg == "--ar" || arg == "--arabic") USE_ARABIC = true;
        else if(arg == "--fr" || arg == "--french") USE_ARABIC = false;
    }

    // Run in server mode or CLI mode
    if(server_mode){
        run_server(port, arbre, table);
        return 0;
    }

    // Interactive CLI mode
    if(argc == 1){
        cout << C_CYAN << "Sélectionnez la langue / اختر اللغة" << C_RESET << "\n";
        cout << "[1] Français\n";
        cout << "[2] العربية\n";
        cout << C_YELLOW << "Choix / الاختيار" << C_RESET << " : ";
        string choix_lang; cin >> choix_lang;
        USE_ARABIC = (choix_lang == "2");
    }

    while(true){
        menu();
        string choix; cin>>choix;
        if(choix=="1"){
            while(true){
                menu_racines();
                string sous; cin>>sous;
                if(sous=="1"){
                    vector<string> racines;
                    arbre.extraire_racines(arbre.racine, racines);
                    cout << C_CYAN << "\n📚 Racines disponibles (" << racines.size() << ")" << C_RESET << "\n";
                    print_rule();
                    if(racines.empty()){
                        cout << C_DIM << "Aucune racine" << C_RESET << "\n";
                    } else {
                        for(size_t i=0; i<racines.size(); i++){
                            cout << "  " << (i+1) << ". " << racines[i] << "\n";
                        }
                    }
                    print_rule();
                }
                else if(sous=="2"){
                    cout << C_YELLOW << "Nouvelle racine" << C_RESET << " : ";
                    string r; cin>>r;
                    if(!arbre.rechercher(arbre.racine, r)){
                        arbre.racine=arbre.inserer(arbre.racine,r);
                        ofstream f("data/racines.txt",ios::app); f<<r<<"\n";
                        cout << C_GREEN << "✅ Racine ajoutée" << C_RESET << "\n";
                    } else {
                        cout << C_DIM << "Racine déjà existante" << C_RESET << "\n";
                    }
                }
                else if(sous=="0"){
                    break;
                }
                else {
                    cout << C_RED << "❌ Choix invalide" << C_RESET << "\n";
                }
            }
        }
        else if(choix=="2"){
            while(true){
                menu_schemes();
                string sous; cin>>sous;
                if(sous=="1"){
                    vector<string> schemes = table.lister();
                    cout << C_CYAN << "\n📐 Schèmes disponibles (" << schemes.size() << ")" << C_RESET << "\n";
                    print_rule();
                    if(schemes.empty()){
                        cout << C_DIM << "Aucun schème" << C_RESET << "\n";
                    } else {
                        for(size_t i=0; i<schemes.size(); i++){
                            cout << "  " << (i+1) << ". " << schemes[i] << "\n";
                        }
                    }
                    print_rule();
                }
                else if(sous=="2"){
                    cout << C_YELLOW << "Nouveau schème" << C_RESET << " : ";
                    string sch; cin>>sch;
                    if(!table.contient(sch)){
                        table.ajouter(sch, "فعل");
                        ofstream f("data/schemes.txt",ios::app); f<<sch<<"\n";
                        cout << C_GREEN << "✅ Schème ajouté" << C_RESET << "\n";
                    } else {
                        cout << C_DIM << "Schème déjà existant" << C_RESET << "\n";
                    }
                }
                else if(sous=="0"){
                    break;
                }
                else {
                    cout << C_RED << "❌ Choix invalide" << C_RESET << "\n";
                }
            }
        }
        else if(choix=="3"){
            cout << C_YELLOW << "Racine" << C_RESET << " : ";
            string r; cin>>r;
            NoeudAVL* n=arbre.rechercher(arbre.racine,r);
            if(n){
                auto derives=generer_derives(n,table);
                cout << C_CYAN << "\n📊 Dérivés de la racine: " << r << C_RESET << "\n";
                print_rule();
                for(auto& p: derives) cout << "  " << p.first << " → " << p.second << "\n";
                print_rule();
            } else {
                cout << C_RED << "❌ Racine non trouvée" << C_RESET << "\n";
            }
        }
        else if(choix=="4"){
            cout << C_YELLOW << "Mot" << C_RESET << " : ";
            string m; cin>>m;
            auto res=verifier_mot(m,arbre,table);
            if(!res.first.empty()) {
                cout << C_GREEN << "\n✅ Mot valide" << C_RESET << "\n";
                print_rule();
                cout << "Mot    : " << m << "\n";
                cout << "Racine : " << res.first << "\n";
                cout << "Schème : " << res.second << "\n";
                print_rule();
            }
            else {
                cout << C_RED << "❌ Mot introuvable" << C_RESET << "\n";
                cout << C_DIM << "Aucune racine connue ne correspond." << C_RESET << "\n";
            }
        }
        else if(choix=="5") {
            cout << C_GREEN << "Au revoir !" << C_RESET << "\n";
            break;
        }
        else {
            cout << C_RED << "❌ Choix invalide" << C_RESET << "\n";
        }
    }
}
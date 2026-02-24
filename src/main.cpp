#include <iostream>
#include <clocale>
#include <locale>
#include "core/AVL.h"
#include "core/TableSchemes.h"
#include "core/Morphologie.h"
#include "cli/CLIHandler.h"
#include "cli/Localization.h"
#include "api/HttpServer.h"
#include "data/DataLoader.h"
#include "utils/Colors.h"

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

    // Initialize irregular verbs
    for(auto& [rac, formes]: verbes_irreguliers)
        for(auto& [sch, mot]: formes)
            mot_to_racine_scheme[simplifier(mot)]={rac,sch};

    // Parse command line arguments
    bool server_mode = false;
    int port = 8080;
    for(int i=1; i<argc; i++){
        string arg = argv[i];
        if(arg == "--server") 
            server_mode = true;
        else if(arg == "--port" && i+1 < argc) 
            port = stoi(argv[++i]);
        else if(arg == "--ar" || arg == "--arabic") 
            USE_ARABIC = true;
        else if(arg == "--fr" || arg == "--french") 
            USE_ARABIC = false;
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
        string choix_lang; 
        cin >> choix_lang;
        USE_ARABIC = (choix_lang == "2");
    }

    CLIHandler cli(arbre, table);
    cli.run();
    
    return 0;
}

#include "CLIHandler.h"
#include "UI.h"
#include "Localization.h"
#include "../utils/Colors.h"
#include "../data/DataLoader.h"
#include "../core/Morphologie.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

CLIHandler::CLIHandler(AVL& arbre, TableSchemes& table) : arbre(arbre), table(table) {}

void CLIHandler::handleAnalyzeWord(){
    cout << C_YELLOW << txt("Mot à analyser", "الكلمة للتحليل") << C_RESET << " : ";
    string m; cin >> m;
    auto res = verifier_mot(m, arbre, table);
    if(!res.first.empty()) {
        cout << C_GREEN << "\n✅ " << txt("Mot valide", "كلمة صحيحة") << C_RESET << "\n";
        print_rule();
        cout << txt("Mot    ", "الكلمة  ") << ": " << m << "\n";
        cout << txt("Racine ", "الجذر   ") << ": " << res.first << "\n";
        cout << txt("Schème ", "الوزن   ") << ": " << res.second << "\n";
        print_rule();
    }
    else {
        cout << C_RED << "❌ " << txt("Mot introuvable", "كلمة غير موجودة") << C_RESET << "\n";
        cout << C_DIM << txt("Aucune racine connue ne correspond.", "لا يوجد جذر معروف يطابق.") << C_RESET << "\n";
    }
}

void CLIHandler::handleManageRoots(){
    while(true){
        menu_racines();
        string sous; cin >> sous;
        if(sous == "1"){
            vector<string> racines;
            arbre.extraire_racines(arbre.racine, racines);
            cout << C_CYAN << "\n📚 " << txt("Racines disponibles", "الجذور المتاحة") << " (" << racines.size() << ")" << C_RESET << "\n";
            print_rule();
            if(racines.empty()){
                cout << C_DIM << txt("Aucune racine", "لا توجد جذور") << C_RESET << "\n";
            } else {
                for(size_t i=0; i<racines.size(); i++){
                    cout << "  " << (i+1) << ". " << racines[i] << "\n";
                }
            }
            print_rule();
        }
        else if(sous == "2"){
            cout << C_YELLOW << txt("Nouvelle racine", "جذر جديد") << C_RESET << " : ";
            string r; cin >> r;
            if(!arbre.rechercher(arbre.racine, r)){
                arbre.racine = arbre.inserer(arbre.racine, r);
                ofstream f("data/racines.txt", ios::app); 
                f << r << "\n";
                cout << C_GREEN << "✅ " << txt("Racine ajoutée", "تم إضافة الجذر") << C_RESET << "\n";
            } else {
                cout << C_DIM << txt("Racine déjà existante", "الجذر موجود مسبقاً") << C_RESET << "\n";
            }
        }
        else if(sous == "0"){
            break;
        }
        else {
            cout << C_RED << "❌ " << txt("Choix invalide", "اختيار غير صحيح") << C_RESET << "\n";
        }
    }
}

void CLIHandler::handleManageSchemes(){
    while(true){
        menu_schemes();
        string sous; cin >> sous;
        if(sous == "1"){
            auto schemes = table.lister_detail();
            cout << C_CYAN << "\n📐 " << txt("Schèmes disponibles", "الأوزان المتاحة") << " (" << schemes.size() << ")" << C_RESET << "\n";
            print_rule();
            if(schemes.empty()){
                cout << C_DIM << txt("Aucun schème", "لا توجد أوزان") << C_RESET << "\n";
            } else {
                for(size_t i=0; i<schemes.size(); i++){
                    cout << "  " << (i+1) << ". " << schemes[i].first << "  →  " << schemes[i].second << "\n";
                }
            }
            print_rule();
        }
        else if(sous == "2"){
            cout << C_YELLOW << txt("Nouveau schème", "وزن جديد") << C_RESET << " : ";
            string sch; cin >> sch;
            if(!table.contient(sch)){
                cout << C_YELLOW << txt("Règle", "القاعدة") << C_RESET << " : ";
                string regle; cin >> regle;
                if(regle.empty()) regle = sch;
                table.ajouter(sch, regle);
                table.sauvegarder_vers_fichier("data/schemes.txt");
                reinitialiser_index(arbre, table);
                cout << C_GREEN << "✅ " << txt("Schème ajouté", "تم إضافة الوزن") << C_RESET << "\n";
            } else {
                cout << C_DIM << txt("Schème déjà existant", "الوزن موجود مسبقاً") << C_RESET << "\n";
            }
        }
        else if(sous == "3"){
            cout << C_YELLOW << txt("Schème à modifier", "الوزن للتعديل") << C_RESET << " : ";
            string sch; cin >> sch;
            if(!table.contient(sch)){
                cout << C_DIM << txt("Schème introuvable", "الوزن غير موجود") << C_RESET << "\n";
            } else {
                cout << C_YELLOW << txt("Nouvelle règle", "القاعدة الجديدة") << C_RESET << " : ";
                string regle; cin >> regle;
                if(regle.empty()) regle = sch;
                table.modifier(sch, regle);
                table.sauvegarder_vers_fichier("data/schemes.txt");
                reinitialiser_index(arbre, table);
                cout << C_GREEN << "✅ " << txt("Schème modifié", "تم تعديل الوزن") << C_RESET << "\n";
            }
        }
        else if(sous == "4"){
            cout << C_YELLOW << txt("Schème à supprimer", "الوزن للحذف") << C_RESET << " : ";
            string sch; cin >> sch;
            if(!table.contient(sch)){
                cout << C_DIM << txt("Schème introuvable", "الوزن غير موجود") << C_RESET << "\n";
            } else {
                table.supprimer(sch);
                table.sauvegarder_vers_fichier("data/schemes.txt");
                reinitialiser_index(arbre, table);
                cout << C_GREEN << "✅ " << txt("Schème supprimé", "تم حذف الوزن") << C_RESET << "\n";
            }
        }
        else if(sous == "0"){
            break;
        }
        else {
            cout << C_RED << "❌ " << txt("Choix invalide", "اختiار غير صحيح") << C_RESET << "\n";
        }
    }
}

void CLIHandler::handleGenerateDerivatives(){
    cout << C_YELLOW << txt("Racine", "الجذر") << C_RESET << " : ";
    string r; cin >> r;
    NoeudAVL* n = arbre.rechercher(arbre.racine, r);
    if(n){
        auto all_derives = generer_derives(n, table);
        cout << C_CYAN << "\n🔄 " << txt("Options de génération", "خيارات التوليد") << C_RESET << "\n";
        print_rule();
        cout << C_BLUE << "[1]" << C_RESET << " " << txt("Tous les dérivés", "جميع المشتقات") << " (" << all_derives.size() << ")\n";
        cout << C_BLUE << "[2]" << C_RESET << " " << txt("Sélectionner des schèmes", "اختيار أوزان محددة") << "\n";
        cout << C_BLUE << "[0]" << C_RESET << " " << txt("Annuler", "إلغاء") << "\n";
        print_rule();
        cout << C_YELLOW << txt("Choix", "الاختيار") << C_RESET << " : ";
        string opt_choix; cin >> opt_choix;
        
        vector<pair<string,string>> derives_to_show;
        
        if(opt_choix == "1"){
            derives_to_show = all_derives;
        } else if(opt_choix == "2"){
            auto schemes_list = table.lister();
            cout << C_CYAN << "\n📐 " << txt("Schèmes disponibles", "الأوزان المتاحة") << C_RESET << "\n";
            print_rule();
            for(size_t i=0; i<schemes_list.size(); i++){
                cout << "  " << (i+1) << ". " << schemes_list[i] << "\n";
            }
            print_rule();
            cout << C_YELLOW << txt("Entrez les numéros des schèmes (séparés par espace, 0 pour tous)", 
                                  "أدخل أرقام الأوزان (مفصولة بمسافة، 0 للجميع)") << C_RESET << " : ";
            cin.ignore();
            string line;
            getline(cin, line);
            istringstream iss(line);
            vector<int> indices;
            int idx;
            while(iss >> idx){
                if(idx == 0) {
                    derives_to_show = all_derives;
                    break;
                }
                indices.push_back(idx-1);
            }
            
            if(derives_to_show.empty() && !indices.empty()){
                vector<string> selected_schemes;
                for(int i : indices){
                    if(i >= 0 && i < static_cast<int>(schemes_list.size())){
                        selected_schemes.push_back(schemes_list[i]);
                    }
                }
                for(const auto& d : all_derives){
                    if(find(selected_schemes.begin(), selected_schemes.end(), d.first) != selected_schemes.end()){
                        derives_to_show.push_back(d);
                    }
                }
            }
        } else {
            return;
        }
        
        if(!derives_to_show.empty()){
            cout << C_CYAN << "\n📊 " << txt("Dérivés de la racine", "مشتقات الجذر") << ": " << r 
                 << " (" << derives_to_show.size() << ")" << C_RESET << "\n";
            print_rule();
            for(size_t i=0; i<derives_to_show.size(); i++){
                cout << "  " << (i+1) << ". " << derives_to_show[i].first << " → " << derives_to_show[i].second << "\n";
            }
            print_rule();
        } else {
            cout << C_DIM << txt("Aucun dérivé sélectionné", "لا توجد مشتقات محددة") << C_RESET << "\n";
        }
    } else {
        cout << C_RED << "❌ " << txt("Racine non trouvée", "الجذر غير موجود") << C_RESET << "\n";
    }
}

void CLIHandler::handleVerifyDerivation(){
    cout << C_YELLOW << txt("Mot à vérifier", "الكلمة للتحقق") << C_RESET << " : ";
    string m; cin >> m;
    cout << C_YELLOW << txt("Racine", "الجذر") << C_RESET << " : ";
    string r; cin >> r;
    
    auto res = verifier_mot_racine(m, r, table, arbre);
    
    if(res.first){
        cout << C_GREEN << "\n✅ " << txt("Mot valide - dérive de cette racine", "كلمة صحيحة - مشتقة من هذا الجذر") << C_RESET << "\n";
        print_rule();
        cout << txt("Mot    ", "الكلمة  ") << ": " << m << "\n";
        cout << txt("Racine ", "الجذر   ") << ": " << r << "\n";
        cout << txt("Schème ", "الوزن   ") << ": " << res.second << "\n";
        print_rule();
    } else {
        cout << C_RED << "❌ " << txt("Le mot ne dérive pas de cette racine", "الكلمة لا تشتق من هذا الجذر") << C_RESET << "\n";
        
        auto res_mot = verifier_mot(m, arbre, table);
        if(!res_mot.first.empty() && res_mot.first != r){
            cout << C_CYAN << txt("💡 Suggestion: ce mot dérive de", "💡 اقتراح: هذه الكلمة مشتقة من") << " '" 
                 << res_mot.first << "' " << txt("avec le schème", "بالوزن") << " '" << res_mot.second << "'\n";
        }
    }
}

void CLIHandler::run(){
    while(true){
        menu();
        string choix; cin >> choix;
        if(choix == "1"){
            handleAnalyzeWord();
        }
        else if(choix == "2"){
            handleManageRoots();
        }
        else if(choix == "3"){
            handleManageSchemes();
        }
        else if(choix == "4"){
            handleGenerateDerivatives();
        }
        else if(choix == "5"){
            handleVerifyDerivation();
        }
        else if(choix == "6") {
            cout << C_GREEN << txt("Au revoir !", "وداعاً!") << C_RESET << "\n";
            break;
        }
        else {
            cout << C_RED << "❌ " << txt("Choix invalide", "اختيار غير صحيح") << C_RESET << "\n";
        }
    }
}

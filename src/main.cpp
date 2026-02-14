#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <clocale>
#include <locale>
#include <cstdlib>
#include <algorithm>
#ifdef _WIN32
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif
#include "AVL.h"
#include "TableSchemes.h"
#include "Morphologie.h"
using namespace std;

#ifdef _WIN32
using socket_t = SOCKET;
static bool init_sockets(){
    WSADATA wsa{};
    return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
}
static void cleanup_sockets(){ WSACleanup(); }
static void close_socket(socket_t s){ closesocket(s); }
#else
using socket_t = int;
static bool init_sockets(){ return true; }
static void cleanup_sockets(){}
static void close_socket(socket_t s){ close(s); }
#endif

void charger_racines(const string& fichier, AVL& arbre){
    ifstream f(fichier);
    string ligne;
    while(getline(f,ligne))
        if(!ligne.empty()) arbre.racine=arbre.inserer(arbre.racine,ligne);
}

const string C_RESET = "\033[0m";
const string C_BOLD = "\033[1m";
const string C_DIM = "\033[2m";
const string C_BLUE = "\033[34m";
const string C_GREEN = "\033[32m";
const string C_YELLOW = "\033[33m";
const string C_RED = "\033[31m";
const string C_CYAN = "\033[36m";

bool USE_ARABIC = false;

string txt(const string& fr, const string& ar){
    return USE_ARABIC ? ar : fr;
}

void print_rule(){
    cout << C_DIM << "════════════════════════════════════════" << C_RESET << "\n";
}

void menu(){
    cout << "\n";
    print_rule();
    cout << C_BOLD << C_CYAN << txt("Moteur Morphologique Arabe", "محرك التحليل الصرفي العربي") << C_RESET << "\n";
    print_rule();
    cout << C_BLUE << "[1]" << C_RESET << " " << txt("� Analyser un mot", "🔍 تحليل كلمة") << "\n";
    cout << C_BLUE << "[2]" << C_RESET << " " << txt("📚 Gestion des racines", "📚 إدارة الجذور") << "\n";
    cout << C_BLUE << "[3]" << C_RESET << " " << txt("📐 Gestion des schèmes", "📐 إدارة الأوزان") << "\n";
    cout << C_BLUE << "[4]" << C_RESET << " " << txt("🔄 Générer dérivés d'une racine", "🔄 توليد مشتقات جذر") << "\n";
    cout << C_BLUE << "[5]" << C_RESET << " " << txt("✅ Vérifier dérivé d'une racine", "✅ التحقق من المشتق") << "\n";
    cout << C_BLUE << "[6]" << C_RESET << " " << txt("❌ Quitter", "❌ خروج") << "\n";
    print_rule();
    cout << C_YELLOW << txt("Choix", "الاختيار") << C_RESET << " : ";
}
void menu_racines(){
    cout << "\n";
    print_rule();
    cout << C_BOLD << C_CYAN << txt("Gestion des racines", "إدارة الجذور") << C_RESET << "\n";
    print_rule();
    cout << C_BLUE << "[1]" << C_RESET << " " << txt("📋 Afficher les racines", "📋 عرض الجذور") << "\n";
    cout << C_BLUE << "[2]" << C_RESET << " " << txt("➕ Ajouter une racine", "➕ إضافة جذر") << "\n";
    cout << C_BLUE << "[0]" << C_RESET << " " << txt("⏎ Retour", "⏎ رجوع") << "\n";
    print_rule();
    cout << C_YELLOW << txt("Choix", "الاختيار") << C_RESET << " : ";
}

void menu_schemes(){
    cout << "\n";
    print_rule();
    cout << C_BOLD << C_CYAN << txt("Gestion des schèmes", "إدارة الأوزان") << C_RESET << "\n";
    print_rule();
    cout << C_BLUE << "[1]" << C_RESET << " " << txt("📋 Afficher les schèmes", "📋 عرض الأوزان") << "\n";
    cout << C_BLUE << "[2]" << C_RESET << " " << txt("➕ Ajouter un schème", "➕ إضافة وزن") << "\n";
    cout << C_BLUE << "[3]" << C_RESET << " " << txt("✏️  Modifier un schème", "✏️  تعديل وزن") << "\n";
    cout << C_BLUE << "[4]" << C_RESET << " " << txt("🗑️  Supprimer un schème", "🗑️  حذف وزن") << "\n";
    cout << C_BLUE << "[0]" << C_RESET << " " << txt("⏎ Retour", "⏎ رجوع") << "\n";
    print_rule();
    cout << C_YELLOW << txt("Choix", "الاختيار") << C_RESET << " : ";
}

void menu_derives(){
    cout << "\n";
    print_rule();
    cout << C_BOLD << C_CYAN << "Gestion des dérivés" << C_RESET << "\n";
    print_rule();
    cout << C_BLUE << "[1]" << C_RESET << " 📋 Afficher tous les dérivés\n";
    cout << C_BLUE << "[2]" << C_RESET << " ➕ Ajouter un dérivé\n";
    cout << C_BLUE << "[0]" << C_RESET << " ⏎ Retour\n";
    print_rule();
    cout << C_YELLOW << "Choix" << C_RESET << " : ";
}
string read_file(const string& path){
    ifstream f(path, ios::binary);
    if(!f) return "";
    ostringstream ss;
    ss<<f.rdbuf();
    return ss.str();
}

string url_decode(const string& value){
    string result;
    result.reserve(value.size());
    for(size_t i=0;i<value.size();i++){
        if(value[i]=='%' && i+2<value.size()){
            char hex[3]={value[i+1], value[i+2], '\0'};
            result.push_back(static_cast<char>(strtol(hex,nullptr,16)));
            i+=2;
        } else if(value[i]=='+'){
            result.push_back(' ');
        } else {
            result.push_back(value[i]);
        }
    }
    return result;
}

string json_escape(const string& value){
    string out;
    out.reserve(value.size());
    for(char c: value){
        switch(c){
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out.push_back(c); break;
        }
    }
    return out;
}

string json_array(const vector<string>& items){
    ostringstream ss;
    ss << "[";
    for(size_t i=0;i<items.size();i++){
        if(i>0) ss << ",";
        ss << "\"" << json_escape(items[i]) << "\"";
    }
    ss << "]";
    return ss.str();
}

string get_query_param(const string& target, const string& key){
    auto pos = target.find('?');
    if(pos==string::npos) return "";
    string query = target.substr(pos+1);
    string pattern = key + "=";
    size_t start = query.find(pattern);
    if(start==string::npos) return "";
    start += pattern.size();
    size_t end = query.find('&', start);
    string value = query.substr(start, end==string::npos ? string::npos : end-start);
    return url_decode(value);
}

string content_type_for(const string& path){
    if(path.rfind(".html")!=string::npos) return "text/html; charset=utf-8";
    if(path.rfind(".css")!=string::npos) return "text/css; charset=utf-8";
    if(path.rfind(".js")!=string::npos) return "application/javascript; charset=utf-8";
    if(path.rfind(".json")!=string::npos) return "application/json; charset=utf-8";
    return "text/plain; charset=utf-8";
}

void send_response(socket_t client_fd, const string& status, const string& content_type, const string& body){
    ostringstream ss;
    ss << "HTTP/1.1 " << status << "\r\n";
    ss << "Content-Type: " << content_type << "\r\n";
    ss << "Access-Control-Allow-Origin: *\r\n";
    ss << "Content-Length: " << body.size() << "\r\n";
    ss << "Connection: close\r\n\r\n";
    ss << body;
    string response = ss.str();
    send(client_fd, response.c_str(), static_cast<int>(response.size()), 0);
}

void handle_client(socket_t client_fd, AVL& arbre, TableSchemes& table){
    string request;
    char buffer[4096];
    int bytes;
    while((bytes = recv(client_fd, buffer, sizeof(buffer), 0)) > 0){
        request.append(buffer, buffer + bytes);
        if(request.find("\r\n\r\n") != string::npos) break;
    }

    if(request.empty()){
        close_socket(client_fd);
        return;
    }

    size_t line_end = request.find("\r\n");
    string request_line = request.substr(0, line_end);
    istringstream iss(request_line);
    string method, target, version;
    iss >> method >> target >> version;

    if(method == "OPTIONS"){
        send_response(client_fd, "204 No Content", "text/plain", "");
        close_socket(client_fd);
        return;
    }

    if(target.find("..") != string::npos){
        send_response(client_fd, "400 Bad Request", "text/plain; charset=utf-8", "Bad request");
        close_socket(client_fd);
        return;
    }

    if(target.rfind("/api/analyze", 0) == 0){
        string word = get_query_param(target, "word");
        if(word.empty()){
            send_response(client_fd, "400 Bad Request", "application/json; charset=utf-8", "{\"error\":\"word is required\"}");
            close_socket(client_fd);
            return;
        }
        auto res = verifier_mot(word, arbre, table);
        bool valid = !res.first.empty();
        ostringstream body;
        body << "{\"word\":\"" << json_escape(word) << "\",";
        body << "\"root\":\"" << json_escape(res.first) << "\",";
        body << "\"scheme\":\"" << json_escape(res.second) << "\",";
        body << "\"valid\":" << (valid ? "true" : "false") << "}";
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", body.str());
        close_socket(client_fd);
        return;
    }

    if(target.rfind("/api/roots", 0) == 0){
        vector<string> racines;
        arbre.extraire_racines(arbre.racine, racines);
        string body = string("{\"roots\":") + json_array(racines) + "}";
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", body);
        close_socket(client_fd);
        return;
    }

    if(target.rfind("/api/add-root", 0) == 0){
        string root = get_query_param(target, "root");
        if(root.empty()){
            send_response(client_fd, "400 Bad Request", "application/json; charset=utf-8", "{\"error\":\"root is required\"}");
            close_socket(client_fd);
            return;
        }
        if(!arbre.rechercher(arbre.racine, root)){
            arbre.racine = arbre.inserer(arbre.racine, root);
            ofstream f("data/racines.txt", ios::app);
            f << root << "\n";
        }
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", "{\"status\":\"ok\"}");
        close_socket(client_fd);
        return;
    }

    if(target.rfind("/api/schemes", 0) == 0){
        vector<string> schemes = table.lister();
        string body = string("{\"schemes\":") + json_array(schemes) + "}";
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", body);
        close_socket(client_fd);
        return;
    }

    if(target.rfind("/api/schemes-details", 0) == 0){
        auto items = table.lister_detail();
        ostringstream ss;
        ss << "{\"schemes\":[";
        for(size_t i=0;i<items.size();i++){
            if(i>0) ss << ",";
            ss << "{\"name\":\"" << json_escape(items[i].first) << "\",\"rule\":\"" << json_escape(items[i].second) << "\"}";
        }
        ss << "]}";
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", ss.str());
        close_socket(client_fd);
        return;
    }

    if(target.rfind("/api/add-scheme", 0) == 0){
        string scheme = get_query_param(target, "scheme");
        string rule = get_query_param(target, "rule");
        if(scheme.empty()){
            send_response(client_fd, "400 Bad Request", "application/json; charset=utf-8", "{\"error\":\"scheme is required\"}");
            close_socket(client_fd);
            return;
        }
        if(rule.empty()) rule = scheme;
        table.ajouter(scheme, rule);
        table.sauvegarder_vers_fichier("data/schemes.txt");
        reinitialiser_index(arbre, table);
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", "{\"status\":\"ok\"}");
        close_socket(client_fd);
        return;
    }

    if(target.rfind("/api/update-scheme", 0) == 0){
        string scheme = get_query_param(target, "scheme");
        string rule = get_query_param(target, "rule");
        if(scheme.empty() || rule.empty()){
            send_response(client_fd, "400 Bad Request", "application/json; charset=utf-8", "{\"error\":\"scheme and rule are required\"}");
            close_socket(client_fd);
            return;
        }
        if(!table.contient(scheme)){
            send_response(client_fd, "404 Not Found", "application/json; charset=utf-8", "{\"error\":\"scheme not found\"}");
            close_socket(client_fd);
            return;
        }
        table.modifier(scheme, rule);
        table.sauvegarder_vers_fichier("data/schemes.txt");
        reinitialiser_index(arbre, table);
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", "{\"status\":\"ok\"}");
        close_socket(client_fd);
        return;
    }

    if(target.rfind("/api/delete-scheme", 0) == 0){
        string scheme = get_query_param(target, "scheme");
        if(scheme.empty()){
            send_response(client_fd, "400 Bad Request", "application/json; charset=utf-8", "{\"error\":\"scheme is required\"}");
            close_socket(client_fd);
            return;
        }
        if(!table.contient(scheme)){
            send_response(client_fd, "404 Not Found", "application/json; charset=utf-8", "{\"error\":\"scheme not found\"}");
            close_socket(client_fd);
            return;
        }
        table.supprimer(scheme);
        table.sauvegarder_vers_fichier("data/schemes.txt");
        reinitialiser_index(arbre, table);
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", "{\"status\":\"ok\"}");
        close_socket(client_fd);
        return;
    }

    if(target.rfind("/api/derives", 0) == 0){
        string root = get_query_param(target, "root");
        string schemes_param = get_query_param(target, "schemes");
        if(root.empty()){
            send_response(client_fd, "400 Bad Request", "application/json; charset=utf-8", "{\"error\":\"root is required\"}");
            close_socket(client_fd);
            return;
        }
        NoeudAVL* n = arbre.rechercher(arbre.racine, root);
        if(!n){
            send_response(client_fd, "200 OK", "application/json; charset=utf-8", "{\"valid\":false,\"derives\":[]}");
            close_socket(client_fd);
            return;
        }
        auto derives = generer_derives(n, table);
        vector<string> allowed;
        if(!schemes_param.empty()){
            stringstream ss(schemes_param);
            string item;
            while(getline(ss, item, ',')){
                if(!item.empty()) allowed.push_back(item);
            }
        }
        if(!allowed.empty()){
            vector<pair<string,string>> filtered;
            for(const auto& d : derives){
                if(find(allowed.begin(), allowed.end(), d.first) != allowed.end()){
                    filtered.push_back(d);
                }
            }
            derives = filtered;
        }
        ostringstream ss;
        ss << "{\"valid\":true,\"derives\":[";
        for(size_t i=0;i<derives.size();i++){
            if(i>0) ss << ",";
            ss << "{\"scheme\":\"" << json_escape(derives[i].first) << "\",";
            ss << "\"word\":\"" << json_escape(derives[i].second) << "\"}";
        }
        ss << "]}";
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", ss.str());
        close_socket(client_fd);
        return;
    }

    if(target.rfind("/api/verify-root", 0) == 0){
        string word = get_query_param(target, "word");
        string root = get_query_param(target, "root");
        if(word.empty() || root.empty()){
            send_response(client_fd, "400 Bad Request", "application/json; charset=utf-8", "{\"error\":\"word and root are required\"}");
            close_socket(client_fd);
            return;
        }
        auto res = verifier_mot_racine(word, root, table, arbre);
        ostringstream body;
        body << "{\"word\":\"" << json_escape(word) << "\",";
        body << "\"root\":\"" << json_escape(root) << "\",";
        body << "\"valid\":" << (res.first ? "true" : "false") << ",";
        body << "\"scheme\":\"" << json_escape(res.second) << "\"}";
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", body.str());
        close_socket(client_fd);
        return;
    }

    string path = target == "/" ? "/index.html" : target;
    string file_path = "web" + path;
    string body = read_file(file_path);
    if(body.empty()){
        send_response(client_fd, "404 Not Found", "text/plain; charset=utf-8", "Not found");
        close_socket(client_fd);
        return;
    }
    send_response(client_fd, "200 OK", content_type_for(file_path), body);
    close_socket(client_fd);
}

void run_server(int port, AVL& arbre, TableSchemes& table){
    if(!init_sockets()){
        cerr << "Erreur init sockets" << endl;
        return;
    }

    socket_t server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        cerr << "Erreur socket" << endl;
        cleanup_sockets();
        return;
    }

    int opt = 1;
#ifdef _WIN32
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));
#else
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if(bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0){
        cerr << "Erreur bind (port " << port << ")" << endl;
        close_socket(server_fd);
        cleanup_sockets();
        return;
    }

    if(listen(server_fd, 10) < 0){
        cerr << "Erreur listen" << endl;
        close_socket(server_fd);
        cleanup_sockets();
        return;
    }

    cout << C_GREEN << "Serveur démarré" << C_RESET << " sur http://localhost:" << port << "\n";
    while(true){
        sockaddr_in client{};
        #ifdef _WIN32
        int client_len = sizeof(client);
        #else
        socklen_t client_len = sizeof(client);
        #endif
        socket_t client_fd = accept(server_fd, (sockaddr*)&client, &client_len);
        if(client_fd < 0) continue;
        handle_client(client_fd, arbre, table);
    }
}

int main(int argc, char** argv){
    setlocale(LC_ALL, "");
    try {
        std::locale::global(std::locale(""));
    } catch (...) {
        // Fallback si la locale système n'est pas disponible
    }

    // Initialiser mot_to_racine_scheme pour irréguliers
    for(auto& [rac, formes]: verbes_irreguliers)
        for(auto& [sch, mot]: formes)
            mot_to_racine_scheme[simplifier(mot)]={rac,sch};

    AVL arbre;
    TableSchemes table;
    if(!table.charger_depuis_fichier("data/schemes.txt")){
        table.sauvegarder_vers_fichier("data/schemes.txt");
    }
    charger_racines("data/racines.txt",arbre);
    
    // Construire l'index inversé pour optimiser verifier_mot() : O(n×k) → O(1)
    construire_index(arbre, table);

    bool server_mode = false;
    int port = 8080;
    for(int i=1;i<argc;i++){
        string arg = argv[i];
        if(arg == "--server") server_mode = true;
        else if(arg == "--port" && i+1 < argc) port = stoi(argv[++i]);
        else if(arg == "--ar" || arg == "--arabic") USE_ARABIC = true;
        else if(arg == "--fr" || arg == "--french") USE_ARABIC = false;
    }

    if(server_mode){
        run_server(port, arbre, table);
        return 0;
    }

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
            // Analyser un mot
            cout << C_YELLOW << txt("Mot à analyser", "الكلمة للتحليل") << C_RESET << " : ";
            string m; cin>>m;
            auto res=verifier_mot(m,arbre,table);
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
        else if(choix=="2"){
            while(true){
                menu_racines();
                string sous; cin>>sous;
                if(sous=="1"){
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
                else if(sous=="2"){
                    cout << C_YELLOW << txt("Nouvelle racine", "جذر جديد") << C_RESET << " : ";
                    string r; cin>>r;
                    if(!arbre.rechercher(arbre.racine, r)){
                        arbre.racine=arbre.inserer(arbre.racine,r);
                        ofstream f("data/racines.txt",ios::app); f<<r<<"\n";
                        cout << C_GREEN << "✅ " << txt("Racine ajoutée", "تم إضافة الجذر") << C_RESET << "\n";
                    } else {
                        cout << C_DIM << txt("Racine déjà existante", "الجذر موجود مسبقاً") << C_RESET << "\n";
                    }
                }
                else if(sous=="0"){
                    break;
                }
                else {
                    cout << C_RED << "❌ " << txt("Choix invalide", "اختيار غير صحيح") << C_RESET << "\n";
                }
            }
        }
        else if(choix=="3"){
            while(true){
                menu_schemes();
                string sous; cin>>sous;
                if(sous=="1"){
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
                else if(sous=="2"){
                    cout << C_YELLOW << txt("Nouveau schème", "وزن جديد") << C_RESET << " : ";
                    string sch; cin>>sch;
                    if(!table.contient(sch)){
                        cout << C_YELLOW << txt("Règle", "القاعدة") << C_RESET << " : ";
                        string regle; cin>>regle;
                        if(regle.empty()) regle = sch;
                        table.ajouter(sch, regle);
                        table.sauvegarder_vers_fichier("data/schemes.txt");
                        reinitialiser_index(arbre, table);
                        cout << C_GREEN << "✅ " << txt("Schème ajouté", "تم إضافة الوزن") << C_RESET << "\n";
                    } else {
                        cout << C_DIM << txt("Schème déjà existant", "الوزن موجود مسبقاً") << C_RESET << "\n";
                    }
                }
                else if(sous=="3"){
                    cout << C_YELLOW << txt("Schème à modifier", "الوزن للتعديل") << C_RESET << " : ";
                    string sch; cin>>sch;
                    if(!table.contient(sch)){
                        cout << C_DIM << txt("Schème introuvable", "الوزن غير موجود") << C_RESET << "\n";
                    } else {
                        cout << C_YELLOW << txt("Nouvelle règle", "القاعدة الجديدة") << C_RESET << " : ";
                        string regle; cin>>regle;
                        if(regle.empty()) regle = sch;
                        table.modifier(sch, regle);
                        table.sauvegarder_vers_fichier("data/schemes.txt");
                        reinitialiser_index(arbre, table);
                        cout << C_GREEN << "✅ " << txt("Schème modifié", "تم تعديل الوزن") << C_RESET << "\n";
                    }
                }
                else if(sous=="4"){
                    cout << C_YELLOW << txt("Schème à supprimer", "الوزن للحذف") << C_RESET << " : ";
                    string sch; cin>>sch;
                    if(!table.contient(sch)){
                        cout << C_DIM << txt("Schème introuvable", "الوزن غير موجود") << C_RESET << "\n";
                    } else {
                        table.supprimer(sch);
                        table.sauvegarder_vers_fichier("data/schemes.txt");
                        reinitialiser_index(arbre, table);
                        cout << C_GREEN << "✅ " << txt("Schème supprimé", "تم حذف الوزن") << C_RESET << "\n";
                    }
                }
                else if(sous=="0"){
                    break;
                }
                else {
                    cout << C_RED << "❌ " << txt("Choix invalide", "اختيار غير صحيح") << C_RESET << "\n";
                }
            }
        }
        else if(choix=="4"){
            // Générer dérivés avec option de filtrer par schèmes
            cout << C_YELLOW << txt("Racine", "الجذر") << C_RESET << " : ";
            string r; cin>>r;
            NoeudAVL* n=arbre.rechercher(arbre.racine,r);
            if(n){
                auto all_derives=generer_derives(n,table);
                cout << C_CYAN << "\n🔄 " << txt("Options de génération", "خيارات التوليد") << C_RESET << "\n";
                print_rule();
                cout << C_BLUE << "[1]" << C_RESET << " " << txt("Tous les dérivés", "جميع المشتقات") << " (" << all_derives.size() << ")\n";
                cout << C_BLUE << "[2]" << C_RESET << " " << txt("Sélectionner des schèmes", "اختيار أوزان محددة") << "\n";
                cout << C_BLUE << "[0]" << C_RESET << " " << txt("Annuler", "إلغاء") << "\n";
                print_rule();
                cout << C_YELLOW << txt("Choix", "الاختيار") << C_RESET << " : ";
                string opt_choix; cin>>opt_choix;
                
                vector<pair<string,string>> derives_to_show;
                
                if(opt_choix=="1"){
                    derives_to_show = all_derives;
                } else if(opt_choix=="2"){
                    // Afficher les schèmes disponibles
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
                        // Filtrer les dérivés
                        for(const auto& d : all_derives){
                            if(find(selected_schemes.begin(), selected_schemes.end(), d.first) != selected_schemes.end()){
                                derives_to_show.push_back(d);
                            }
                        }
                    }
                } else {
                    continue;
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
        else if(choix=="5"){
            // Vérifier si un mot dérive d'une racine spécifique
            cout << C_YELLOW << txt("Mot à vérifier", "الكلمة للتحقق") << C_RESET << " : ";
            string m; cin>>m;
            cout << C_YELLOW << txt("Racine", "الجذر") << C_RESET << " : ";
            string r; cin>>r;
            
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
                
                // Optionnel: suggérer la vraie racine si le mot existe
                auto res_mot = verifier_mot(m, arbre, table);
                if(!res_mot.first.empty() && res_mot.first != r){
                    cout << C_CYAN << txt("💡 Suggestion: ce mot dérive de", "💡 اقتراح: هذه الكلمة مشتقة من") << " '" 
                         << res_mot.first << "' " << txt("avec le schème", "بالوزن") << " '" << res_mot.second << "'\n";
                }
            }
        }
        else if(choix=="6") {
            cout << C_GREEN << txt("Au revoir !", "وداعاً!") << C_RESET << "\n";
            break;
        }
        else {
            cout << C_RED << "❌ " << txt("Choix invalide", "اختيار غير صحيح") << C_RESET << "\n";
        }
    }
}
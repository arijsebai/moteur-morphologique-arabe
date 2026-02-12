#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <clocale>
#include <locale>
#include <cstdlib>
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
    cout << C_BLUE << "[1]" << C_RESET << " " << txt("📚 Gestion des racines", "📚 إدارة الجذور") << "\n";
    cout << C_BLUE << "[2]" << C_RESET << " " << txt("📐 Gestion des schèmes", "📐 إدارة الأوزان") << "\n";
    cout << C_BLUE << "[3]" << C_RESET << " " << txt("🔄 Générer dérivés d'une racine", "🔄 توليد مشتقات جذر") << "\n";
    cout << C_BLUE << "[4]" << C_RESET << " " << txt("✔️  Vérifier un mot", "✔️  التحقق من كلمة") << "\n";
    cout << C_BLUE << "[5]" << C_RESET << " " << txt("❌ Quitter", "❌ خروج") << "\n";
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

    if(target.rfind("/api/add-scheme", 0) == 0){
        string scheme = get_query_param(target, "scheme");
        if(scheme.empty()){
            send_response(client_fd, "400 Bad Request", "application/json; charset=utf-8", "{\"error\":\"scheme is required\"}");
            close_socket(client_fd);
            return;
        }
        table.ajouter(scheme, "فعل");
        ofstream f("data/schemes.txt", ios::app);
        f << scheme << "\n";
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", "{\"status\":\"ok\"}");
        close_socket(client_fd);
        return;
    }

    if(target.rfind("/api/derives", 0) == 0){
        string root = get_query_param(target, "root");
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
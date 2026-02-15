#include "RequestHandler.h"
#include "HttpServer.h"
#include "../utils/HttpUtils.h"
#include "../utils/Platform.h"
#include "../core/AVL.h"
#include "../core/TableSchemes.h"
#include "../core/Morphologie.h"
#include "../data/DataLoader.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

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

    // API: analyze word
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

    // API: get all roots
    if(target.rfind("/api/roots", 0) == 0){
        vector<string> racines;
        arbre.extraire_racines(arbre.racine, racines);
        string body = string("{\"roots\":") + json_array(racines) + "}";
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", body);
        close_socket(client_fd);
        return;
    }

    // API: add root
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
            send_response(client_fd, "200 OK", "application/json; charset=utf-8", "{\"success\":true,\"message\":\"Racine ajoutée\"}");
        } else {
            send_response(client_fd, "200 OK", "application/json; charset=utf-8", "{\"success\":false,\"message\":\"الجذر موجود بالفعل\",\"duplicate\":true}");
        }
        close_socket(client_fd);
        return;
    }

    // API: get all schemes
    if(target.rfind("/api/schemes", 0) == 0){
        vector<string> schemes = table.lister();
        string body = string("{\"schemes\":") + json_array(schemes) + "}";
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", body);
        close_socket(client_fd);
        return;
    }

    // API: get scheme details
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

    // API: add scheme
    if(target.rfind("/api/add-scheme", 0) == 0){
        string scheme = get_query_param(target, "scheme");
        string rule = get_query_param(target, "rule");
        if(scheme.empty()){
            send_response(client_fd, "400 Bad Request", "application/json; charset=utf-8", "{\"error\":\"scheme is required\"}");
            close_socket(client_fd);
            return;
        }
        if(table.contient(scheme)){
            send_response(client_fd, "200 OK", "application/json; charset=utf-8", "{\"success\":false,\"message\":\"الوزن موجود بالفعل\",\"duplicate\":true}");
            close_socket(client_fd);
            return;
        }
        if(rule.empty()) rule = scheme;
        table.ajouter(scheme, rule);
        table.sauvegarder_vers_fichier("data/schemes.txt");
        reinitialiser_index(arbre, table);
        send_response(client_fd, "200 OK", "application/json; charset=utf-8", "{\"success\":true,\"message\":\"Schème ajouté\"}");
        close_socket(client_fd);
        return;
    }

    // API: update scheme
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

    // API: delete scheme
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

    // API: generate derivatives
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

    // API: verify root
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

    // Static files
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

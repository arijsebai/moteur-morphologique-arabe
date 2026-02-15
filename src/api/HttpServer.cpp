#include "HttpServer.h"
#include "RequestHandler.h"
#include "../utils/Platform.h"
#include "../utils/Colors.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void send_response(socket_t client_fd, const string& status, 
                   const string& content_type, const string& body){
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

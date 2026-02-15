#pragma once
#include "../utils/Platform.h"
#include <string>

// HTTP server and communication
void send_response(socket_t client_fd, const std::string& status, 
                   const std::string& content_type, const std::string& body);
void run_server(int port, class AVL& arbre, class TableSchemes& table);

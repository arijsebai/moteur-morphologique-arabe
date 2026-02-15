#pragma once
#include "../utils/Platform.h"
#include <string>

class AVL;
class TableSchemes;

// Request handling
void handle_client(socket_t client_fd, AVL& arbre, TableSchemes& table);

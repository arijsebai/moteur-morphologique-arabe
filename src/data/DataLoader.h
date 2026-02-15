#pragma once
#include <string>
#include "../core/AVL.h"

// Data loading and file operations
std::string read_file(const std::string& path);
void load_roots(const std::string& filepath, AVL& arbre);

#include "DataLoader.h"
#include <fstream>
#include <sstream>

using namespace std;

string read_file(const string& path){
    ifstream f(path, ios::binary);
    if(!f) return "";
    ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

void load_roots(const string& filepath, AVL& arbre){
    ifstream f(filepath);
    string ligne;
    while(getline(f, ligne)){
        if(!ligne.empty()) {
            arbre.racine = arbre.inserer(arbre.racine, ligne);
        }
    }
}

#pragma once
#include "../core/AVL.h"
#include "../core/TableSchemes.h"

class CLIHandler {
public:
    CLIHandler(AVL& arbre, TableSchemes& table);
    void run();
    
private:
    AVL& arbre;
    TableSchemes& table;
    
    void handleAnalyzeWord();
    void handleManageRoots();
    void handleManageSchemes();
    void handleGenerateDerivatives();
    void handleVerifyDerivation();
};

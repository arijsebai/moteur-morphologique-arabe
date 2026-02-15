#include "Localization.h"

bool USE_ARABIC = false;

std::string txt(const std::string& fr, const std::string& ar){
    return USE_ARABIC ? ar : fr;
}

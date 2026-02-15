#include "UI.h"
#include "Localization.h"
#include "../utils/Colors.h"
#include <iostream>

using namespace std;

void print_rule(){
    cout << C_DIM << "════════════════════════════════════════" << C_RESET << "\n";
}

void menu(){
    cout << "\n";
    print_rule();
    cout << C_BOLD << C_CYAN << txt("Moteur Morphologique Arabe", "محرك التحليل الصرفي العربي") << C_RESET << "\n";
    print_rule();
    cout << C_BLUE << "[1]" << C_RESET << " " << txt("🔍 Analyser un mot", "🔍 تحليل كلمة") << "\n";
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

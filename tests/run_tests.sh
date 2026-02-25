# Script de test automatique pour le moteur morphologique

echo "=========================================="
echo "Tests du Moteur Morphologique Arabe"
echo "=========================================="
echo ""

PASSED=0
FAILED=0
TOTAL=0

# Fonction pour tester un cas valide
test_valid() {
    local word=$1
    local expected_root=$2
    local expected_scheme=$3
    
    TOTAL=$((TOTAL + 1))
    
    # Appel API (suppose serveur lancé sur port 8080)
    result=$(curl -s "http://localhost:8080/api/analyze?word=$word")
    
    if echo "$result" | grep -q "\"valid\":true"; then
        if echo "$result" | grep -q "\"root\":\"$expected_root\""; then
            if echo "$result" | grep -q "\"scheme\":\"$expected_scheme\""; then
                echo "✓ PASS: $word → $expected_root ($expected_scheme)"
                PASSED=$((PASSED + 1))
            else
                echo "✗ FAIL: $word → mauvais schème (attendu: $expected_scheme)"
                FAILED=$((FAILED + 1))
            fi
        else
            echo "✗ FAIL: $word → mauvaise racine (attendu: $expected_root)"
            FAILED=$((FAILED + 1))
        fi
    else
        echo "✗ FAIL: $word → mot non reconnu (devrait être valide)"
        FAILED=$((FAILED + 1))
    fi
}

# Fonction pour tester un cas invalide
test_invalid() {
    local word=$1
    
    TOTAL=$((TOTAL + 1))
    
    result=$(curl -s "http://localhost:8080/api/analyze?word=$word")
    
    if echo "$result" | grep -q "\"valid\":false"; then
        echo "✓ PASS: $word → correctement rejeté"
        PASSED=$((PASSED + 1))
    else
        echo "✗ FAIL: $word → devrait être invalide"
        FAILED=$((FAILED + 1))
    fi
}

# Vérifier que le serveur est lancé
echo "Vérification du serveur..."
if ! curl -s http://localhost:8080/api/roots > /dev/null; then
    echo "❌ Erreur: Le serveur n'est pas lancé sur le port 8080"
    echo "Lancez d'abord: ./moteur_morphologique --server"
    exit 1
fi
echo "✓ Serveur actif"
echo ""

# Tests cas valides
echo "=========================================="
echo "TEST 1: Cas valides (verbes réguliers)"
echo "=========================================="
test_valid "كتب" "كتب" "فعل"
test_valid "كاتب" "كتب" "فاعل"
test_valid "مكتوب" "كتب" "مفعول"
test_valid "درس" "درس" "فعل"
test_valid "دارس" "درس" "فاعل"
echo ""

# Tests verbes irréguliers
echo "=========================================="
echo "TEST 2: Verbes irréguliers"
echo "=========================================="
test_valid "قال" "قال" "فعل"
test_valid "قائل" "قال" "فاعل"
test_valid "مقول" "قال" "مفعول"
test_valid "قرأ" "قرأ" "فعل"
test_valid "قارئ" "قرأ" "فاعل"
test_valid "مقروء" "قرأ" "مفعول"
test_valid "داعي" "دعى" "فاعل"
test_valid "واقف" "وقف" "فاعل"
test_valid "مادّ" "مدّ" "فاعل"
echo ""

# Tests cas invalides
echo "=========================================="
echo "TEST 3: Cas invalides"
echo "=========================================="
test_invalid "زظغ"
test_invalid "كبت"
test_invalid "hello"
test_invalid "كوتب"
echo ""

# Tests API
echo "=========================================="
echo "TEST 4: API /roots et /schemes"
echo "=========================================="
TOTAL=$((TOTAL + 1))
roots_count=$(curl -s "http://localhost:8080/api/roots" | grep -o "أخذ\|أكل\|كتب" | wc -l)
if [ "$roots_count" -ge 3 ]; then
    echo "✓ PASS: API /roots retourne des racines"
    PASSED=$((PASSED + 1))
else
    echo "✗ FAIL: API /roots ne retourne pas de racines"
    FAILED=$((FAILED + 1))
fi

TOTAL=$((TOTAL + 1))
schemes_count=$(curl -s "http://localhost:8080/api/schemes" | grep -o "فعل\|فاعل\|مفعول" | wc -l)
if [ "$schemes_count" -ge 3 ]; then
    echo "✓ PASS: API /schemes retourne des schèmes"
    PASSED=$((PASSED + 1))
else
    echo "✗ FAIL: API /schemes ne retourne pas de schèmes"
    FAILED=$((FAILED + 1))
fi
echo ""

# Résultats finaux
echo "=========================================="
echo "RÉSULTATS"
echo "=========================================="
echo "Total: $TOTAL tests"
echo "Réussis: $PASSED ✓"
echo "Échoués: $FAILED ✗"
echo ""

if [ $FAILED -eq 0 ]; then
    echo "🎉 Tous les tests sont passés!"
    exit 0
else
    echo "❌ Certains tests ont échoué"
    exit 1
fi

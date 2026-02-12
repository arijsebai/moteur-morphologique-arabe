// ========== Navigation entre les pages ==========
function showPage(pageId) {
    // Masquer toutes les pages
    document.querySelectorAll('.page').forEach(page => {
        page.classList.remove('active');
    });
    
    // Afficher la page sélectionnée
    const targetPage = document.getElementById(pageId);
    if (targetPage) {
        targetPage.classList.add('active');
        setTimeout(() => {
            targetPage.style.opacity = '1';
        }, 50);
    }
    
    // Afficher le bouton retour
    document.getElementById('homeBtn').style.display = 'block';
    
    // Auto-focus sur le premier input
    const firstInput = targetPage.querySelector('.main-input');
    if (firstInput) {
        setTimeout(() => firstInput.focus(), 300);
    }
}

function showHome() {
    // Masquer toutes les pages
    document.querySelectorAll('.page').forEach(page => {
        page.classList.remove('active');
    });
    
    // Afficher la page d'accueil
    document.getElementById('homePage').classList.add('active');
    
    // Masquer le bouton retour
    document.getElementById('homeBtn').style.display = 'none';
    
    // Vider les résultats
    clearAllResults();
}

function clearAllResults() {
    const resultContainers = document.querySelectorAll('.results-container');
    resultContainers.forEach(container => {
        container.innerHTML = '';
    });
}

// ========== Gestion des tabs ==========
function switchTab(tabId) {
    // Obtenir le parent page
    const page = document.querySelector('.page.active');
    if (!page) return;
    
    // Désactiver tous les tabs et contents de cette page
    page.querySelectorAll('.tab').forEach(tab => tab.classList.remove('active'));
    page.querySelectorAll('.tab-content').forEach(content => content.classList.remove('active'));
    
    // Activer le tab cliqué
    const clickedTab = event.target;
    clickedTab.classList.add('active');
    
    // Activer le contenu correspondant
    const targetContent = page.querySelector(`#${tabId}`);
    if (targetContent) {
        targetContent.classList.add('active');
    }
}

// ========== Fonctions API ==========
async function analyzeWord() {
    const wordInput = document.getElementById('wordInput').value;
    const resultsDiv = document.getElementById('analyzeResults');

    if (!wordInput.trim()) {
        resultsDiv.innerHTML = '<div class="message error">❌ الرجاء إدخال كلمة</div>';
        return;
    }

    resultsDiv.innerHTML = '<div class="message info">⏳ جارٍ التحليل...</div>';

    try {
        const response = await fetch(`/api/analyze?word=${encodeURIComponent(wordInput)}`);
        if (!response.ok) {
            throw new Error('Erreur serveur');
        }
        const data = await response.json();
        if (!data.valid) {
            resultsDiv.innerHTML = `
                <div class="message error">
                    <p>❌ الكلمة "${wordInput}" غير موجودة في قاعدة البيانات</p>
                </div>
            `;
            return;
        }
        resultsDiv.innerHTML = `
            <div class="result-item">
                <p>✨ <strong>الكلمة:</strong> ${data.word}</p>
                <p>🌱 <strong>الجذر:</strong> ${data.root}</p>
                <p>⚖️ <strong>الوزن:</strong> ${data.scheme}</p>
            </div>
        `;
    } catch (error) {
        resultsDiv.innerHTML = '<div class="message error">❌ حدث خطأ أثناء الاتصال بالخادم</div>';
    }
}

async function loadRoots() {
    const rootsList = document.getElementById('rootsList');
    rootsList.innerHTML = '<div class="message info">⏳ جارٍ التحميل...</div>';
    try {
        const response = await fetch('/api/roots');
        if (!response.ok) {
            throw new Error('Erreur serveur');
        }
        const data = await response.json();
        if (!data.roots || data.roots.length === 0) {
            rootsList.innerHTML = '<div class="message error">❌ لا توجد جذور حالياً</div>';
            return;
        }
        rootsList.innerHTML = `
            <div class="card-info">
                <p><strong>✅ الجذور المتاحة (${data.roots.length}):</strong></p>
            </div>
            <ul class="list">
                ${data.roots.map((root, index) => `<li>🌱 ${index + 1}. ${root}</li>`).join('')}
            </ul>
        `;
    } catch (error) {
        rootsList.innerHTML = '<div class="message error">❌ تعذر تحميل الجذور</div>';
    }
}

async function addRoot() {
    const rootInput = document.getElementById('rootInput');
    const rootValue = rootInput.value.trim();
    const rootsList = document.getElementById('addRootResult');

    if (!rootValue) {
        rootsList.innerHTML = '<div class="message error">❌ الرجاء إدخال جذر صحيح</div>';
        return;
    }

    rootsList.innerHTML = '<div class="message info">⏳ جارٍ الإضافة...</div>';
    try {
        const response = await fetch(`/api/add-root?root=${encodeURIComponent(rootValue)}`);
        if (!response.ok) {
            throw new Error('Erreur serveur');
        }
        rootInput.value = '';
        rootsList.innerHTML = '<div class="message success">✅ تم إضافة الجذر بنجاح!</div>';
        setTimeout(() => {
            rootsList.innerHTML = '';
            switchTab('viewRoots');
            loadRoots();
        }, 1500);
    } catch (error) {
        rootsList.innerHTML = '<div class="message error">❌ تعذر إضافة الجذر</div>';
    }
}

async function loadSchemes() {
    const schemesList = document.getElementById('schemesList');
    schemesList.innerHTML = '<div class="message info">⏳ جارٍ التحميل...</div>';
    try {
        const response = await fetch('/api/schemes');
        if (!response.ok) {
            throw new Error('Erreur serveur');
        }
        const data = await response.json();
        if (!data.schemes || data.schemes.length === 0) {
            schemesList.innerHTML = '<div class="message error">❌ لا توجد أوزان حالياً</div>';
            return;
        }
        schemesList.innerHTML = `
            <div class="card-info">
                <p><strong>✅ الأوزان المتاحة (${data.schemes.length}):</strong></p>
            </div>
            <ul class="list">
                ${data.schemes.map((scheme, index) => `<li>⚖️ ${index + 1}. ${scheme}</li>`).join('')}
            </ul>
        `;
    } catch (error) {
        schemesList.innerHTML = '<div class="message error">❌ تعذر تحميل الأوزان</div>';
    }
}

async function addScheme() {
    const schemeInput = document.getElementById('schemeInput');
    const schemeValue = schemeInput.value.trim();
    const schemesList = document.getElementById('addSchemeResult');

    if (!schemeValue) {
        schemesList.innerHTML = '<div class="message error">❌ الرجاء إدخال وزن صحيح</div>';
        return;
    }

    schemesList.innerHTML = '<div class="message info">⏳ جارٍ الإضافة...</div>';
    try {
        const response = await fetch(`/api/add-scheme?scheme=${encodeURIComponent(schemeValue)}`);
        if (!response.ok) {
            throw new Error('Erreur serveur');
        }
        schemeInput.value = '';
        schemesList.innerHTML = '<div class="message success">✅ تم إضافة الوزن بنجاح!</div>';
        setTimeout(() => {
            schemesList.innerHTML = '';
            switchTab('viewSchemes');
            loadSchemes();
        }, 1500);
    } catch (error) {
        schemesList.innerHTML = '<div class="message error">❌ تعذر إضافة الوزن</div>';
    }
}

async function generateDerives() {
    const deriveInput = document.getElementById('deriveRootInput');
    const rootValue = deriveInput.value.trim();
    const derivesList = document.getElementById('derivesList');

    if (!rootValue) {
        derivesList.innerHTML = '<div class="message error">❌ الرجاء إدخال جذر</div>';
        return;
    }

    derivesList.innerHTML = '<div class="message info">⏳ جارٍ التوليد...</div>';
    try {
        const response = await fetch(`/api/derives?root=${encodeURIComponent(rootValue)}`);
        if (!response.ok) {
            throw new Error('Erreur serveur');
        }
        const data = await response.json();
        if (!data.valid || !data.derives || data.derives.length === 0) {
            derivesList.innerHTML = '<div class="message error">❌ الجذر غير موجود أو لا توجد مشتقات</div>';
            return;
        }
        derivesList.innerHTML = `
            <div class="card-info">
                <p><strong>✨ مشتقات الجذر ${rootValue} (${data.derives.length}):</strong></p>
            </div>
            <ul class="list">
                ${data.derives.map((item, index) => `<li>💫 ${index + 1}. <strong>${item.word}</strong> <span style="color: #888;">(وزن: ${item.scheme})</span></li>`).join('')}
            </ul>
        `;
    } catch (error) {
        derivesList.innerHTML = '<div class="message error">❌ تعذر توليد المشتقات</div>';
    }
}

async function verifyDerivation() {
    const verifyInput = document.getElementById('verifyWordInput');
    const wordValue = verifyInput.value.trim();
    const verifyResults = document.getElementById('verifyResults');

    if (!wordValue) {
        verifyResults.innerHTML = '<div class="message error">❌ الرجاء إدخال كلمة</div>';
        return;
    }

    verifyResults.innerHTML = '<div class="message info">⏳ جارٍ التحقق...</div>';
    try {
        const response = await fetch(`/api/verify-derivation?word=${encodeURIComponent(wordValue)}`);
        if (!response.ok) {
            throw new Error('Erreur serveur');
        }
        const data = await response.json();
        
        if (!data.valid) {
            verifyResults.innerHTML = `
                <div class="message error">
                    <p>❌ الكلمة "${wordValue}" ليست مشتقة من أي جذر معروف</p>
                </div>
            `;
            return;
        }
        
        // Afficher les résultats
        let html = `
            <div class="result-item">
                <p>✨ <strong>الكلمة:</strong> ${data.word}</p>
                <p>✅ <strong>هذه كلمة صحيحة ومشتقة من جذر عربي!</strong></p>
                <p>🌱 <strong>الجذر:</strong> ${data.root}</p>
                <p>⚖️ <strong>الوزن:</strong> ${data.scheme}</p>
            </div>
        `;
        
        // Si plusieurs correspondances possibles
        if (data.possible_matches && data.possible_matches.length > 1) {
            html += `
                <div class="card-info">
                    <p><strong>📋 احتمالات أخرى (${data.possible_matches.length}):</strong></p>
                    <ul class="list">
                        ${data.possible_matches.map((match, idx) => 
                            `<li>🔹 ${idx + 1}. جذر: <strong>${match.root}</strong> - وزن: <strong>${match.scheme}</strong></li>`
                        ).join('')}
                    </ul>
                </div>
            `;
        }
        
        verifyResults.innerHTML = html;
    } catch (error) {
        verifyResults.innerHTML = '<div class="message error">❌ تعذر التحقق من الكلمة</div>';
    }
}

// Allow Enter key to trigger analysis
document.addEventListener('DOMContentLoaded', function() {
    document.getElementById('wordInput')?.addEventListener('keypress', function(event) {
        if (event.key === 'Enter') {
            analyzeWord();
        }
    });

    document.getElementById('rootInput')?.addEventListener('keypress', function(event) {
        if (event.key === 'Enter') {
            addRoot();
        }
    });

    document.getElementById('deriveRootInput')?.addEventListener('keypress', function(event) {
        if (event.key === 'Enter') {
            generateDerives();
        }
    });

    document.getElementById('schemeInput')?.addEventListener('keypress', function(event) {
        if (event.key === 'Enter') {
            addScheme();
        }
    });

    document.getElementById('verifyWordInput')?.addEventListener('keypress', function(event) {
        if (event.key === 'Enter') {
            verifyDerivation();
        }
    });
});

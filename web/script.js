async function analyzeWord() {
    const wordInput = document.getElementById('wordInput').value;
    const resultsDiv = document.getElementById('results');

    if (!wordInput.trim()) {
        resultsDiv.innerHTML = '<p>الرجاء إدخال كلمة</p>';
        return;
    }

    resultsDiv.innerHTML = '<p>جارٍ التحليل...</p>';

    try {
        const response = await fetch(`/api/analyze?word=${encodeURIComponent(wordInput)}`);
        if (!response.ok) {
            throw new Error('Erreur serveur');
        }
        const data = await response.json();
        if (!data.valid) {
            resultsDiv.innerHTML = `
                <div class="result-item">
                    <p>الكلمة: ${wordInput}</p>
                    <p>لا توجد نتائج مطابقة</p>
                </div>
            `;
            return;
        }
        resultsDiv.innerHTML = `
            <div class="result-item">
                <p>تحليل الكلمة: ${data.word}</p>
                <p>الجذر: ${data.root}</p>
                <p>الوزن: ${data.scheme}</p>
            </div>
        `;
    } catch (error) {
        resultsDiv.innerHTML = '<p>حدث خطأ أثناء الاتصال بالخادم</p>';
    }
}

async function loadRoots() {
    const rootsList = document.getElementById('rootsList');
    rootsList.innerHTML = '<p>جارٍ التحميل...</p>';
    try {
        const response = await fetch('/api/roots');
        if (!response.ok) {
            throw new Error('Erreur serveur');
        }
        const data = await response.json();
        if (!data.roots || data.roots.length === 0) {
            rootsList.innerHTML = '<p>لا توجد جذور حالياً</p>';
            return;
        }
        rootsList.innerHTML = `
            <ul class="list">
                ${data.roots.map(root => `<li>${root}</li>`).join('')}
            </ul>
        `;
    } catch (error) {
        rootsList.innerHTML = '<p>تعذر تحميل الجذور</p>';
    }
}

async function addRoot() {
    const rootInput = document.getElementById('rootInput');
    const rootValue = rootInput.value.trim();
    const rootsList = document.getElementById('rootsList');

    if (!rootValue) {
        rootsList.innerHTML = '<p>الرجاء إدخال جذر صحيح</p>';
        return;
    }

    rootsList.innerHTML = '<p>جارٍ الإضافة...</p>';
    try {
        const response = await fetch(`/api/add-root?root=${encodeURIComponent(rootValue)}`);
        if (!response.ok) {
            throw new Error('Erreur serveur');
        }
        rootInput.value = '';
        await loadRoots();
    } catch (error) {
        rootsList.innerHTML = '<p>تعذر إضافة الجذر</p>';
    }
}

async function generateDerives() {
    const deriveInput = document.getElementById('deriveRootInput');
    const rootValue = deriveInput.value.trim();
    const derivesList = document.getElementById('derivesList');

    if (!rootValue) {
        derivesList.innerHTML = '<p>الرجاء إدخال جذر</p>';
        return;
    }

    derivesList.innerHTML = '<p>جارٍ التوليد...</p>';
    try {
        const response = await fetch(`/api/derives?root=${encodeURIComponent(rootValue)}`);
        if (!response.ok) {
            throw new Error('Erreur serveur');
        }
        const data = await response.json();
        if (!data.valid || !data.derives || data.derives.length === 0) {
            derivesList.innerHTML = '<p>الجذر غير موجود أو لا توجد مشتقات</p>';
            return;
        }
        derivesList.innerHTML = `
            <ul class="list">
                ${data.derives.map(item => `<li>${item.scheme} → ${item.word}</li>`).join('')}
            </ul>
        `;
    } catch (error) {
        derivesList.innerHTML = '<p>تعذر توليد المشتقات</p>';
    }
}

// Allow Enter key to trigger analysis
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

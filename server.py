#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Serveur web Python pour le moteur morphologique arabe
Alternative au serveur C++ natif
"""

import http.server
import socketserver
import json
import urllib.parse
import os
from pathlib import Path

PORT = 8080

class MorphologicalEngine:
    """Moteur morphologique simplifié en Python"""
    
    def __init__(self):
        self.roots = set()
        self.schemes = set()
        self.load_data()
    
    def load_data(self):
        """Charger les données depuis les fichiers"""
        try:
            with open('data/racines.txt', 'r', encoding='utf-8') as f:
                self.roots = {line.strip() for line in f if line.strip()}
        except:
            self.roots = {'كتب', 'درس', 'علم', 'قرأ'}
        
        try:
            with open('data/schemes.txt', 'r', encoding='utf-8') as f:
                self.schemes = {line.strip() for line in f if line.strip()}
        except:
            self.schemes = {'فعل', 'فاعل', 'مفعول', 'فعيل'}
    
    def save_roots(self):
        """Sauvegarder les racines"""
        with open('data/racines.txt', 'w', encoding='utf-8') as f:
            for root in sorted(self.roots):
                f.write(root + '\n')
    
    def save_schemes(self):
        """Sauvegarder les schèmes"""
        with open('data/schemes.txt', 'w', encoding='utf-8') as f:
            for scheme in sorted(self.schemes):
                f.write(scheme + '\n')
    
    def analyze_word(self, word):
        """Analyser un mot"""
        word = word.strip()
        
        # Logique simplifiée d'analyse
        for root in self.roots:
            for scheme in self.schemes:
                # Vérification très simplifiée
                if self._matches(word, root, scheme):
                    return {
                        'valid': True,
                        'word': word,
                        'root': root,
                        'scheme': scheme
                    }
        
        return {'valid': False, 'word': word}
    
    def _matches(self, word, root, scheme):
        """Vérifier si un mot correspond à racine+schème"""
        # Version simplifiée - à améliorer
        if len(word) < 2:
            return False
        
        # Quelques cas de base
        if word == root:
            return scheme == 'فعل'
        
        root_letters = list(root)
        if len(root_letters) >= 3:
            r1, r2, r3 = root_letters[0], root_letters[1], root_letters[2]
            
            if scheme == 'فاعل' and r1 in word and r2 in word and r3 in word:
                return True
            if scheme == 'مفعول' and word.startswith('م') and r1 in word:
                return True
        
        return False
    
    def apply_scheme(self, root, scheme):
        """Appliquer un schème à une racine selon les règles morphologiques arabes"""
        root_letters = list(root)
        if len(root_letters) < 3:
            return None
        
        r1, r2, r3 = root_letters[0], root_letters[1], root_letters[2]
        
        # Dictionnaire des transformations morphologiques
        # ف = première lettre racine, ع = deuxième, ل = troisième
        transformations = {
            'فعل': root,                                      # forme de base
            'فاعل': f'{r1}ا{r2}{r3}',                         # اسم فاعل
            'مفعول': f'م{r1}{r2}و{r3}',                       # اسم مفعول
            'فعيل': f'{r1}{r2}ي{r3}',                         # صفة مشبهة
            'افتعل': f'ا{r1}ت{r2}{r3}',                       # صيغة افتعال
            'تفعيل': f'ت{r1}{r2}ي{r3}',                       # مصدر
            'مفعل': f'م{r1}{r2}{r3}',                         # اسم مكان/زمان
            'استفعال': f'است{r1}{r2}ا{r3}',                  # استفعال
            'فعّل': f'{r1}{r2}{r2}{r3}',                      # فعّل (تضعيف)
            'مستفعل': f'مست{r1}{r2}{r3}',                     # اسم فاعل استفعال
            'انفعل': f'ان{r1}{r2}{r3}',                       # انفعال
            'تفاعل': f'ت{r1}ا{r2}{r3}',                       # تفاعل
            'فعال': f'{r1}{r2}ا{r3}',                         # فعال
            'فعول': f'{r1}{r2}و{r3}',                         # فعول
            'مفاعل': f'م{r1}ا{r2}{r3}',                       # مفاعل
            'تفعّل': f'ت{r1}{r2}{r2}{r3}',                    # تفعّل
            'افعال': f'ا{r1}{r2}ا{r3}',                       # أفعال
            'مفعلة': f'م{r1}{r2}{r3}ة',                       # مفعلة
            'فعلان': f'{r1}{r2}{r3}ان',                       # فعلان
            'فاعلة': f'{r1}ا{r2}{r3}ة',                       # فاعلة
        }
        
        # Retourner la transformation si elle existe, sinon générer dynamiquement
        if scheme in transformations:
            return transformations[scheme]
        
        # Génération dynamique pour les schèmes non reconnus
        # Remplacer ف par r1, ع par r2, ل par r3
        word = scheme.replace('ف', r1).replace('ع', r2).replace('ل', r3)
        return word
    
    def generate_derives(self, root):
        """Générer les dérivés d'une racine pour TOUS les schèmes disponibles"""
        if root not in self.roots:
            return {'valid': False, 'derives': []}
        
        derives = []
        
        # Générer un dérivé pour chaque schème disponible
        for scheme in sorted(self.schemes):
            word = self.apply_scheme(root, scheme)
            if word:
                derives.append({'word': word, 'scheme': scheme})
        
        return {'valid': True, 'root': root, 'derives': derives}
    
    def verify_derivation(self, word):
        """Vérifier si un mot est dérivé d'une racine et identifier laquelle"""
        word = word.strip()
        possible_matches = []
        
        # Parcourir toutes les racines
        for root in self.roots:
            # Vérifier tous les schèmes pour cette racine
            for scheme in self.schemes:
                derived = self.apply_scheme(root, scheme)
                if derived and derived == word:
                    possible_matches.append({
                        'root': root,
                        'scheme': scheme
                    })
        
        if possible_matches:
            # Retourner le premier match et tous les possibles
            return {
                'valid': True,
                'word': word,
                'root': possible_matches[0]['root'],
                'scheme': possible_matches[0]['scheme'],
                'possible_matches': possible_matches
            }
        
        return {
            'valid': False,
            'word': word
        }


# Instance globale du moteur
engine = MorphologicalEngine()


class MorphologicalHandler(http.server.SimpleHTTPRequestHandler):
    """Gestionnaire de requêtes HTTP avec API"""
    
    def do_GET(self):
        """Gérer les requêtes GET"""
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path
        params = urllib.parse.parse_qs(parsed.query)
        
        # API endpoints
        if path.startswith('/api/'):
            self.handle_api(path, params)
        else:
            # Servir les fichiers statiques
            if path == '/':
                self.path = '/web/index.html'
            elif not path.startswith('/web/'):
                self.path = '/web' + path
            
            super().do_GET()
    
    def handle_api(self, path, params):
        """Gérer les requêtes API"""
        self.send_response(200)
        self.send_header('Content-type', 'application/json; charset=utf-8')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        
        response = {}
        
        try:
            if path == '/api/analyze':
                word = params.get('word', [''])[0]
                response = engine.analyze_word(word)
            
            elif path == '/api/roots':
                response = {'roots': sorted(list(engine.roots))}
            
            elif path == '/api/add-root':
                root = params.get('root', [''])[0].strip()
                if root:
                    engine.roots.add(root)
                    engine.save_roots()
                    response = {'success': True, 'message': 'Racine ajoutée'}
                else:
                    response = {'success': False, 'message': 'Racine invalide'}
            
            elif path == '/api/schemes':
                response = {'schemes': sorted(list(engine.schemes))}
            
            elif path == '/api/add-scheme':
                scheme = params.get('scheme', [''])[0].strip()
                if scheme:
                    engine.schemes.add(scheme)
                    engine.save_schemes()
                    response = {'success': True, 'message': 'Schème ajouté'}
                else:
                    response = {'success': False, 'message': 'Schème invalide'}
            
            elif path == '/api/derives':
                root = params.get('root', [''])[0]
                response = engine.generate_derives(root)
            
            elif path == '/api/verify-derivation':
                word = params.get('word', [''])[0]
                response = engine.verify_derivation(word)
            
            else:
                response = {'error': 'Unknown endpoint'}
        
        except Exception as e:
            response = {'error': str(e)}
        
        self.wfile.write(json.dumps(response, ensure_ascii=False).encode('utf-8'))
    
    def log_message(self, format, *args):
        """Logger personnalisé avec emojis"""
        print(f"🌐 [{self.log_date_time_string()}] {format % args}")


def main():
    """Démarrer le serveur"""
    os.chdir(Path(__file__).parent)
    
    with socketserver.TCPServer(("", PORT), MorphologicalHandler) as httpd:
        print("=" * 60)
        print(f"🚀 Serveur morphologique démarré")
        print(f"📡 URL: http://localhost:{PORT}")
        print(f"📁 Dossier: {os.getcwd()}")
        print("=" * 60)
        print("✅ Prêt à recevoir des requêtes...")
        print("⏹️  Ctrl+C pour arrêter")
        print("=" * 60)
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n\n🛑 Arrêt du serveur...")
            httpd.shutdown()


if __name__ == '__main__':
    main()

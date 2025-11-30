#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <queue>
#include <sstream>

class GrafoCSR {
private:
    int numNodos;
    int numAristas;
    std::vector<int> row_ptr; 
    std::vector<int> col_indices;

public:
    GrafoCSR() : numNodos(0), numAristas(0) {}
    
    void cargarDatos(const std::string& filename) {
        std::cout << "\n[C++] INICIANDO CARGA SEGURA..." << std::endl;
        std::ifstream file(filename);
        if (!file.is_open()) { 
            std::cerr << "[ERROR] No se pudo abrir el archivo: " << filename << std::endl; 
            return; 
        }

        std::vector<std::vector<int>> tempAdj;
        int maxID = 0;
        std::string line;
        long long linea_actual = 0;

        try {
            while (std::getline(file, line)) {
                linea_actual++;
                if (line.empty()) continue;

                std::replace(line.begin(), line.end(), '\t', ' ');

                if (line[0] == '#' || line[0] == '%') continue;

                int u, v;
                std::stringstream ss(line);
                
                if (!(ss >> u >> v)) continue;

                if (linea_actual % 100000 == 0) {
                    std::cout << "\r[C++] Procesando linea: " << linea_actual << std::flush;
                }

                if (u > maxID) maxID = u;
                if (v > maxID) maxID = v;
                
                if (tempAdj.size() <= maxID) {
                    if (maxID > tempAdj.size() + 1000000) {
                         std::cout << "\n[Aviso] Redimensionando memoria para nodo: " << maxID << std::endl;
                    }
                    tempAdj.resize(maxID + 1);
                }
                
                tempAdj[u].push_back(v);
                numAristas++;
            }
        } catch (const std::exception& e) {
            std::cerr << "\n[ERROR CRITICO] " << e.what() << std::endl;
            return;
        }

        std::cout << "\n[C++] Lectura finalizada. Comprimiendo a CSR..." << std::endl;
        
        numNodos = tempAdj.size();
        row_ptr.resize(numNodos + 1);
        col_indices.reserve(numAristas); 
        
        int current = 0;
        for (int i = 0; i < numNodos; ++i) {
            row_ptr[i] = current;
            for (int vecino : tempAdj[i]) {
                col_indices.push_back(vecino);
                current++;
            }
        }
        row_ptr[numNodos] = current;
        
        std::cout << "[EXITO] Grafo cargado. Nodos: " << numNodos << " | Conexiones: " << numAristas << std::endl;
    }

    int obtenerNodoCritico() {
        if (numNodos == 0) return -1;
        int maxG = -1, id = -1;
        for (int i = 0; i < numNodos; ++i) {
            int g = row_ptr[i+1] - row_ptr[i];
            if (g > maxG) { maxG = g; id = i; }
        }
        return id;
    }

    std::vector<int> bfs(int start, int depth) {
        std::vector<int> res;
        if (numNodos == 0 || start < 0 || start >= numNodos) return res;
        
        std::vector<bool> seen(numNodos, false);
        std::queue<std::pair<int, int>> q;
        
        q.push({start, 0});
        seen[start] = true;
        
        while(!q.empty()){
            int u = q.front().first;
            int d = q.front().second;
            q.pop();
            res.push_back(u);
            
            if(d < depth){
                for(int i=row_ptr[u]; i<row_ptr[u+1]; ++i){
                    int v = col_indices[i];
                    if(!seen[v]){ seen[v]=true; q.push({v, d+1}); }
                }
            }
        }
        return res;
    }
    
    int getN() { return numNodos; }
    int getM() { return numAristas; }
};
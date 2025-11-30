#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <queue>

class GrafoCSR {
private:
    int numNodos;
    int numAristas;
    std::vector<int> row_ptr; 
    std::vector<int> col_indices;

public:
    GrafoCSR() : numNodos(0), numAristas(0) {}
    
    void cargarDatos(const std::string& filename) {
        std::cout << "[C++] Leyendo: " << filename << std::endl;
        std::ifstream file(filename);
        if (!file.is_open()) { std::cerr << "Error al abrir archivo" << std::endl; return; }

        std::vector<std::vector<int>> tempAdj;
        int u, v, maxID = 0;
        while (file >> u >> v) {
            if (u > maxID) maxID = u;
            if (v > maxID) maxID = v;
            if (tempAdj.size() <= maxID) tempAdj.resize(maxID + 1);
            tempAdj[u].push_back(v);
            tempAdj[v].push_back(u); 
            numAristas++;
        }
        numNodos = tempAdj.size();

        row_ptr.resize(numNodos + 1);
        col_indices.reserve(numAristas * 2);
        int current = 0;
        for (int i = 0; i < numNodos; ++i) {
            row_ptr[i] = current;
            for (int vecino : tempAdj[i]) {
                col_indices.push_back(vecino);
                current++;
            }
        }
        row_ptr[numNodos] = current;
        std::cout << "[C++] CSR Creado. Nodos: " << numNodos << " Aristas: " << numAristas << std::endl;
    }

    int obtenerNodoCritico() {
        int maxG = -1, id = -1;
        for (int i = 0; i < numNodos; ++i) {
            int g = row_ptr[i+1] - row_ptr[i];
            if (g > maxG) { maxG = g; id = i; }
        }
        return id;
    }

    std::vector<int> bfs(int start, int depth) {
        std::vector<int> res;
        if (start >= numNodos) return res;
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

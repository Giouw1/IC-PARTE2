#ifndef gera_grafos
#define gera_grafos
#include <cstdlib>  // For system()
#include <random>
#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <tuple>

using namespace std;
typedef vector<vector<int>> grafo;

//preco = pre colored, numco = numero de cores, nvert = numero de vertices
void gera_txt(int nvert, float preco, float numco);
pair<grafo,vector<int>> gera_grafo(int nvert, float preco, float numco);
pair<grafo,vector<int>> txtparagrafo(int nvert);


#endif

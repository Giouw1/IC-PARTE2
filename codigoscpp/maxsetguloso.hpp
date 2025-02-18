#ifndef maxsetoguloso
#define maxsetguloso
#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <filesystem>
#include <chrono>
#include "gera_grafos.hpp"

using namespace std;
typedef vector<vector<int>> grafo;



//preco = pre colored, numco = numero de cores, nvert = numero de vertices
int BT (grafo g, vector<int>custos, int k, unordered_set<int> f,int seed);

int felicidade_maxima(grafo g,vector<int> custos, int k, int seed);


#endif
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
pair<int,pair<unordered_set<int>,unordered_set<int>>> BT (grafo g, vector<int>custos, int k, unordered_set<int> f,int seed,float gamma);

pair<int,pair<unordered_set<int>,unordered_set<int>>> felicidade_maxima_guloso(grafo g,vector<int> custos, int k, int seed, float gamma);


#endif
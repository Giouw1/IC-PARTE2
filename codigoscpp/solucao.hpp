#ifndef solucao
#define solucao
#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <filesystem>
#include <chrono>
using namespace std;
typedef vector<vector<int>> grafo;



//preco = pre colored, numco = numero de cores, nvert = numero de vertices
pair<int,pair<int,pair<unordered_set<int>,vector<bool>>>> BT (grafo g, vector<int>graus, int k, unordered_set<int> h, unordered_set<int> hB, unordered_set<int> f, vector<bool> colorido,clock_t clock_start,int limit,int seed,int bestsol, int nodes);

pair<int,pair<int,pair<unordered_set<int>,vector<bool>>>> felicidade_maxima_backtrack(grafo g,vector<int> graus, int k,clock_t clock_start,int limit,int seed);


#endif
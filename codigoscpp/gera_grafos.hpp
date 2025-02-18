#ifndef gera_grafos
#define gera_grafos
#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <fstream>

using namespace std;
typedef vector<vector<int>> grafo;

//preco = pre colored, numco = numero de cores, nvert = numero de vertices
pair<int,pair<grafo,vector<int>>> leitor_grafos(string path);



#endif

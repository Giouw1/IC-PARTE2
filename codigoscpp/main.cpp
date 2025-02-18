#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <filesystem>
#include <chrono>
using namespace std;
#include "maxsetguloso.hpp"
#include "solucao.hpp"
#include "gera_grafos.hpp"

int main(int argc, char* argv[]) {
    if (argc < 4) {  // Expecting at least 4 arguments (program name + 3 inputs)
        cerr << "Está faltando parâmetros: É um path, um inteiro(limite de tempo), e outro inteiro (seed)"<< endl;
        return 1;
    }
    string path = argv[1];     // First argument: file path
    int limit = stoi(argv[2]); // Second argument: integer (converted from string)
    int seed = stoi(argv[3]);
    ofstream arquivoresult("resultcpp.txt");
    grafo g;
    vector<int> graus;
    int k;
    int nodes;
    int solutionback;
    pair<grafo,vector<int>> get;
    unordered_set<int> happyback;
    unordered_set<int> coloredback;
    tie(k,get) = leitor_grafos(path);
    g = get.first;
    graus = get.second;
    //lendo o grafo
    clock_t clock_start;
    clock_t clock_end;
    int tempo;
    pair<int,unordered_set<int>> getter;
    //executando backtracking
    clock_start = clock();
    tie(nodes,getter) = felicidade_maxima(g,graus,k,clock_start,limit,seed);
    clock_end = clock();
    tempo = static_cast<int>(double((clock_end-clock_start)/CLOCKS_PER_SEC));
    solutionback = getter.first;
    happyback = getter.second;
    arquivoresult<< nodes << " "<< tempo << " "<< solutionback<<endl;
    
    for (int element: happyback){
        for (int viz : g[element]){
            coloredback.insert(viz);
        }
    }
    for (int element:happyback){
        arquivoresult<< "h"<< " "<< element <<endl;
    }    
    for (int element:coloredback){
        arquivoresult<< "c"<< " "<< element <<endl;
    }
    vector<int> happyheur;
    int solutionheur;
    int ajuda;
    for (int i = 0;i<25;i++){
        ajuda = solutionheur;
        solutionheur = felicidade_maxima(g,graus,k,seed);
        if (ajuda>solutionheur){
            solutionheur = ajuda;
        }

    }
    arquivoresult<< solutionheur<< endl;
    arquivoresult.close();




}
//Roteiro: testar legal solucao.cpp, ver se o main consegue chamar os dois corretamente
//Após isso, tentar começar a chamar o main pelo python,ajustar o excel para conter seus dados,e somente após isso, testar em instâncias conhecidas.
//Pode demorar, mas vai funcionar.
//Ver as seeds depois, randomizar a heurística

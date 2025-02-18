#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <filesystem>
#include <chrono>
using namespace std;
#include "maxsetguloso.hpp"
#include "solucao.hpp"
#include "gera_grafos.hpp"

int main(/*int argc, char* argv[]*/) {
    /*if (argc < 4) {  // Expecting at least 4 arguments (program name + 3 inputs)
        cerr << "Está faltando parâmetros: É um path, um inteiro(limite de tempo), e outro inteiro (seed)"<< endl;
        return 1;
    }
    string path = argv[1];     // First argument: file path
    int k = stoi(argv[2]);
    int limit = stoi(argv[3]); // Second argument: integer (converted from string)
    int seed = stoi(argv[4]);*/
    string path = "/home/aluno/Documents/giotemp/IC-PARTE2-main/pasta/graph_d_regular_k2_p0.3_v100_20241127_083958.txt";
    int k = 50;
    int limit = 60;
    int seed = 0;
    ofstream arquivoresult("resultcpp.txt");
    grafo g;
    vector<int> graus;
    int ka;
    int nodes;
    int solutionback;
    pair<grafo,vector<int>> get;
    unordered_set<int> happyback;
    unordered_set<int> coloredback;
    tie(ka,get) = leitor_grafos(path);
    g = get.first;
    graus = get.second;
    //lendo o grafo
    clock_t clock_start;
    clock_t clock_end;
    int tempo;
    pair<int,unordered_set<int>> getter;
    //executando backtracking
    time_t timestamp = time(&timestamp);
    struct tm datetime = *localtime(&timestamp);
    cout << datetime.tm_min<< " "<< datetime.tm_sec<<endl;
    clock_start = clock();
    tie(nodes,getter) = felicidade_maxima(g,graus,k,clock_start,limit,seed);
    clock_end = clock();//checar essa questão do tempo
    time_t timestampa = time(&timestampa);
    struct tm datetimea = *localtime(&timestampa);
    cout << datetimea.tm_min<< " "<< datetimea.tm_sec<<endl;
    tempo = double((clock_end-clock_start)/CLOCKS_PER_SEC);
    cout <<tempo<<endl;
    cout<<nodes<<endl;
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
    for (int i = 0;i<25;i++){//marcar todos os resultados e ver os tempos--total e individual
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

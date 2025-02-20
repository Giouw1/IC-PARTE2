#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include "gera_grafos.hpp"
#include "maxsetguloso.hpp"
//randomizar isso daqui
int BT (grafo g, vector<int>custos, int k, unordered_set<int> f,int seed){
    vector<int> candidato(0);
    int graumin;
    int resposta = 0;
    vector<int> colorido(g.size(),0);
    std::mt19937 gen(seed);
    unordered_set<int> happy;
    unordered_set<int> coloridos;
    while (k>0){
        int v = -1;
        graumin = g.size()+1;
        candidato = {};
        for (auto it = f.begin(); it != f.end(); ) {
            //1a etapa, escolher o candidato v:certo
            if (colorido[*it]){
                    if (custos[*it]<graumin){
                        graumin = custos[*it];
                        candidato = {*it};
                    }
                    if (custos[*it]==graumin){
                        candidato.push_back(*it);
                    }
            }   else{
                    if (custos[*it]+1<graumin){
                        graumin = custos[*it]+1;
                        candidato = {*it};
                    }
                    if (custos[*it]+1==graumin){
                        candidato.push_back(*it);
                    }
            }
            it++;
        }
        uniform_int_distribution<int> dist(0,candidato.size()-1);
        v = candidato[dist(gen)];
        //Gera o candidato
        if (colorido[v] == 0){
            resposta+=1;
            k-=custos[v]+1;
        }
        else{
            resposta+=1;
            k -=custos[v];

        }
        if (k<0){
            resposta-=1;
            return resposta;
        }
        happy.insert(v);
        cout<< (k<0) <<endl;


        //descobri o v, agora tenho que redefinir os custos
        for (int viz : g[v]){  //para cada vizinho de v e para cada vizinho dos vizinhos de v, reduzir o grau do vizinho em 1, caso nao tenham sido coloridos
            //Se v já tinha sido colorido na etapa anterior, não precisa reavaliar vizinhos de v
            if (colorido[v] == 0){
                custos[viz]--; //tira 1 grau pois o vértice v ficou feliz, logo, colorido
            }
            if (colorido[viz] == 0){ //se o vértice i já não tiver sido colorido por uma outra etapa
                coloridos.insert(viz);
                colorido[viz] = 1; //colore ele, e colore os vizinhos se eles ja nao forem coloridos, para o mundo de v feliz
                for (int vizviz : g[viz]){ // A avaliação dos vizinhos i dos vértices vizinhos ao feliz da vez
                    custos[vizviz]--;//tira 1 grau para cada vizinho dos coloridol que deixam v feliz. 
                    }
                }
            }
            custos[v] = 0;
            colorido[v] = 1;
            f.erase(v);
        }

        return resposta;
    }

int felicidade_maxima(grafo g,vector<int> custos, int k,int seed){
    unordered_set<int> coloriveis;
    vector<int> coloridos(g.size(),0);
    for (int i = 0;i<g.size();i++){
        coloriveis.insert(i);
    }
    return BT(g,custos,k,coloriveis,seed);
}




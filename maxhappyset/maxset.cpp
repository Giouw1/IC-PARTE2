#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include "gera_grafos.hpp"
  
pair<int,vector<int>> BT (grafo g, vector<int>graus, int k, unordered_set<int> f){
    int v;
    int graumin;
    vector<int> colorido(g.size(),0);
    while (k>0){
        v=-1;
        graumin = g.size()+1;
        for (auto it = f.begin(); it != f.end(); ) {
            //1a etapa, escolher o candidato v
            if (colorido[*it]){
                    if (graus[*it]<graumin){
                        graumin = graus[*it];
                        v = *it;
                    }
                }
            else{
                    if (graus[*it]+1<graumin){
                        graumin = graus[*it]+1;
                        v = *it;
                }
            }
            it++;
            }
            if (colorido[v] == 0){
                if (k<graus[v]+1){
                    f.erase(v);
                }
                else{
                k = k-(graus[v]+1);    
                    //se não tiver sido colorido, tem que colorir o vértice
                }
            }//se v já foi colorido
            else{
                if (k<graus[v]){
                    f.erase(v);
                }
                else{
                    k = k-(graus[v]);    
                }
            }
            
        //descobri o v, agora tenho que redefinir os graus
            for (int i = 0; i< g[v].size();i++){  //para cada vizinho de v e para cada vizinho dos vizinhos de v, reduzir o grau do vizinho em 1, caso nao tenham sido coloridos
                if (g[v][i] == 1){ //Se v já tinha sido colorido na etapa anterior, não precisa reavaliar vizinhos de v
                    if (colorido[v] != 1){
                        graus[i]--; //tira 1 grau pois o vértice v ficou feliz, logo, colorido
                    }
                    if (colorido[i] != 1){ //se o vértice i já não tiver sido colorido por uma outra etapa
                        colorido[i] = 1; //colore ele, e muda o custo dos vizinhos dele
                        for (int j = 0; j<g[i].size();j++){ // A avaliação dos vizinhos i dos vértices vizinhos ao feliz da vez
                            if (g[i][j] == 1){
                                graus[j]--;//tira 1 grau para cada vizinho dos coloridol que deixam v feliz.
                            }
                        }
                    }
                }
            }
            graus[v] = 0;
            colorido[v] = 1;
            f.erase(v);
        }
        return {0,colorido};
    }

pair<int,vector<int>> felicidade_maxima(grafo g,vector<int> graus, int k){
     unordered_set<int> coloriveis;
    vector<int> coloridos(g.size(),0);
    for (int i = 0;i<graus.size();i++){
            coloriveis.insert(i);
    }
    return BT(g,graus,k,coloriveis);
}


int main(){ 
    grafo g;
    vector<int> graus; 
    tie(g,graus) =  criador_grafos(8,6);
    vector<int> resposta;
    int b;
    tie(b,resposta) = felicidade_maxima(g,graus,5);
    cout << b<< endl;
    return 0;
}


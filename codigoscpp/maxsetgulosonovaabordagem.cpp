#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include "gera_grafos.hpp"
#include "maxsetguloso.hpp"


//randomizar isso daqui
//Implementar o elitismo depois talvez. por enquanto, gamma
pair<int,pair<unordered_set<int>,unordered_set<int>>> BT (grafo g, vector<int>custos, int k, unordered_set<int> f,int seed, float gamma = 1){
    list<int> candidato(0);
    int graumin;
    int resposta = 0;
    vector<int> colorido(g.size(),0);
    std::mt19937 gen(seed);
    unordered_set<int> happy;
    unordered_set<int> coloridos;
    while (true){
        vector<int> deletar_f = {};
        int v = -1;
        graumin = g.size()+1;
        candidato.clear();
        if (f.size() == 0){//Caso K seja=nvert
            return {resposta,{happy,coloridos}};
        }
        for (auto it = f.begin(); it != f.end();) {
            //1a etapa, escolher o candidato v:certo
            if (colorido[*it]){
                    if (custos[*it] == 0){ // caso tenha custo zero, pula essa iteração
                        deletar_f.push_back(*it);
                        happy.insert(*it);
                        resposta+=1;
                    }   
                    else{
                        if (custos[*it]>k){//caso seja impossível, já adiciona
                            deletar_f.push_back(*it);
                        }
                        else{
                            if (custos[*it]<graumin*gamma){//caso possa ser um candidato (estar no range)
                                candidato.push_back(*it);
                            }
                            //O caso acima já adiciona o novo melhor candidato (custos[*it]<graumin caso ele apareca), não preciso readicionar
                            if (custos[*it]<graumin){
                                graumin = custos[*it];
                                for (auto it = candidato.begin(); it != candidato.end(); ) {
                                    if (custos[*it]>graumin*gamma) {
                                        it = candidato.erase(it);  // erase retorna o próximo iterador válido
                                    } else {
                                        ++it;
                                    }
                                }

                            }
                        }
                    }
            }
            else{
                if (custos[*it]+1>k){
                    deletar_f.push_back(*it);
                }
                else{
                    if (custos[*it]+1<graumin*gamma){
                        candidato.push_back(*it);
                    }
                    if (custos[*it]+1<graumin){
                       graumin = custos[*it]+1;
                        for (auto it = candidato.begin(); it != candidato.end(); ) {
                            if (custos[*it]+1>graumin*gamma) {
                                it = candidato.erase(it);  // erase retorna o próximo iterador válido
                            } else {
                                ++it;
                            }
                        }
                    }
                }
            }
            it++;
        }
        if (deletar_f.size()>0){
            for (int v: deletar_f){
                f.erase(v);
                }
            }
        //Isso aqui é alcançado, é a saída de fato.
        if (candidato.size() == 0){
            if (happy.size() == 0){
                    return {resposta,{}};
                }
                return {resposta,{happy,coloridos}};
            }
    
        uniform_int_distribution<int> dist(0,candidato.size()-1);
        auto it = candidato.begin();
        std::advance(it, dist(gen));
        v = *it;
        //Gera o candidato
        if (colorido[v] == 0){
            resposta+=1;
            k-=custos[v]+1;
        }
        else{
            resposta+=1;
            k -=custos[v];

        }
        if (k<0){//Se reduzir o k para um número menor do que 0, 
                 //o vértice v não pode ser colorido, e já encontramos a solução
                 //Isso aqui é nunca alcançado
            resposta-=1;
            if (happy.size() == 0){
                return {resposta,{}};
            }
            return {resposta,{happy,coloridos}};
        }
        //Se k é maior que 0, v faz parte da solução, e continuar o ciclo
        happy.insert(v);
        coloridos.insert(v);


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
            custos[v] = 0;//Só uma formalidade,pois v já é feliz    
            colorido[v] = 1;
            f.erase(v);
        }
    }

pair<int,pair<unordered_set<int>,unordered_set<int>>> felicidade_maxima_guloso (grafo g,vector<int> custos, int k,int seed, float gamma = 1){
    unordered_set<int> coloriveis;
    vector<int> coloridos(g.size(),0);
    for (int i = 0;i<g.size();i++){
        coloriveis.insert(i);
    }
    return BT(g,custos,k,coloriveis,seed,gamma);
}
/*int main(){
    grafo g;
    vector<int> graus;
    tie(g,graus) = leitor_grafos("/home/aluno/giotemp/IC-PARTE2-main/pasta/graph_d_regular_k2_p0.3_v100_20241127_083958.txt");
    cout<<felicidade_maxima(g,graus,40,1)<<endl;
}*/



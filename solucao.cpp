#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include "gera_grafos.hpp"
using namespace std;

    //Boa opção é definir o primeiro f e o primeiro hB antes de começar a execução: resolver agora isso

    //Dúvida no bestsol inicial
    //h são os felizes, hB são não felizes, f são os felizíveis: preciso do hB
    //Grau nesse código acaba sendo algo como o custo de pintar: preciso garantir o bom funcionamento da revisão dos graus: se isso funcionar, fechou. se não, pensar na redefinição do hb e f
    //O que fazer: acertar o redefinir graus(grausnovo), redefinir o hB e f para o que não contém v: Solução para o redefinir parece simples, mas custo alto. Preciso redefinir o hB e f? Nesses casos, o que acontece não é que o chamado mesmo com o f e hB errado são redefinidos, e o candidato é escolhido corretamente? talvez tenha uma chamada a mais, mas e daí?
    //Acho que dá certo pois o que contém v procura os novos candidatos corretamente, assim como o que não contém v: minha dúvida e se a análise das condições vai fazer sentido (acho que não). pensar anas analsies de condições, gastar uns 15 minutos focando entendendo o código
    //Erro de lógica: não faz sentido os dois f's serem iguais, os coloridos dado que v é feliz e dado q v não é feliz são diferentes: escolher candidatos, redefinir graus, redefinir hB e f para o não contendo: terá um hBL e um fL (um para v feliz e outro para v não feliz)
int BT (grafo g, vector<int>graus, int k, unordered_set<int> h, unordered_set<int> hB, unordered_set<int> f, vector<bool> colorido = {}){
    int v = -1;
    int bestsol1;
    int bestsol2;
    int graumin = g.size()+1;
    vector<int> auxiliar;
    unordered_set<int> hl= h;
    vector<int> candidatos(0);
    vector<int> grausnovo = graus;
    vector<bool> coloridos = colorido; //Cópia dos coloridos pelo passo anterior, para poder tanto passar o parâmetro de um v colorido quanto de um v não colorido
    mt19937 gen(random_device{}());
    //Obtendo os candidatos à v: os de menor custo/grau
    for (auto it = f.begin(); it != f.end(); ) {
        if (graus[*it]>k) {
            hB.insert(*it);
            it = f.erase(it);  // apaga e vai para o próximo iterador : aqui eu já tiro os vértices que não podem ser coloridos: reavaliação do f para um dado K ISSO aqui está errado
        } 
        else{
            if (graus[*it]<graumin){
                    graumin = graus[*it];
                    candidatos = {*it};
                }
            else{
                if (graus[*it]==graumin){
                candidatos.push_back(*it);
                }
            }
            ++it;
        }
    } 

    if (candidatos.size() == 0){
        return h.size();
        //Se não houver mais candidatos, não dá para colorir mais, a resposta é o tamanho de h
    }
    //Randomização da escolha do candidato para V (evita ficar preso em regiões)
    uniform_int_distribution<> dist(0, candidatos.size()-1);
    v = candidatos[dist(gen)];


    //Tornando V feliz verificando os ao redor e ajustando os seus graus: essencial, pois grau é o custo de deixar o vértice feliz dado K
    
    for (int i = 0; i< g[v].size();i++){  //para cada vizinho de v, para cada vizinho que eles tem em comum, reduzir o grau do vizinho em 1
        if (g[v][i] == 1){ //Se v já tinha sido colorido na etapa anterior, não precisa reavaliar vizinhos de v
            if (colorido[v] != 1){
                coloridos[i] = 1;
                grausnovo[i]--; //tira 1 grau pois o vértice v ficou feliz (caso o vértice v já não fosse colorido)
            }
            if (colorido[i] != 1){ //se o vértice i já não tiver sido colorido
                for (int j = 0; j<g[i].size();j++){ // A avaliação dos vizinhos i dos vértices vizinhos ao feliz da vez
                    if (g[i][j]){ 
                        grausnovo[j]--;//tira 1 grau para cada vizinho dos coloridos que deixam v feliz.
                        //Opção extremamente custosa: ver para cada vértice feliz, se v[vf][j] == 1: ainda é uma opção 
                    }
                }
            }
        }
    }
    coloridos[v] = 1;


    grausnovo[v] = 0;
    f.erase(v);//Seja para o caso de que v é feliz ou não é feliz, v não será colorível: a cada chamada, se acaba com um v
    hl.insert(v); // Conjunto que assume que v é feliz
    //Não entendi a parte do bestsol, vou pular. O que a função deveria receber como bestsol?
    if ((hl.size() + f.size()) > (hl.size())){ //Considero v feliz, k-graus[v]+1 pois colori graus[v] +1 vértices pela questão da não redefinição de f, uma chamada a mais, a não ser que v fosse o último
        if (colorido[v] == 1){ //Se essa etapa não coloriu v, o número de vértices coloridos não é k-graus[v]+1 (+1 é o próprio vértice, que já estava colorido)
            bestsol1 = BT(g,grausnovo,k-(graus[v]),hl,hB,f,coloridos);
            }
        else{
            bestsol1 = BT(g,grausnovo,k-(graus[v]+1),hl,hB,f,coloridos);
            }
    }
    else{bestsol1 = hl.size();} //Se hl.size() + f.size() for igual à 0, f.size() é zero, o que quer dizer que a solução ótima contendo v é o tamanho do hl, não tem como melhorar.
    if ((h.size()+f.size())>(hl.size())){//Considero que v não vai ser feliz, hB contém v.
        hB.insert(v);
        bestsol2 = BT(g,graus,k,h,hB,f,colorido); //f já não vai conter
    }
    else{bestsol2=h.size();} 
    return max(bestsol1,bestsol2);

}
int felicidade_maxima(grafo g,vector<int> graus, int k){
    unordered_set<int> h;
    unordered_set<int> hB;
    unordered_set<int> coloriveis;
    vector<bool> coloridos(g.size(),0);
    for (int i = 0;i<graus.size();i++){
        if (graus[i] > k){
            hB.insert(i) ;
        }else{
            coloriveis.insert(i);
        }
    }
    return BT(g,graus,k,h,hB,coloriveis,coloridos);
}
//A lógica do algoritmo é a cada "etapa" tornar um vértice feliz: pagar o custo disso
//Assumir que h é conjunto dos vértices coloridos
//Primeira linha: interpretação: v é ( o vértice associado a) o argumento mínimo entre os vértices não coloridos
//Levando em consideração que a soma dos graus de i é menor ou igual à quantidade de vértices à serem coloridos - a quantidade de vértices já coloridos
//Na 1 etapa se colore v e os seus vizinhos.

//Segunda linha: h/: vértices não coloríveis: são os vértices que dado K e a configuração atual já não são coloríveis

//Terceira linha: f: vértices coloríveis: todos os vértices que dado K e a configuração atual são coloríveis

// A lógica é tentar a possibilidade mais lógica: se h'+ f for maior que h', der para colorir: colore com essa possibilidade
//Se h+f for maior do que h', tenta também: tenta as duas possibilidades: Caso h' for a melhor possibilidade, vai parar

int main(){
    grafo g;
    vector<int> graus; 
    tie(g,graus)= criador_grafos(6,6);
    int b = felicidade_maxima(g,graus,4);
    return 0;
}


//Mais tarde verificar e ajustar o código: hoje depois rosa e grafos.
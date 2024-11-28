#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include "gera_grafos.hpp"
using namespace std;

    //Boa opção é definir o primeiro f e o primeiro hB antes de começar a execução: resolver agora isso

    //Dúvida no bestsol inicial
    //h são os felizes, hB são não felizes, f são os felizíveis: preciso do hB
    //Grau nesse código acaba sendo algo como o custo de pintar: preciso garantir o bom funcionamento da revisão dos graus: se isso funcionar, fechou. se não, pensar na redefinição do hb e f
    //O que fazer: acertar o redefinir graus(grausl), redefinir o hB e f para o que não contém v: Solução para o redefinir parece simples, mas custo alto. Preciso redefinir o hB e f? Nesses casos, o que acontece não é que o chamado mesmo com o f e hB errado são redefinidos, e o candidato é escolhido corretamente? talvez tenha uma chamada a mais, mas e daí?
    //Acho que dá certo pois o que contém v procura os novos candidatos corretamente, assim como o que não contém v: minha dúvida e se a análise das condições vai fazer sentido (acho que não). pensar anas analsies de condições, gastar uns 15 minutos focando entendendo o código
    //Erro de lógica: não faz sentido os dois f's serem iguais, os coloridol dado que v é feliz e dado q v não é feliz são diferentes: escolher candidatos, redefinir graus, redefinir hB e f para o não contendo: terá um hBL e um fL (um para v feliz e outro para v não feliz)
int BT (grafo g, vector<int>graus, int k, unordered_set<int> h, unordered_set<int> hB, unordered_set<int> f, vector<bool> colorido = {},int bestsol = 0){
    int v = -1;
    int graumin = g.size()+1;
    vector<int> auxiliar;
    vector<int> candidatos(0);
    //Todas as estruturas a seguir são implementadas para armazenar os conjuntos reavaliados após colorir o candidato
    unordered_set<int> hl= h;
    vector<int> grausl = graus;
    vector<bool> coloridol = colorido; //Cópia dos coloridol pelo passo anterior, para poder tanto passar o parâmetro de um v colorido quanto de um v não colorido
    unordered_set<int> fl = f;
    unordered_set<int> hBl = hB;
    mt19937 gen(random_device{}());
    //Obtendo os candidatos à v: os de menor custo/grau
    for (auto it = f.begin(); it != f.end(); ) {
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



    //Randomização da escolha do candidato para V (evita ficar preso em regiões)
    uniform_int_distribution<> dist(0, candidatos.size()-1);
    v = candidatos[dist(gen)];
    if (colorido[v] ==1){//Considera se v foi colorido nessa etapa ou em uma anterior
        if(k-graus[v] < 0){
                return bestsol;
            }
    }else{
        if ((k-(graus[v]+1)<0)){
            return bestsol;
            }
        }
    fl.erase(v); //v é colorido para esse conjunto
    hl.insert(v); // Conjunto que assume que v é feliz
    hBl.insert(v);


    //Tornando V feliz verificando os ao redor e ajustando os seus graus: essencial, pois grau é o custo de deixar o vértice feliz dado K
    
    for (int i = 0; i< g[v].size();i++){  //para cada vizinho de v e para cada vizinho dos vizinhos de v, reduzir o grau do vizinho em 1
        if (g[v][i] == 1){ //Se v já tinha sido colorido na etapa anterior, não precisa reavaliar vizinhos de v
            if (colorido[v] != 1){
                grausl[i]--; //tira 1 grau pois o vértice v ficou feliz (caso o vértice v já não fosse colorido)
            }
            if (colorido[i] != 1){ //se o vértice i já não tiver sido colorido por uma outra etapa
                coloridol[i] = 1; //colore ele, e colore os vizinhos se eles ja nao forem coloridos
                for (int j = 0; j<g[i].size();j++){ // A avaliação dos vizinhos i dos vértices vizinhos ao feliz da vez
                    if (g[i][j] == 1){
                        grausl[j]--;//tira 1 grau para cada vizinho dos coloridol que deixam v feliz.
                    }
                }
            }
        }
    }
    coloridol[v] = 1;
    grausl[v] = 0;
    f.erase(v);//Seja para o caso de que v é feliz ou não é feliz, v não será colorível: a cada chamada, se acaba com um v
    hB.insert(v);
    //Redefinind os f e hB para o caso de que v foi feito feliz
    if (colorido[v] ==1){//Considera se v foi colorido nessa etapa ou em uma anterior
        for (auto it = f.begin(); it != f.end(); ) {
            if (grausl[*it]>(k-graus[v])){
                fl.erase(*it);
                hBl.insert(*it);
                }
            ++it;
            }
    }
    else{
        for (auto it = f.begin(); it != f.end(); ) {
            if (grausl[*it]>(k-(graus[v]+1))){
                fl.erase(*it);
                hBl.insert(*it);
                }
            ++it;
            }
    }

    bestsol = max(bestsol,static_cast<int>(hl.size()));
    if ((hl.size() + fl.size()) > (bestsol)){ //Considero v feliz, k-graus[v]+1 pois colori graus[v] +1 vértices pela questão da não redefinição de f, uma chamada a mais, a não ser que v fosse o último
        if (colorido[v] == 1){ //Se essa etapa não coloriu v, o número de vértices coloridol não é k-graus[v]+1 (+1 é o próprio vértice, que já estava colorido)
            bestsol = max(bestsol, (BT(g,grausl,k-(graus[v]),hl,hBl,fl,coloridol,bestsol)));
            }
        else{
            bestsol = max(bestsol,(BT(g,grausl,k-(graus[v]+1),hl,hBl,fl,coloridol,bestsol)));
            }
        }//O caso acima considera v feliz, de duas formas: v já estava colorido por uma etapa anterior ou não
        
        if ((h.size()+f.size())>(bestsol)){
            bestsol = max(bestsol,(BT(g,graus,k,h,hB,f,colorido,bestsol))); 
        }//não precisa dos dois casos, não tornei ninguém feliz para essa etapa

    return bestsol;
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
    return BT(g,graus,k,h,hB,coloriveis,coloridos,0);
}
//A lógica do algoritmo é a cada "etapa" tornar um vértice feliz: pagar o custo disso
//Assumir que h é conjunto dos vértices coloridol
//Primeira linha: interpretação: v é ( o vértice associado a) o argumento mínimo entre os vértices não coloridol
//Levando em consideração que a soma dos graus de i é menor ou igual à quantidade de vértices à serem coloridol - a quantidade de vértices já coloridol
//Na 1 etapa se colore v e os seus vizinhos.

//Segunda linha: h/: vértices não coloríveis: são os vértices que dado K e a configuração atual já não são coloríveis

//Terceira linha: f: vértices coloríveis: todos os vértices que dado K e a configuração atual são coloríveis

// A lógica é tentar a possibilidade mais lógica: se h'+ f for maior que h', der para colorir: colore com essa possibilidade
//Se h+f for maior do que h', tenta também: tenta as duas possibilidades: Caso h' for a melhor possibilidade, vai parar

int main(){
    grafo g;
    vector<int> graus; 
    tie(g,graus)= criador_grafos(6,9);
    int b = felicidade_maxima(g,graus,2);
    cout << b<< endl;
    return 0;
}

//rever e passar para o git de manhazinha: testar outros casos.

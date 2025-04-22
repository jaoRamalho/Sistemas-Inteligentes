/*
    PENSANDO EM CAPSLOCK!!!!!!!!!

    - ENUNCIANDO O PRBLEMA:
        - DADO UM CONJUNTO DE CIDADES E AS DISTANCIAS ENTRE CADA PAR DE CIDADES, DETERMINAR O CAMINHO
        MAIS CURTO ONDE PASSO EXATAMENTE UMA VEZ EM CADA CIDADE E VOLTO PARA A CIDADE DE ORIGEM.abort()

    - MODELANDO:
        - VAMOS POSSUIR UM GRAFO COMPLETO COM ARESTAS BIDIRECIONAIS COM PESOS ALEATÓRIOS ENTRE 1 E N
        - VAMOS APLICAR DIFERENTES MANEIRAS PARA BUSCAR O CAMINHO HALMILTONIANO MAIS CURTO
        - TEMPERA SIMULADA 
        - ALGUMA VARIAÇÃO DO ALGORITMO GENÉTICO
        - BUSCA LOCAL




    pseudo algoritimo tempera simulada:

        melhorRota ← rotaAtual
        melhorCusto ← custoAtual
        T ← T0
        iter ← 0

        enquanto (T > Tmin e iter < maxIter):
            vizinho ← twoOpt(rotaAtual) // troca aleatória de dois vértices
            custoViz ← custo(vizinho)  // calcula o custo da nova rota
            Δ ← custoViz – custoAtual  // diferença de custo entre a nova e a atual
            se (Δ ≤ 0 ou rand() < exp(−Δ/T)): // aceita a nova solução
                rotaAtual ← vizinho // atualiza a rota
                custoAtual ← custoViz // atualiza o custo
                se (custoAtual < melhorCusto): // se a nova rota é melhor que a melhor encontrada
                    melhorRota ← rotaAtual // atualiza a melhor rota
                    melhorCusto ← custoAtual // atualiza o melhor custo
            fim se 
            T ← α * T // resfriamento exponencial
            iter++  // incrementa o número de iterações
        fim enquanto

*/

#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "Vertice.hpp"
#include <vector>
#include <random>
#include <string>
#include <iostream>


class Grafo {
private:
    std::vector<Vertice> vertices;
    Grafo* grafo_rota;
    std::string grafoString;
    std::string nomeGrafo;
    double custo;

public:
    Grafo(std::string nome);
    Grafo(int n_vertices, std::string nome, int minPeso = 1, int maxPeso = 10);

    void gerarGrafoCompleto(int n_vertices, int minPeso = 1, int maxPeso = 10);
    
    void imprimirGrafo();
    void salvarGrafo();
   
    bool inserirAresta(int origem, int destino, int peso);
    bool removerAresta(int origem, int destino);
    bool adicionarVertice(int id);
    double calcularCusto(std::vector<int>& solucao);

    std::vector<int> gerarSolucaoVizinha(std::vector<int>& rotaAtual);

    std::vector<int> TemperaSimulada(int origem_destino);
    std::vector<int> buscaNN(int origem_destino);

    void criarGrafoRota(std::vector<int>& rota, double custo);

    double resfriamento(double T, double alpha, int iter);

    std::vector<int> buscaGenetica();
    void swap_position(std::vector<int>& rota, int i, int j);
    std::vector<int> gerarNovoEstado(std::vector<int>& solucaoAtual);
    std::vector<int> criar3Opt(std::vector<int>& rota);
    std::vector<int> criar2Opt(std::vector<int>& rota);

};

#endif

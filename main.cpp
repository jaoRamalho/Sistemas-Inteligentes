#include <iostream>
#include "Grafo.hpp"

int main() {
    int n_vertices, minPeso, maxPeso;
    n_vertices = 50;
    minPeso = 1;
    maxPeso = 100;
    std::string nomeGrafo = "grafo_teste1";
    
    Grafo g(n_vertices, nomeGrafo, minPeso, maxPeso);
    g.gerarGrafoCompleto(n_vertices, minPeso, maxPeso);
    //g.imprimirGrafo();
    //g.salvarGrafo();

     
    std::vector<int> nn =  g.buscaNN(0);
    // for (auto i : nn) {
    //     std::cout << i << " ";
    // }
   
    // std::cout << std::endl;
    std::cout << "Custo: " << g.calcularCusto(nn) << std::endl;
   
    std::vector<int> bT = g.TemperaSimulada(0);
    // for (auto i : bT) {
    //     std::cout << i << " ";
    // }
    // std::cout << std::endl;
    std::cout << "Custo: " << g.calcularCusto(bT) << std::endl;

    return 0;
}

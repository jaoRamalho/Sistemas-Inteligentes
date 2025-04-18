#include <iostream>
#include "Grafo.hpp"

int main() {
    int n_vertices, minPeso, maxPeso, maxConexoesExtras;
    n_vertices = 10;
    minPeso = 1;
    maxPeso = 10;
    maxConexoesExtras = 4;
    std::string nomeGrafo = "grafo_aleatorio";
    
    Grafo g(n_vertices, nomeGrafo, minPeso, maxPeso, maxConexoesExtras);
    g.gerarGrafoAleatorioConexo(n_vertices, nomeGrafo, minPeso, maxPeso, maxConexoesExtras);
    g.imprimirGrafo();
    g.salvarGrafo();
    std::cout << "Grafo gerado e salvo com sucesso!" << std::endl;

    return 0;
}

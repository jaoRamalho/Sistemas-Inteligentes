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
    std::string grafoString;
    std::string nomeGrafo;

public:
    Grafo(int n_vertices, std::string nome, int minPeso = 1, int maxPeso = 10, int maxConexoesExtras = 4);

    void gerarGrafoAleatorioConexo(int n_vertices, std::string nome, int minPeso = 1, int maxPeso = 10, int maxConexoesExtras = 4);
    void imprimirGrafo();
    void salvarGrafo();
};

#endif

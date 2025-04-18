#include "Vertice.hpp"

Vertice::Vertice(int id) : id(id) {}

void Vertice::adicionarAresta(int destino, int peso) {
    adjacentes.emplace_back(destino, peso);
}

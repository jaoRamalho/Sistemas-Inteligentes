#ifndef VERTICE_HPP
#define VERTICE_HPP

#include <vector>
#include <utility>
#include <string>

class Vertice {
public:
    int id;
    std::vector<std::pair<int, int>> adjacentes; // par (id_destino, peso)

    Vertice(int id);

    void adicionarAresta(int destino, int peso);
};

#endif

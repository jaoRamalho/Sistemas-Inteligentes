#include "Grafo.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <ctime>
#include <sstream>

Grafo::Grafo(int n_vertices, std::string nome, int minPeso, int maxPeso, int maxConexoesExtras) {
    gerarGrafoAleatorioConexo(n_vertices, nome, minPeso, maxPeso, maxConexoesExtras);
    nomeGrafo = nome;
}

void Grafo::gerarGrafoAleatorioConexo(int n, std::string nome, int minPeso, int maxPeso, int maxConexoesExtras) {
    vertices.clear();
    for (int i = 0; i < n; ++i) {
        vertices.emplace_back(i);
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> peso_dist(1, n);

    // Garante que o grafo será conectado: cria uma árvore aleatória
    for (int i = 1; i < n; ++i) {
        int destino = i;
        std::uniform_int_distribution<int> origem_dist(0, i - 1);
        int origem = origem_dist(rng);
        int peso = peso_dist(rng);
        vertices[origem].adicionarAresta(destino, peso);
    }

    // Adiciona arestas extras aleatórias com maior variação 
    std::uniform_int_distribution<int> vert_dist(0, n - 1);
    // Exemplo: de 0 a 4 conexões extras por vértice
    std::uniform_int_distribution<int> extras_dist(0, 4);
    for (int i = 0; i < n; ++i) {
        int conexoesExtras = extras_dist(rng);
        for (int j = 0; j < conexoesExtras; ++j) {
            int origem = i;
            int destino = vert_dist(rng);
            if (origem != destino) {
                int peso = peso_dist(rng);
                vertices[origem].adicionarAresta(destino, peso);
            }
        }
    }
}

void Grafo::imprimirGrafo() {
    for (const auto& v : vertices) {
        std::cout << "Vertice " << v.id << " -> ";
        for (const auto& [dest, peso] : v.adjacentes) {
            std::cout << "(" << dest << ", peso: " << peso << ") ";
        }
        std::cout << '\n';
    }
}

void Grafo::salvarGrafo()
{
    std::ostringstream oss;
    for (const auto& v : vertices) {
        oss << "Vertice " << v.id << " -> ";
        for (const auto& [dest, peso] : v.adjacentes) {
            oss << "(" << dest << ", peso: " << peso << ") ";
        }
        oss << '\n';
    }
    grafoString = oss.str();

    std::ofstream arquivo(nomeGrafo + ".txt");
    if (arquivo.is_open()) {
        arquivo << grafoString;
        arquivo.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo para salvar o grafo." << std::endl;
    }
    std::cout << "Grafo salvo em " << nomeGrafo << ".txt" << std::endl;
}

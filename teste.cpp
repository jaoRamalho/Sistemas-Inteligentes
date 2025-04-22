#include <iostream>
#include <random>
#include <cmath>

int main() {
    // Cria o gerador de números aleatórios
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Distribuição uniforme entre 0 e 1
    std::uniform_real_distribution<double> uniform01(0.0, 1.0);
    
    // Gera e imprime 1000 números, transformando U em um número enviesado entre 0 e 500
    for (int i = 0; i < 2000; ++i) {
        double u = uniform01(gen);
        // Elevar U a um expoente > 1 favorece valores menores.
        int numeroAleatorio = 500.0 * std::pow((1 - u), 1.4);
        std::cout << numeroAleatorio << std::endl;
    }
    
    return 0;
}
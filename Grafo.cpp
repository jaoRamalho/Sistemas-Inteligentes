#include "Grafo.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <ctime>
#include <sstream>
#include <algorithm> // para std::find_if
#include <set>
#include <utility>


Grafo::Grafo(std::string nome) : grafoString(""), nomeGrafo(nome), grafo_rota(nullptr), custo(0.0) {


}



Grafo::Grafo(int n_vertices, std::string nome, int minPeso, int maxPeso) : grafo_rota(nullptr)
{
    gerarGrafoCompleto(n_vertices, minPeso, maxPeso);
    nomeGrafo = nome;
    custo = 0;
}



void Grafo::gerarGrafoCompleto(int n, int minPeso, int maxPeso) {
    vertices.clear();
    for (int i = 0; i < n; ++i) {
        vertices.emplace_back(i);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> pesoDist(minPeso, maxPeso);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int peso = pesoDist(gen);
            vertices[i].adicionarAresta(j, peso);
            vertices[j].adicionarAresta(i, peso); // Para grafo não direcionado
        }
    }
    std::cout << "Grafo completo gerado com " << n << " vértices." << std::endl;
   
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



bool Grafo::inserirAresta(int origem, int destino, int peso)
{
    // Procura os vértices com os IDs passados
    auto itOrig  =  std::find_if(vertices.begin(), vertices.end(), 
                    [origem](const Vertice& v) { return v.id == origem; });
    auto itDest  =  std::find_if(vertices.begin(), vertices.end(), 
                    [destino](const Vertice& v) { return v.id == destino; });

    if (itOrig == vertices.end() || itDest == vertices.end()) {
        std::cerr << "Vértice(s) não existente(s): origem " << origem 
                    << " ou destino " << destino << std::endl;
        return false;
    }

    if (origem == destino) {
        std::cerr << "Não é possível adicionar uma aresta para o mesmo vértice." << std::endl;
        return false;
    }

    itOrig->adicionarAresta(destino, peso);
    itDest->adicionarAresta(origem, peso); // Para grafo não direcionado
    return true;
}



bool Grafo::removerAresta(int origem, int destino){
    if (origem < 0 || origem >= vertices.size() || destino < 0 || destino >= vertices.size()) {
        std::cerr << "Índices de origem ou destino inválidos." << std::endl;
        return false;
    }

    if (origem == destino) {
        std::cerr << "Não é possível remover uma aresta para o mesmo vértice." << std::endl;
        return false;
    }

    // talvez implementar um método para remover arestas
    return true;
}



bool Grafo::adicionarVertice(int id){
    if (id < 0) {
        std::cerr << "ID do vértice inválido." << std::endl;
        return false;
    }

    for (const auto& v : vertices) {
        if (v.id == id) {
            std::cerr << "Vértice com ID " << id << " já existe." << std::endl;
            return false;
        }
    }

    vertices.emplace_back(id);
    return true;
}



double Grafo::calcularCusto(std::vector<int> &solucao){
    double retorno = 0.0;
    for (size_t i = 0; i < solucao.size() - 1; ++i) {
        int org = solucao[i];
        int dest = solucao[i + 1];
        bool arestaEncontrada = false;

        for(auto& aresta : vertices[org].adjacentes) {
            if(aresta.first == dest) {
                retorno += aresta.second;
                arestaEncontrada = true;
                break;
            }
        }

        if(!arestaEncontrada) {
            std::cerr << "Aresta não encontrada entre " << org << " e " << dest << std::endl;
            return -1; // ou outro valor de erro
        }
    }

    return retorno;
}

std::vector<int> Grafo::gerarSolucaoVizinha(std::vector<int> &rota)
{
    std::vector<int> novaRota = rota; // cria uma cópia da rota atual

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, rota.size() - 1);

    int i = dist(gen); // gera um índice aleatório
    int k = dist(gen); // gera outro índice aleatório
    while (i == k ) {
        k = dist(gen); // garante que os índices sejam diferentes
    }

    // Garante que i seja menor que k para facilitar a inversão
    if (i > k) {
        std::swap(i, k);
    }

    // Aplica a estratégia 2-opt: inverte a subsequência entre i e k
    std::reverse(novaRota.begin() + i, novaRota.begin() + k + 1);

    return novaRota; // retorna a nova rota
}



std::vector<int> Grafo::TemperaSimulada(int origem_destino) {
    std::vector<int> rota = buscaNN(origem_destino); // Inicializa com a melhor solução inicial
    double custoAtual = calcularCusto(rota); // Calcula o custo da rota inicial
    double melhorCusto = custoAtual; // Inicializa o melhor custo com o custo atual
    std::vector<int> melhorRota = rota; // Inicializa a melhor rota com a rota atual

    const double alpha = 0.95; // Taxa de resfriamento
    const double T0 = 1.0; // Temperatura inicial
    const double Tmin = 0.001; // Temperatura mínima
    const int maxIter = 1000; // Número máximo de iterações
    const int tamanhoRota = vertices.size(); // Tamanho da rota (número de vértices)

    double T = T0; // Inicializa a temperatura
    int iter = 0; // Inicializa o contador de iterações

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> uniform01(0.0, 1.0); // Distribuição uniforme entre 0 e 1

    while (iter < maxIter && T > Tmin) {
        // Gera uma nova solução vizinha
        std::vector<int> novaRota = criar3Opt(rota);
        double custoNovaRota = calcularCusto(novaRota);

        // Se a nova solução for melhor, aceita.
        if (custoNovaRota < custoAtual) {
            rota = novaRota;
            custoAtual = custoNovaRota;

            // Atualiza a melhor solução encontrada
            if (custoNovaRota < melhorCusto) {
                melhorRota = novaRota;
                melhorCusto = custoNovaRota;
            }
        } else {
            // Calcula a probabilidade de aceitar uma solução pior
            double delta = custoNovaRota - custoAtual;
            double probabilidade = exp(-delta / (T * tamanhoRota) );
            if (uniform01(gen) < probabilidade) {
                rota = novaRota;
                custoAtual = custoNovaRota;
            }
        }
        
        // Atualiza a temperatura
        T *= alpha;
        iter++;
    }

    return melhorRota; // Retorna a melhor rota encontrada
}




std::vector<int> Grafo::buscaNN(int origem_destino)
{
    int n = vertices.size();
    if(n == 0 || origem_destino < 0 || origem_destino >= n) {
        std::cerr << "Grafo vazio ou índice de origem/destino inválido." << std::endl;
        return std::vector<int>(); // retorna vetor vazio
    }

    int contadorExec = 0;
    int contadorOtimo = 0; 
    std::vector<bool> visitados(n, false);
    std::vector<int> rota;

    int atual = origem_destino;
    visitados[atual] = true;
    rota.push_back(atual);
    int totalCusto = 0;

    // Loop principal: cada iteração é considerada uma exploração
    while(rota.size() < n) {
        int proximo = -1;
        int menorPeso = INT_MAX;
        // Loop para contar a complexidade: percorre os vizinhos do vértice atual
        for(int i = 0; i < vertices[atual].adjacentes.size(); ++i) {
            int dest = vertices[atual].adjacentes[i].first;
            int peso = vertices[atual].adjacentes[i].second;
            if(!visitados[dest] && peso < menorPeso) {
                contadorOtimo++; // conta cada iteração do loop principal
                menorPeso = peso;
                proximo = dest;

            }
            contadorExec++; // conta cada iteração do loop interno
        }
        
        // Se não encontrar próximo, a rota não é possível (teoricamente não acontece em grafo completo)
        if(proximo == -1) {
            std::cerr << "Erro: não foi possível encontrar próximo vértice." << std::endl;
            break;
        }

        rota.push_back(proximo);

        visitados[proximo] = true;
        totalCusto += menorPeso;
        atual = proximo;
    }

    return rota;
}

void Grafo::criarGrafoRota(std::vector<int> &rota, double custo){
    if(grafo_rota != nullptr) {
        delete grafo_rota; // libera memória do grafo anterior
    }
    grafo_rota = nullptr; // inicializa o ponteiro para evitar dangling pointer

    grafo_rota = new Grafo(nomeGrafo + "_rota"); // cria um novo grafo para a rota
    for (int i = 0; i < rota.size() - 1; ++i) {
        grafo_rota->adicionarVertice(rota[i]); // adiciona os vértices da rota
    }

    // Insere as arestas correspondentes à rota
    for (size_t i = 0; i < rota.size() - 1; ++i) {
        int origem = rota[i];
        int destino = rota[i + 1];
        int pesoAresta = -1;
        // Procura na lista de adjacência do vértice 'origem' a aresta que vai para 'destino'
        for (const auto& aresta : vertices[origem].adjacentes) {
            if (aresta.first == destino) {
                pesoAresta = aresta.second;
                break;
            }
        }
        if (pesoAresta == -1) {
            std::cerr << "Aresta não encontrada entre " << origem << " e " << destino << std::endl;
        } else {
            grafo_rota->inserirAresta(origem, destino, pesoAresta);
        }
    }

    std::cout << "Grafo da rota criado com custo: " << custo << std::endl;
    grafo_rota->imprimirGrafo(); // imprime o grafo da rota
    grafo_rota->salvarGrafo(); // salva o grafo da rota em um arquivo
    std::cout << "Grafo da rota salvo em " << nomeGrafo << "_rota.txt" << std::endl;

}

double Grafo::resfriamento(double T, double alpha, int iter)
{
    return T * alpha; // resfriamento exponencial
}

std::vector<int> Grafo::buscaGenetica(){
    // criar população inicial com individuos indivíduos
    int individuos = 500;
    std::vector<std::pair<std::vector<int>, double>> populacao;
    std::vector<int> bg = buscaNN(0); // busca o melhor caminho inicial
    double custoInicial = calcularCusto(bg);
    populacao.push_back(std::make_pair(bg, custoInicial)); // adiciona o melhor caminho inicial na população
    for (int i = 1; i < individuos; ++i) {

        std::vector<int> individuo;
        individuo.push_back(0); // adiciona o vértice inicial (0) na primeira posição
        for (int j = 1; j < vertices.size(); ++j) {
            individuo.push_back(j);
        }

        std::random_shuffle(individuo.begin(), individuo.end());
        double custo = calcularCusto(individuo);
        populacao.push_back(std::make_pair(individuo, custo));
    }

    // ordenar a população pelo custo
    std::sort(populacao.begin(), populacao.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> uniform01(0.0, 1.0);
    
    // cruzar os melhores indivíduos para criar a nova população
    std::vector<std::pair<std::vector<int>, double>> nova_populacao;
    // Suponha que "populacao" já foi criada e ordenada
    std::set<std::pair<int, int>> paresUsados; // Armazena os pares já selecionados (na ordem escolhida)
   
   
    double melhorCusto = populacao[0].second;
    double piorCusto = populacao[populacao.size() - 1].second;
    double difCusto = (piorCusto - melhorCusto)/piorCusto; // diferença percentual entre o melhor e o pior custo

    double custoObjetivo = 0.0;
    int count_populacao = 0;
    double indiceSaturacao = 0.0;

    //while ainda não está pronto
    bool continua = true;
    std::ofstream arquivoCustos("custos.csv", std::ios::app); // abre em modo append
    
    if(arquivoCustos.is_open()){
        for (int i = 0; i < individuos; ++i) {
            arquivoCustos << "Populacao" << i << ",";
        }
        arquivoCustos << "\n";
    } else {
        std::cerr << "Erro ao abrir o arquivo de custos." << std::endl;
    }


    while(continua){
        if (arquivoCustos.is_open()) {
            for (int i = 0; i < populacao.size(); ++i) {
                double custo = populacao[i].second;
                arquivoCustos << custo << ","; 
            }
            arquivoCustos << "\n";
        }

        continua &= (!(melhorCusto <= custoObjetivo));
        if (!continua) { std::cout << "Custo objetivo atingido: " << melhorCusto << std::endl; }
        continua &= (!(indiceSaturacao >= 0.80)); // 90% de saturação
        if (!continua) { std::cout << "Saturação atingida: " << indiceSaturacao << std::endl; }        
        continua &= (!(count_populacao >= 1000)); // 1000 iterações
        if (!continua) { std::cout << "Número máximo de iterações atingido: " << count_populacao << std::endl;}
        continua &= (!(difCusto <= 0.05)); // diferença de custo menor que 1%
        if (!continua) { std::cout << "Diferença de custo atingida: " << difCusto << std::endl; }
        
        paresUsados.clear(); // limpa os pares usados a cada iteração
        // deixa a elite
        for (int i = 0; i < individuos * 0.1; ++i) {
            nova_populacao.push_back(populacao[i]);
        }

        for (int i = 50; i < individuos; i++) {
            int posicao_pai = individuos * std::pow(1 - uniform01(gen), 1.414);
            int posicao_mae = individuos * std::pow(1 - uniform01(gen), 1.414);

            while (posicao_pai == posicao_mae) {
                posicao_mae = individuos * std::pow(1 - uniform01(gen), 1.414);
            }
            
            std::pair<int, int> parEscolhido = std::make_pair(posicao_pai, posicao_mae);
            // Se esse par já foi usado (na mesma ordem), gera novos índices
            while (paresUsados.find(parEscolhido) != paresUsados.end()) {
                posicao_pai = individuos * std::pow(1 - uniform01(gen), 1.41);
                posicao_mae = individuos * std::pow(1 - uniform01(gen), 1.41);
                while (posicao_pai == posicao_mae) {
                    posicao_mae = individuos * std::pow(1 - uniform01(gen), 1.41);
                }
                parEscolhido = std::make_pair(posicao_pai, posicao_mae);
            }
            
            // Registra o par para não usá-lo novamente
            paresUsados.insert(parEscolhido);

            std::pair<std::vector<int>, double> pai = populacao[posicao_pai];
            std::pair<std::vector<int>, double> mae = populacao[posicao_mae];

            std::vector<int> filho;
            int n = pai.first.size();
            std::vector<bool> usado(n, false);

            // Para cada posição, tenta usar o gene do pai ou da mãe (de forma aleatória)
            for (int j = 0; j < n; ++j) {
                int gene;
                if (rand() % 2 == 0) {
                    gene = pai.first[j];
                } else {
                    gene = mae.first[j];
                }
                // Se esse gene ainda não foi usado, adiciona-o...
                if (!usado[gene]) {
                    filho.push_back(gene);
                    usado[gene] = true;
                } else {
                    // Caso já tenha sido adicionado, tenta o outro gene na mesma posição
                    int outroGene = (gene == pai.first[j]) ? mae.first[j] : pai.first[j];
                    if (!usado[outroGene]) {
                        filho.push_back(outroGene);
                        usado[outroGene] = true;
                    } else {
                        // Se ambos já foram usados, percorre os vértices e adiciona o primeiro que faltar
                        for (int k = 0; k < n; ++k) {
                            if (!usado[k]) {
                                filho.push_back(k);
                                usado[k] = true;
                                break;
                            }
                        }
                    }
                }
            }

            // mutação
            if (rand() % 100 < 5) {
                int pos1 = rand() % filho.size();
                int pos2 = rand() % filho.size();
                std::swap(filho[pos1], filho[pos2]);
            }

            nova_populacao.push_back(std::pair<std::vector<int>, double>(filho, calcularCusto(filho)));
        }

        // ordenar a população pelo custo (tentando fazer uma elitização)
        std::sort(nova_populacao.begin(), nova_populacao.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        melhorCusto = nova_populacao[0].second;
        piorCusto = nova_populacao[nova_populacao.size() - 1].second;
        difCusto = (piorCusto - melhorCusto)/piorCusto; // diferença percentual entre o melhor e o pior custo
        //custoObjetivo = melhorCusto; // atualiza o custo objetivo
        count_populacao++; // incrementa o contador de iterações
        
        populacao = nova_populacao; // atualiza a população para a nova geração
        nova_populacao.clear(); // limpa a nova população para a próxima iteração
    } 

    arquivoCustos.close(); // fecha o arquivo de custos
    return populacao[0].first; // retorna o melhor indivíduo encontrado
}

void Grafo::swap_position(std::vector<int> &rota, int i, int j)
{
    if (i < 0 || i >= rota.size() || j < 0 || j >= rota.size()) {
        std::cerr << "Índices inválidos para troca." << std::endl;
        return;
    }
    std::swap(rota[i], rota[j]);
}

std::vector<int> Grafo::gerarNovoEstado(std::vector<int> &solucaoAtual) {
    std::vector<int> novaSolucao = solucaoAtual; // Cria uma cópia da solução atual

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, solucaoAtual.size() - 1);

    int i = dist(gen); // Gera um índice aleatório
    int j = dist(gen); // Gera outro índice aleatório

    // Garante que os índices sejam diferentes
    while (i == j) {
        j = dist(gen);
    }

    // Troca os vértices nas posições i e j
    std::swap(novaSolucao[i], novaSolucao[j]);


    return novaSolucao; // Retorna a nova solução gerada
}

std::vector<int> Grafo::criar2Opt(std::vector<int> &rota) {
    std::vector<int> retorno = rota; // Cria uma cópia da rota atual

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, rota.size() - 1); // Gera índices entre 1 e n-1

    int i = dist(gen); // Gera um índice aleatório
    int k = dist(gen); // Gera outro índice aleatório
    while (i == k) {
        k = dist(gen); // Garante que os índices sejam diferentes
    }

    // Garante que i seja menor que k para facilitar a inversão
    if (i > k) {
        std::swap(i, k);
    }

    // Aplica a estratégia 2-opt: inverte a subsequência entre i e k
    std::reverse(retorno.begin() + i, retorno.begin() + k + 1);

    return retorno; // Retorna a nova rota
    
}

std::vector<int> Grafo::criar3Opt(std::vector<int> &rota) {
    int n = rota.size();
    std::vector<int> novaRota = rota;

    // Seleciona três índices aleatórios i, j, k
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, n - 1);

    int i = dist(gen);
    int j = dist(gen);
    int k = dist(gen);

    // Garante que i < j < k
    if (i > j) std::swap(i, j);
    if (j > k) std::swap(j, k);
    if (i > j) std::swap(i, j);

    // Aplica a estratégia 3-opt: cria uma nova rota com reconexão dos segmentos
    std::reverse(novaRota.begin() + i, novaRota.begin() + j + 1); // Reverte o segmento [i, j]
    std::reverse(novaRota.begin() + j + 1, novaRota.begin() + k + 1); // Reverte o segmento [j+1, k]

    return novaRota; // Retorna a nova rota gerada
}
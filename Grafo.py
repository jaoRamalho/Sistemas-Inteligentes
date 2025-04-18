import sys
import networkx as nx
import matplotlib.pyplot as plt
import re

# Lê o arquivo .txt que contém a definição do grafo.
# Você pode passar o caminho do arquivo como argumento ou digitá-lo quando solicitado.

file_name = "graforEsparco.txt"

try:
    with open(file_name, 'r', encoding='utf-8') as f:
        entrada = f.read()
except Exception as e:
    print("Erro ao ler o arquivo:", e)
    sys.exit(1)

# Criar o grafo direcionado
G = nx.DiGraph()

# Parse da entrada
for linha in entrada.strip().splitlines():
    if '->' not in linha:
        continue

    partes = linha.split('->')
    origem = int(partes[0].strip().split()[1])
    if len(partes[1].strip()) == 0:
        continue

    arestas = partes[1].strip().split(') ')
    for a in arestas:
        a = a.replace('(', '').replace(')', '')
        if not a:
            continue
        destino_str, peso_str = a.split(', peso:')
        destino = int(destino_str.strip())
        peso = int(peso_str.strip())
        G.add_edge(origem, destino, weight=peso)

# Layout circular para facilitar a visualização
pos = nx.circular_layout(G)
edge_labels = nx.get_edge_attributes(G, 'weight')

plt.figure(figsize=(10, 10))
nx.draw(G, pos, with_labels=True, node_color="lightgreen", node_size=800, font_size=10, font_weight='bold', arrowsize=20)
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_size=8)

plt.title("Grafo Direcionado com Pesos (Layout Circular)", fontsize=14)
plt.show()
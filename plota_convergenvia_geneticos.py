import pandas as pd
import matplotlib.pyplot as plt

# Lê os dados do arquivo custos.csv
data = pd.read_csv('custos.csv')

# Cria o gráfico
plt.figure(figsize=(12, 6))

# Plota cada coluna (população) como uma curva
for coluna in data.columns:
    plt.plot(data.index, data[coluna], label=coluna)

# Adiciona título e rótulos
plt.title('Curva de Convergência das Populações')
plt.xlabel('Geração')
plt.ylabel('Custo')

# Adiciona legenda
plt.legend(loc='upper right', fontsize='small', ncol=2)

# Exibe o gráfico
plt.grid(True)
plt.tight_layout()
plt.show()
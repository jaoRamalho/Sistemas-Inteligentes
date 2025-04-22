import pandas as pd
import matplotlib.pyplot as plt

# Lê o arquivo ignorando as linhas que começam com '//'
df = pd.read_csv('data.csv', comment='/', skipinitialspace=True)

# Garante que não haja espaços em branco no nome das colunas
df.columns = df.columns.str.strip()

# Plota o histograma dos valores
plt.figure(figsize=(10,6))
plt.hist(df['valor'], bins=20, color='b', edgecolor='black')
plt.title('Histograma de valores do data.csv')
plt.xlabel('Valor')
plt.ylabel('Frequência')
plt.grid(True)
plt.show()
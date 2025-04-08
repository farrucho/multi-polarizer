def ler_dados(ficheiro):
    with open(ficheiro, 'r') as f:
        conteudo = f.read()
    # Separar os números e convertê-los para float
    dados = [float(valor) for valor in conteudo.split()]
    return dados

# Exemplo de uso:
nome_ficheiro = "dados.txt"  # Substituir pelo caminho real do ficheiro
dados = ler_dados(nome_ficheiro)
print(dados)

plt.figure(figsize=(10, 5))
plt.plot(data, marker='o', linestyle='-')
plt.xlabel('Índice')
plt.ylabel('Valor')
plt.title('Gráfico dos Dados Fornecidos')
plt.grid(True)
plt.show()
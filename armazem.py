def menu(estoque):
    while True:
        print("\nEstoque Atual:")
        for produto, quantidade in estoque.items():
            print(f"{produto}: {quantidade}")

        produtos_baixos = [produto for produto, quantidade in estoque.items() if quantidade <= 50]
        if produtos_baixos:
            print("\nProdutos em nível baixo (≤50):")
            for produto in produtos_baixos:
                print(f"{produto}: {estoque[produto]}")
        else:
            print("\nNenhum produto em nível baixo.")

        print("\nMenu de Opções:")
        print("1. Adicionar produto")
        print("2. Sair")
        escolha = input("Escolha uma opção: ")

        if escolha == "1":
            nome_produto = input("Produto: ").strip().lower()
            try:
                quantidade_produto = int(input("Quantidade: "))
                if quantidade_produto < 0: 
                    print("Quantidade inválida.")
                    continue
            except ValueError:
                print("Digite um número inteiro.")
                continue
            estoque[nome_produto] = estoque.get(nome_produto, 0) + quantidade_produto
        elif escolha == "2":
            break
        else:
            print("Opção inválida.")

estoque = {
    "canetas": 150, 
    "cadernos": 95, 
    "borrachas": 60, 
    "lapis": 200,
    "marcadores": 80, 
    "reguas": 40, 
    "mochilas": 30, 
    "estojos": 120
}

movimentacoes = [
    ("canetas", "saída", 25), 
    ("cadernos", "entrada", 10), 
    ("borrachas", "saída", 50),
    ("lapis", "entrada", 100), 
    ("marcadores", "saída", 30), 
    ("reguas", "entrada", 20),
    ("mochilas", "saída", 15),
    ("estojos", "saída", 40),
    ("cola", "entrada", 70),
    ("apontadores", "entrada", 60), 
    ("canetas", "saída", 90), 
    ("cadernos", "saída", 50),
    ("lapis", "saída", 180), 
    ("borrachas", "entrada", 20), 
    ("mochilas", "entrada", 25),
    ("marcadores", "entrada", 15), 
    ("estojos", "saída", 60)
]

for nome_produto, tipo_movimentacao, quantidade_movimentacao in movimentacoes:
    tipo_movimentacao = tipo_movimentacao.lower().replace("í", "i")
    if tipo_movimentacao == "entrada":
        estoque[nome_produto] = estoque.get(nome_produto, 0) + quantidade_movimentacao
    elif tipo_movimentacao == "saida":
        estoque[nome_produto] = max(estoque.get(nome_produto, 0) - quantidade_movimentacao, 0)

menu(estoque)

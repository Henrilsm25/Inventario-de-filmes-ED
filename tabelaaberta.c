#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =========================
// ESTRUTURAS E CONSTANTES
// =========================
typedef struct {
    int id;
    char titulo[50];
    int ano;
} Filme;

typedef struct {
    Filme filme;
    int ocupado; /* 0 = Vazio, 1 = Ocupado, -1 = Removido */
} HashAberto;

// =========================
// FUNÇÕES DA TABELA HASH
// =========================

// Inicializa a tabela hash aberta
void inicializarAberto(HashAberto *tabela, int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++) {
        tabela[i].ocupado = 0; // Define todas as posições como vazias
    }
}

// Função hash para calcular o índice
int hash(int id, int tamanho) {
    return id % tamanho; // Calcula o índice baseado no resto da divisão
}

// Insere um filme na tabela hash aberta
void inserirAberto(HashAberto *tabela, int tamanho, Filme filme) {
    int indice = hash(filme.id, tamanho);
    int inicial = indice;

    // Procura uma posição livre ou com o mesmo ID para inserir
    while (tabela[indice].ocupado == 1) {
        if (tabela[indice].filme.id == filme.id) {
            printf("ID já existe! Atualize o registro em vez de inserir.\n");
            return;
        }
        indice = (indice + 1) % tamanho; // Incrementa para o próximo índice (linear)
        if (indice == inicial) { // Verifica se percorreu toda a tabela
            printf("Tabela cheia! Não foi possível inserir.\n");
            return;
        }
    }

    // Insere o filme na posição encontrada
    tabela[indice].filme = filme;
    tabela[indice].ocupado = 1; // Marca como ocupado
    printf("Filme inserido com sucesso!\n");
}

// Atualiza um filme existente na tabela hash aberta
void atualizarAberto(HashAberto *tabela, int tamanho, Filme filme) {
    int indice = hash(filme.id, tamanho);
    int inicial = indice;

    // Procura o filme pelo ID
    while (tabela[indice].ocupado != 0) { // Continua enquanto a posição não for "vazia"
        if (tabela[indice].ocupado == 1 && tabela[indice].filme.id == filme.id) {
            tabela[indice].filme = filme; // Atualiza o filme
            printf("Filme atualizado com sucesso!\n");
            return;
        }
        indice = (indice + 1) % tamanho; // Incrementa para o próximo índice (linear)
        if (indice == inicial) { // Verifica se percorreu toda a tabela
            break;
        }
    }

    // Caso não encontre o filme
    printf("Filme com ID %d não encontrado para atualização.\n", filme.id);
}

// Remove um filme da tabela hash aberta
void removerAberto(HashAberto *tabela, int tamanho, int id) {
    int indice = hash(id, tamanho);
    int inicial = indice;

    // Procura o filme pelo ID
    while (tabela[indice].ocupado != 0) {
        if (tabela[indice].ocupado == 1 && tabela[indice].filme.id == id) {
            tabela[indice].ocupado = -1; // Marca como removido
            printf("Filme removido com sucesso!\n");
            return;
        }
        indice = (indice + 1) % tamanho; // Incrementa para o próximo índice (linear)
        if (indice == inicial) { // Verifica se percorreu toda a tabela
            break;
        }
    }

    // Caso não encontre o filme
    printf("Filme com ID %d não encontrado para remoção.\n", id);
}

// Lista todos os filmes da tabela hash aberta
void listarAberto(HashAberto *tabela, int tamanho) {
    int i;
    printf("\nLista de Filmes:\n");
    for (i = 0; i < tamanho; i++) {
        if (tabela[i].ocupado == 1) {
            // Exibe os dados do filme na posição ocupada
            printf("Posicao %d -> ID: %d, Titulo: %s, Ano: %d\n",
                   i, tabela[i].filme.id, tabela[i].filme.titulo, tabela[i].filme.ano);
        } else if (tabela[i].ocupado == -1) {
            // Indica que a posição foi removida
            printf("Posicao %d -> Removido\n", i);
        } else {
            // Indica que a posição está vazia
            printf("Posicao %d -> Vazia\n", i);
        }
    }
}

// =========================
// PROGRAMA PRINCIPAL
// =========================

// Exibe o menu do programa
void menu() {
    printf("\nMenu:\n");
    printf("1. Inserir Filme\n");
    printf("2. Atualizar Filme\n");
    printf("3. Remover Filme\n");
    printf("4. Listar Filmes\n");
    printf("0. Sair\n");
}

int main() {
    const int TAMANHO = 20;
    HashAberto tabelaAberta[TAMANHO];
    int opcao, id;
    Filme filme;

    // Inicializa a tabela hash aberta
    inicializarAberto(tabelaAberta, TAMANHO);

    // Laço principal do programa
    do {
        menu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); /* Limpa o buffer do '\n' */

        switch (opcao) {
            case 1:
                // Inserir um novo filme
                printf("Inserir Filme (ID, Titulo, Ano): ");
                scanf("%d", &filme.id);
                getchar(); /* Limpa o buffer do '\n' */
                printf("Titulo: ");
                scanf(" %[^\n]s", filme.titulo);
                printf("Ano: ");
                scanf("%d", &filme.ano);
                inserirAberto(tabelaAberta, TAMANHO, filme);
                break;

            case 2:
                // Atualizar um filme existente
                printf("Atualizar Filme (ID): ");
                scanf("%d", &id);
                getchar(); /* Limpa o buffer do '\n' */
                printf("Novo Titulo: ");
                fgets(filme.titulo, sizeof(filme.titulo), stdin);
                filme.titulo[strcspn(filme.titulo, "\n")] = '\0'; // Remove o '\n'
                printf("Novo Ano: ");
                scanf("%d", &filme.ano);
                filme.id = id;
                atualizarAberto(tabelaAberta, TAMANHO, filme);
                break;

            case 3:
                // Remover um filme
                printf("Remover Filme (ID): ");
                scanf("%d", &id);
                removerAberto(tabelaAberta, TAMANHO, id);
                break;

            case 4:
                // Listar todos os filmes
                listarAberto(tabelaAberta, TAMANHO);
                break;

            case 0:
                // Sair do programa
                printf("Encerrando o programa...\n");
                break;

            default:
                // Opção inválida
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

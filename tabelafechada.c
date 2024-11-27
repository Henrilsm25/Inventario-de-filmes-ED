#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =========================
// ESTRUTURAS E CONSTANTES
// =========================
#define TAMANHO 20

typedef struct Filme {
    int id;
    char titulo[50];
    int ano;
} Filme;

typedef struct Node {
    Filme filme;
    struct Node *proximo;
} Node;

typedef struct {
    Node **buckets;
    int tamanho;
} HashFechado;

// =========================
// FUNÇÕES DA TABELA HASH
// =========================

// Função hash para calcular índice
int hash(int id, int tamanho) {
    return id % tamanho;
}

// Inicializa a tabela hash
void inicializarFechado(HashFechado *tabela, int tamanho) {
    int i;
    tabela->tamanho = tamanho;
    tabela->buckets = (Node **)malloc(tamanho * sizeof(Node *));
    for (i = 0; i < tamanho; i++) {
        tabela->buckets[i] = NULL;
    }
}

// Insere um filme na tabela hash
void inserirFechado(HashFechado *tabela, Filme filme) {
    int indice = hash(filme.id, tabela->tamanho);
    Node *atual = tabela->buckets[indice];
    Node *novo;

    // Verificar se o filme já existe
    while (atual != NULL) {
        if (atual->filme.id == filme.id) {
            printf("ID já existe! Atualize o registro em vez de inserir.\n");
            return;
        }
        atual = atual->proximo;
    }

    // Inserir novo filme no início da lista encadeada
    novo = (Node *)malloc(sizeof(Node));
    if (novo == NULL) {
        printf("Erro de alocação de memória!\n");
        return;
    }
    novo->filme = filme;
    novo->proximo = tabela->buckets[indice];
    tabela->buckets[indice] = novo;

    printf("Filme inserido com sucesso!\n");
}

// Atualiza um filme na tabela hash
void atualizarFechado(HashFechado *tabela, Filme filme) {
    int indice = hash(filme.id, tabela->tamanho);
    Node *atual = tabela->buckets[indice];

    // Procurar o filme na lista encadeada
    while (atual != NULL) {
        if (atual->filme.id == filme.id) {
            atual->filme = filme;
            printf("Filme atualizado com sucesso!\n");
            return;
        }
        atual = atual->proximo;
    }

    printf("Filme com ID %d não encontrado para atualização.\n", filme.id);
}

// Remove um filme da tabela hash
void removerFechado(HashFechado *tabela, int id) {
    int indice = hash(id, tabela->tamanho);
    Node *atual = tabela->buckets[indice];
    Node *anterior = NULL;

    // Procurar o filme na lista encadeada
    while (atual != NULL) {
        if (atual->filme.id == id) {
            if (anterior == NULL) { // Primeiro nó
                tabela->buckets[indice] = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            printf("Filme removido com sucesso!\n");
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    printf("Filme com ID %d não encontrado para remoção.\n", id);
}

// Lista todos os filmes na tabela hash
void listarFechado(HashFechado *tabela) {
    int i;
    Node *atual;

    printf("\nLista de Filmes:\n");
    for (i = 0; i < tabela->tamanho; i++) {
        printf("Bucket %d:\n", i);
        atual = tabela->buckets[i];
        if (atual == NULL) {
            printf("  Vazio\n");
        }
        while (atual != NULL) {
            printf("  -> ID: %d, Titulo: %s, Ano: %d\n",
                   atual->filme.id, atual->filme.titulo, atual->filme.ano);
            atual = atual->proximo;
        }
    }
}

// Libera a memória da tabela hash
void liberarFechado(HashFechado *tabela) {
    int i;
    Node *atual, *temp;

    for (i = 0; i < tabela->tamanho; i++) {
        atual = tabela->buckets[i];
        while (atual != NULL) {
            temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(tabela->buckets);
}

// =========================
// PROGRAMA PRINCIPAL
// =========================
void menu() {
    printf("\nMenu:\n");
    printf("1. Inserir Filme\n");
    printf("2. Atualizar Filme\n");
    printf("3. Remover Filme\n");
    printf("4. Listar Filmes\n");
    printf("0. Sair\n");
}

int main() {
    HashFechado tabela;
    int opcao, id;
    Filme filme;

    inicializarFechado(&tabela, TAMANHO);

    do {
        menu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do '\n'

        switch (opcao) {
            case 1:
                printf("Inserir Filme (ID, Titulo, Ano): ");
                scanf("%d", &filme.id);
                getchar(); // Limpa o buffer do '\n'
                printf("Titulo: ");
                scanf(" %[^\n]s", filme.titulo);
                printf("Ano: ");
                scanf("%d", &filme.ano);
                inserirFechado(&tabela, filme);
                break;

            case 2:
                printf("Atualizar Filme (ID): ");
                scanf("%d", &id);
                getchar(); // Limpa o buffer do '\n'
                printf("Novo Titulo: ");
                scanf(" %[^\n]s", filme.titulo);
                printf("Novo Ano: ");
                scanf("%d", &filme.ano);
                filme.id = id;
                atualizarFechado(&tabela, filme);
                break;

            case 3:
                printf("Remover Filme (ID): ");
                scanf("%d", &id);
                removerFechado(&tabela, id);
                break;

            case 4:
                listarFechado(&tabela);
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    liberarFechado(&tabela);
    return 0;
}

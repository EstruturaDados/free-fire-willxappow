#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 30
#define MAX_TIPO 20
#define MAX_ITENS 10     // Capacidade máxima da mochila
// --- Structs ---
typedef struct {
    char nome[MAX_NOME]; 
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

// --- Variáveis globais ---
Item mochila[MAX_ITENS];  // Vetor que armazena os itens
int quantidadeItens = 0;  // Contador de itens cadastrados

// --- Funções do sistema - Auxiliares ---
void inserirItem();
void removerItem();
void listarItens();
void buscarItem();
void pausar();

int main() {
    int opcao;

    do {
        printf("\n====================================================\n");
        printf("=== MOCHILA DO JOGADOR - ITENS DA MOCHILA (%d/%d) ===\n", quantidadeItens, MAX_ITENS);
        printf("====================================================\n\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item da Mochila\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item\n");
        printf("0. Sair\n");
        printf("--------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Consome o '\n' deixado pelo scanf

        switch(opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarItem();
                break;
            case 0:
                printf("Saindo do JOGO...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while(opcao != 0);

    return 0;
}

// --- Função para cadastrar um item ---
void inserirItem() {
    if (quantidadeItens >= MAX_ITENS) {
        printf("Erro: Mochila cheia! Nao e possivel adicionar mais itens.\n");
        pausar();
        return;
    }

    Item novo;
    printf("\nDigite o nome do item: ");
    fgets(novo.nome, MAX_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // Remove o \n

    printf("Digite o tipo do item (Arma, Municao, Cura, Ferramenta...): ");
    fgets(novo.tipo, MAX_TIPO, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar();

    mochila[quantidadeItens] = novo;
    quantidadeItens++;

    printf("\nItem \"%s\" adicionado com sucesso!\n", novo.nome);
    listarItens();
    
}

// --- Função para remover um item pelo nome ---
void removerItem() {
    if (quantidadeItens == 0) {
        printf("\nA mochila esta vazia, nada para remover.\n");
        pausar();
        return;
    }

    char nome[MAX_NOME];
    printf("\nDigite o nome do item a ser removido: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int pos = -1;
    for (int i = 0; i < quantidadeItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("\nItem \"%s\" nao encontrado na mochila.\n", nome);
        pausar();
        return;
    }

    // Desloca os itens para preencher o espaço removido
    for (int i = pos; i < quantidadeItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    quantidadeItens--;

    printf("\nItem \"%s\" removido com sucesso.\n", nome);
    pausar();
}

// --- Função para listar os itens da mochila ---
void listarItens() {
    if (quantidadeItens == 0) {
        printf("\nA Mochila está vazia, nada para listar.\n");
        pausar();
        return;
    }

    printf("\n===============================\n");
    printf("=== ITENS NA MOCHILA (%d/%d) ===\n", quantidadeItens, MAX_ITENS);
    printf("===============================\n");
    for (int i = 0; i < quantidadeItens; i++) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
        pausar();
    }
}

// --- Função Buscar Item da Mochila ---
void buscarItem() {
    if (quantidadeItens == 0) {
        printf("\n--- Mochila vazia! Não há itens para buscar! ---\n");
        pausar();
        return;
    }

    char nome[MAX_NOME];
    printf("\n=== BUSCANDO ITEM NA MOCHILA ===\n\n");
    printf("Digite o nome do item a buscar: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0'; // remove \n

    for (int i = 0; i < quantidadeItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            printf("\n=== ITEM ENCONTRADO NA MOCHILA ===\n\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            pausar();
            return;
            
        }
    }

    printf("\n--- Item \"%s\" NÃO encontrado na MOCHILA! ---\n", nome);
    pausar();
}

void pausar(){
    printf("\nPressione Enter para continuar...");
    getchar();
}
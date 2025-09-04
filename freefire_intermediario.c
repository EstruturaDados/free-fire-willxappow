#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// --- Constantes ---
#define MAX_NOME 30
#define MAX_TIPO 20
#define MAX_ITENS 10

// --- Structs ---
// --- Struct Item
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;
// --- Struct No
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// --- Variáveis globais ---
// Vetor
Item mochilaVetor[MAX_ITENS];
int quantidadeItensVetor = 0;
int quantidadeItensLista = 0;

// Lista encadeada
No* cabecaLista = NULL;

// Contadores de comparações
int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

// --- Funções auxiliares - Prototipos ---
void limparBuffer();
void lerString();
void inserirItemVetor();
void removerItemVetor();
void listarItensVetor();
void ordenarVetor();
void buscarSequencialVetor();
void buscarBinariaVetor();
void inserirItemLista();
void removerItemLista();
void listarItensLista();
void buscarSequencialLista();
void menuVetor();
void menuLista();
void menuPrincipal ();
void liberarLista();
void pausar();

// --- Main Principal ---
int main() {
    menuPrincipal();
    liberarLista();
    return 0;
}
// --- Liberar Buffer ---
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
// --- Liberar Lista ---
void liberarLista() {
    No* atual = cabecaLista;
    while (atual != NULL) {
        No* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    cabecaLista = NULL;
}
// --- Função Pausar para usuario ver com clareza as informacoes
void pausar() {
    printf("\nPressione Enter para continuar...");
    getchar();
}

void lerString(char* buffer, int tamanho) {
    fgets(buffer, tamanho, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

// --- Funções do vetor ---
// LISTA SEQUENCIAL
// ---Inserir Item ---
void inserirItemVetor() {
    if (quantidadeItensVetor >= MAX_ITENS) {
        printf("\nMochila cheia! Remova algum item para continuar\n");
        pausar();
        return;
    }

    Item novo;
    printf("\n === ADICIONANDO ITEM NA MOCHILA (Lista Sequencial) ===\n\n");
    printf("Digite o nome do item: ");
    lerString(novo.nome, MAX_NOME);
    printf("Digite o tipo do item: ");
    lerString(novo.tipo, MAX_TIPO);
    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);
    limparBuffer();

    // Evita duplicatas
    for (int i = 0; i < quantidadeItensVetor; i++) {
        if (strcmp(mochilaVetor[i].nome, novo.nome) == 0) {
            mochilaVetor[i].quantidade += novo.quantidade;
            printf("\n--- Item existente atualizado! Quantidade total: %d\n", mochilaVetor[i].quantidade);
            pausar();
            return;
        }
    }

    mochilaVetor[quantidadeItensVetor++] = novo;
    printf("\n--- Item adicionado com sucesso!\n");
    pausar();
}
// --- Remover Item ---
void removerItemVetor() {
    if (quantidadeItensVetor == 0) {
        printf("\n--- Mochila vazia, NÃO há itens para remover!\n");
        pausar();
        return;
    }

    char nome[MAX_NOME];
    printf("\n=== REMOVENDO ITEM DA MOCHILA ===\n\n");
    printf("Digite o nome do item a remover: ");
    lerString(nome, MAX_NOME);

    int pos = -1;
    for (int i = 0; i < quantidadeItensVetor; i++) {
        if (strcmp(mochilaVetor[i].nome, nome) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("\n--- Item NÃO encontrado na MOCHILA!\n");
        pausar();
        return;
    }

    for (int i = pos; i < quantidadeItensVetor - 1; i++) {
        mochilaVetor[i] = mochilaVetor[i + 1];
    }
    quantidadeItensVetor--;
    printf("\n--- Item REMOVIDO com sucesso!\n");
    pausar();
}
// --- Listar Itens ----
void listarItensVetor() {
    if (quantidadeItensVetor == 0) {
        printf("\n--- Mochila vazia! Não há Itens para listar!\n");
        pausar();
        return;
    }

    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n\n", quantidadeItensVetor, MAX_ITENS);
    for (int i = 0; i < quantidadeItensVetor; i++) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochilaVetor[i].nome, mochilaVetor[i].tipo, mochilaVetor[i].quantidade);
    }
    pausar();
}
// --- Duplo laço para Bubble Sort ---
void ordenarVetor() {
    if (quantidadeItensVetor < 2){
        printf("\n--- Não há itens para ordenar, ou número de itens insuficiente na MOCHILA para ordenação!\n");
    return;
    }
    

    for (int i = 0; i < quantidadeItensVetor - 1; i++) {
        for (int j = 0; j < quantidadeItensVetor - i - 1; j++) {
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[j + 1].nome) > 0) {
                Item temp = mochilaVetor[j];
                mochilaVetor[j] = mochilaVetor[j + 1];
                mochilaVetor[j + 1] = temp;
            }
        }
    }
    printf("\n--- Vetor ordenado por nome!\n");
    pausar();
}
// --- Busca Sequencial no Vetor ---
void buscarSequencialVetor() {
    if (quantidadeItensVetor == 0) {
        printf("\n--- Mochila vazia! Não há itens na MOCHILA para Buscar!\n");
        pausar();
        return;
    }

    char nome[MAX_NOME];
    printf("\n=== BUSCANDO ITEM NA MOCHILA (BUSCA SEQUENCIAL) ===\n\n");
    printf("Digite o nome do item a buscar: ");
    lerString(nome, MAX_NOME);

    comparacoesSequencial = 0;
    for (int i = 0; i < quantidadeItensVetor; i++) {
        comparacoesSequencial++;
        if (strcmp(mochilaVetor[i].nome, nome) == 0) {
            printf("\n=== ITEM ENCONTRADO NA MOCHILA ===\n\n");
            printf("--- Item encontrado: Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochilaVetor[i].nome, mochilaVetor[i].tipo, mochilaVetor[i].quantidade);
            printf("\n--- Comparações realizadas: %d\n", comparacoesSequencial);
            pausar();
            return;
        }
    }
    printf("\n--- Item NÃO encontrado na MOCHILA!\n");
    printf("\n--- Comparações realizadas: %d\n", comparacoesSequencial);
    pausar();
}
// --- Busca Binario no Vetor ---
void buscarBinariaVetor() {
    if (quantidadeItensVetor == 0) {
        pausar();
        return;
    }

    char nome[MAX_NOME];
    printf("\n=== BUSCANDO ITEM NA MOCHILA (BUSCA BINARIA) ===\n\n");
    printf("Digite o nome do item a buscar: ");
    lerString(nome, MAX_NOME);

    int inicio = 0, fim = quantidadeItensVetor - 1;
    comparacoesBinaria = 0;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        comparacoesBinaria++;
        int cmp = strcmp(mochilaVetor[meio].nome, nome);
        if (cmp == 0) {
            printf("\n=== ITEM ENCONTRADO NA MOCHILA ===\n\n");
            printf("--- Item encontrado: Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochilaVetor[meio].nome, mochilaVetor[meio].tipo, mochilaVetor[meio].quantidade);
            printf("\n--- Comparações realizadas: %d\n", comparacoesBinaria);
            pausar();
            return;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    printf("\n--- Item NÃO encontrado na MOCHILA!\n");
    printf("\n--- Comparações realizadas: %d\n", comparacoesBinaria);
    pausar();
}
//LISTA ENCADEADA
// --- Funções da lista encadeada ---
void inserirItemLista() {
    Item novoItem;
    printf("\n === ADICIONANDO ITEM NA MOCHILA (%d/%d) ===\n\n",quantidadeItensLista, MAX_ITENS);
    printf("Digite o nome do item: ");
    lerString(novoItem.nome, MAX_NOME);
    printf("Digite o tipo do item: ");
    lerString(novoItem.tipo, MAX_TIPO);
    printf("Digite a quantidade: ");
    scanf("%d", &novoItem.quantidade);
    limparBuffer();

    // Verifica duplicado
    No* atual = cabecaLista;
    while (atual != NULL) {
        if (strcmp(atual->dados.nome, novoItem.nome) == 0) {
            atual->dados.quantidade += novoItem.quantidade;
            printf("\nItem existente atualizado! Quantidade total: %d\n", atual->dados.quantidade);
            pausar();
            return;
        }
        atual = atual->proximo;
    }

    // Inserção no início
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->dados = novoItem;
    novoNo->proximo = cabecaLista;
    cabecaLista = novoNo;
    quantidadeItensLista++;

    printf("\n--- Item adicionado com sucesso! ---\n");
    pausar();
}
// --- Remover Item Lista Encadeada ---
void removerItemLista() {
    if (cabecaLista == NULL) {
        printf("\n--- Mochila vazia, NÃO há itens para remover!\n");
        pausar();
        return;
    }

    char nome[MAX_NOME];
    printf("\n=== REMOVENDO ITEM DA MOCHILA ===\n\n");
    printf("Digite o nome do item a remover: ");
    lerString(nome, MAX_NOME);

    No *atual = cabecaLista, *anterior = NULL;
    while (atual != NULL && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("\nItem NÃO encontrado!\n");
        pausar();
        return;
    }

    if (anterior == NULL) { // Primeiro nó
        cabecaLista = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    quantidadeItensLista--;
    printf("\nItem removido com sucesso!\n");
    pausar();
}
// --- Listar Item ---
void listarItensLista() {
    if (cabecaLista == NULL) {
        printf("\n--- Mochila vazia! Não há Itens para listar!\n");
        pausar();
        return;
    }

    printf("\n--- ITENS DA MOCHILA (%d/%d) ---\n\n", quantidadeItensLista, MAX_ITENS);
    No* atual = cabecaLista;
    int i = 1;
    while (atual != NULL) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i++, atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
    pausar();
}
// --- Busca Sequencial ---
void buscarSequencialLista() {
    if (cabecaLista == NULL) {
        printf("\n--- Mochila vazia! Não há itens na MOCHILA para Buscar!\n");
        pausar();
        return;
    }

    char nome[MAX_NOME];
    printf("\n=== BUSCANDO ITEM NA MOCHILA (BUSCA SEQUENCIAL) ===\n\n");
    printf("Digite o nome do item a buscar: ");
    lerString(nome, MAX_NOME);

    comparacoesSequencial = 0;
    No* atual = cabecaLista;
    while (atual != NULL) {
        comparacoesSequencial++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            printf("\n=== ITEM ENCONTRADO NA MOCHILA ===\n\n");
            printf("Item encontrado: Nome: %s | Tipo: %s | Quantidade: %d\n",
                   atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
            printf("\nComparações realizadas: %d\n", comparacoesSequencial);
            pausar();
            return;
        }
        atual = atual->proximo;
    }

    printf("\n--- Item NÃO encontrado na MOCHILA!\n");
    printf("\nComparações realizadas: %d\n", comparacoesSequencial);
    pausar();
}
// --- Menus ---
// --- Menu da Lista Sequencial ---
void menuVetor() {
    int opcao;
    do {
        
        printf("\n=== Mochila com Vetor (lista Sequencial) (%d/%d) ===\n\n", quantidadeItensVetor, MAX_ITENS);
        printf("1. Inserir novo item\n");
        printf("2. Remover item por Nome\n");
        printf("3. Listar todos os itens da Mochila\n");
        printf("4. Ordenar os itens por nome\n");
        printf("5. Buscar item por nome (busca sequencial)\n");
        printf("6. Buscar binaria\n");
        printf("0. Voltar\n\n");
        printf("Escolha: ");

        scanf("%d", &opcao);
        limparBuffer();
        switch(opcao) {
            case 1: 
            inserirItemVetor(); 
            break;
            case 2: 
            removerItemVetor(); 
            break;
            case 3: 
            listarItensVetor(); 
            break;
            case 4:
            ordenarVetor(); 
            break;
            case 5: 
            buscarSequencialVetor(); 
            break;
            case 6: 
            printf("\n--- Atenção: a mochila será ordenada automaticamente antes da busca binária ---\n");
            ordenarVetor(); // ordenar automaticamente antes da busca binária
            buscarBinariaVetor(); 
            break;
            case 0: 
            break;
            default: 
            printf("\nOpcao invalida!\n");
        }
    } while(opcao != 0);
}
// --- Menu da Lista Encadeada
void menuLista() {
    int opcao;
    do {
        printf("\n=== Mochila com Lista Encadeada (%d/%d) ===\n\n", quantidadeItensLista, MAX_ITENS);
        printf("1. Inserir novo item\n");
        printf("2. Remover item por Nome\n");
        printf("3. Listar todos os itens da MOCHILA\n");
        printf("4. Buscar item por nome (busca sequencial)\n");
        printf("0. Voltar\n");
        printf("\nEscolha: ");

        scanf("%d", &opcao);
        limparBuffer();
        switch(opcao) {
            case 1: inserirItemLista(); 
            break;
            case 2: removerItemLista(); 
            break;
            case 3: listarItensLista(); 
            break;
            case 4: buscarSequencialLista(); 
            break;
            case 0: 
            break;
            default: 
            printf("\nOpcao invalida!\n");
        }
    } while(opcao != 0);
}
// --- Menu Principal ---
void menuPrincipal() {
    int opcao;
    do {
        printf("\n=== SISTEMA DE MOCHILA - INTERMEDIARIO ===\n\n");
        printf("1. Usar Mochila com vetor (Lista Sequencial)\n");
        printf("2. Usar Mochila com (Lista Encadeada)\n");
        printf("0. Sair\n");
        printf("\nEscolha: ");
        scanf("%d", &opcao);
        limparBuffer();
        switch(opcao) {
            case 1: menuVetor(); break;
            case 2: menuLista(); break;
            case 0: printf("\nSaindo...\n"); break;
            default: printf("\nOpcao invalida!\n");
        }
    } while(opcao != 0);
}

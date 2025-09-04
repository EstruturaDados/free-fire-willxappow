#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constantes
#define TAM_NOME 30
#define TAM_TIPO 20
#define MAX_ITENS 20

// -----------------------------
// Estruturas de dados
// -----------------------------

typedef struct {
    char nome[TAM_NOME];     // Ex.: "chip central"
    char tipo[TAM_TIPO];
    int quantidade;
    int prioridade;          // 1 (alta) ... 10(baixa)
} Componente;

// -----------------------------
// Enum para controle do último critério de ordenação
// -----------------------------

typedef enum { NAO_ORDENADO = 0, ORDENADO_POR_NOME, ORDENADO_POR_TIPO, ORDENADO_POR_PRIORIDADE } UltimaOrdenacao;

// -----------------------------
// Utilitários (assinaturas)
// -----------------------------

void limparNovaLinha(char *s);
void limparBuffer();
void pausar();
void lerString(const char *rotulo, char *dest, size_t tam);
int lerInteiroIntervalo(const char *rotulo, int min, int max);
void mostrarComponentes(Componente v[], int n);

// -----------------------------
// Medição de tempo genérica
// -----------------------------

typedef void (*SortFn)(Componente[], int, long*);
double medirTempo(SortFn algoritmo, Componente v[], int n, long *comparacoes);

// -----------------------------
// Algoritmos de ordenação (cada um conta comparações)
// -----------------------------

// Bubble Sort por NOME (string)
void bubbleSortNome(Componente v[], int n, long *comparacoes) {
    if (comparacoes) *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - i - 1; j++) {
            // comparação de strings (contabilizada)
            if (comparacoes) (*comparacoes)++;
            if (strcmp(v[j].nome, v[j+1].nome) > 0) {
                Componente tmp = v[j];
                v[j] = v[j+1];
                v[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break; // já está ordenado
    }
}

// Insertion Sort por TIPO (string)
void insertionSortTipo(Componente v[], int n, long *comparacoes) {
    if (comparacoes) *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;
        // Conta apenas as comparações de strings (strcmp)
        while (j >= 0) {
            if (comparacoes) (*comparacoes)++;
            if (strcmp(v[j].tipo, chave.tipo) > 0) {
                v[j+1] = v[j];
                j--;
            } else {
                break;
            }
        }
        v[j+1] = chave;
    }
}

// Selection Sort por PRIORIDADE (int)
void selectionSortPrioridade(Componente v[], int n, long *comparacoes) {
    if (comparacoes) *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (comparacoes) (*comparacoes)++;
            if (v[j].prioridade < v[minIdx].prioridade) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Componente tmp = v[i];
            v[i] = v[minIdx];
            v[minIdx] = tmp;
        }
    }
}

// -----------------------------
// Busca Binária por NOME (apenas após ordenação por nome)
// -----------------------------

int buscaBinariaPorNome(Componente v[], int n, const char chave[], long *comparacoes) {
    int ini = 0, fim = n - 1;
    if (comparacoes) *comparacoes = 0;
    while (ini <= fim) {
        int meio = ini + (fim - ini) / 2;
        int cmp = strcmp(v[meio].nome, chave);
        if (comparacoes) (*comparacoes)++;
        if (cmp == 0) return meio;
        else if (cmp < 0) ini = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}
// --- Achar Indice por Nome
int acharIndicePorNome(const Componente v[], int n, const char *nome) {
    for (int i = 0; i < n; i++) {
    if (strcmp(v[i].nome, nome) == 0) return i;
    }
    return -1;
}

// -----------------------------
// Remover Componente
// -----------------------------
void removerComponente(Componente v[], int *n) {
    if (*n == 0) {
        printf("\nNão há componentes para remover.\n");
        return;
    }
    printf("\n--- Remover Componente ---\n\n");
    printf("1. Remover por Indice\n");
    printf("2. Remover por Nome\n");
    int modo = lerInteiroIntervalo("Escolha: ", 1, 2);


    int idx = -1;  
    if (modo == 1) {
        mostrarComponentes(v, *n);
        idx = lerInteiroIntervalo("\nÍndice (0..N-1): ", 0, *n-1);
    } else {
        char chave[TAM_NOME];
        lerString("\nNome exato do Componente a Remover: ", chave, sizeof(chave));
        idx = acharIndicePorNome(v, *n, chave);
        if (idx == -1) {
            printf("\n--- Item não encontrado. ---\n");
            pausar();
            return;
        }
}
// Desloca à esquerda
// Guardar nome antes de remover
    char removido[TAM_NOME];
    strcpy(removido, v[idx].nome);

    for (int i = idx; i < *n - 1; i++) v[i] = v[i+1];
    (*n)--;
    printf("\nComponente \"%s\" removido com sucesso!\n", removido);
    pausar();
}

// -----------------------------
// Funções de I/O e utilitários
// -----------------------------

void limparNovaLinha(char *s) {
    if (!s) return;
    size_t len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}
// --- Liberar Buffer ---
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
// --- Função Ler Strings ---
void lerString(const char *rotulo, char *dest, size_t tam) {
    printf("%s", rotulo);
    if (fgets(dest, (int)tam, stdin) == NULL) {
        // fallback
        dest[0] = '\0';
        return;
    }
    limparNovaLinha(dest);
}
// --- Função Pausar para usuario ver com clareza as informacoes
void pausar() {
    printf("\nPressione Enter para continuar...");
    getchar();
}
// --- Função para ler valores entre intervalos Inteiros
int lerInteiroIntervalo(const char *rotulo, int min, int max) {
    char buf[64];
    long val;
    char *end;
    while (1) {
        printf("%s", rotulo);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        val = strtol(buf, &end, 10);
        if (end == buf) {
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }
        if (val < min || val > max) {
            printf("Valor deve estar entre %d e %d.\n", min, max);
            continue;
        }
        return (int)val;
    }
}
// --- Função Mostrar os Componentes Cadastrados
void mostrarComponentes(Componente v[], int n) {
    printf("\n--- INVENTARIO ATUAL (%d/%d) ---\n", n, MAX_ITENS);
    printf("----------------------------------------------------------------------------------------\n");
    printf("%-6s | %-30s | %-20s | %10s | %10s\n", "Indice","Nome", "Tipo", "Quantidade", "Prioridade");
    printf("%.*s\n", 6 + 3 + 30 + 3 + 20 + 3 + 10 + 3 + 10, "-----------------------------------------------------------------------------------------");
    for (int i = 0; i < n; i++) {
    printf("%-6d | %-30s | %-20s | %10d | %10d\n",i ,v[i].nome, v[i].tipo, v[i].quantidade, v[i].prioridade);
}
    printf("----------------------------------------------------------------------------------------\n");
    
}
// Convertendo enum em texto
const char* statusOrdenacaoTexto(UltimaOrdenacao u) {
    switch (u) {
        case NAO_ORDENADO: return "NÃO ORDENADO";
        case ORDENADO_POR_NOME: return "ORDENADO POR NOME";
        case ORDENADO_POR_TIPO: return "ORDENADO POR TIPO";
        case ORDENADO_POR_PRIORIDADE: return "ORDENADO POR PRIORIDADE";
        default: return "DESCONHECIDO";
    }
}

// -----------------------------
// Medição de tempo
// -----------------------------

double medirTempo(SortFn algoritmo, Componente v[], int n, long *comparacoes) {
    clock_t inicio = clock();
    algoritmo(v, n, comparacoes);
    clock_t fim = clock();
    return (double)(fim - inicio) / CLOCKS_PER_SEC;
}

// -----------------------------
// Cadastro de componentes
// -----------------------------

void cadastrarComponente(Componente v[], int *n) {
    if (*n >= MAX_ITENS) {
        printf("\nLimite máximo de %d componentes atingido.\n", MAX_ITENS);
        return;
    }
    Componente c;
    printf("\n--- Coletando Novo Componente ---\n");
    lerString("Nome do componente: ", c.nome, sizeof(c.nome));
    lerString("Tipo do componente(Estrutural, Eletronico, Energia): ", c.tipo, sizeof(c.tipo));
    c.quantidade = lerInteiroIntervalo("Quantidade: ", 1, 10000);
    c.prioridade = lerInteiroIntervalo("Prioridade de Montagem(1-10): ", 1, 10);
    v[*n] = c;
    (*n)++;
    printf("\nComponente '%s' cadastrado com sucesso!\n", c.nome);
    mostrarComponentes(v, *n);
     
}

// -----------------------------
// Menu interativo
// -----------------------------

void exibirMenu(int n, UltimaOrdenacao ultima) {
    printf("\n=======================================================\n");
    printf("==== PLANO DE FUGA — CÓDIGO DA ILHA (NIVEL MESTRE) ==== \n");
    printf("=======================================================\n");
    printf("Itens na Mochila: %d/%d\n",n , MAX_ITENS);
    printf("Status da Ordenação por Nome: %s\n", statusOrdenacaoTexto(ultima));
    printf("\n1. Adicionar componente\n");
    printf("2. Remover componente\n");
    printf("3. Listar Componentes (Inventário)\n");
    printf("4. Organizar MOCHILA (Ordenar Componentes)\n");
    printf("5. Busca Binaria por Componente-Chave (por NOME)\n");
    printf("0. ATIVAR TORRE de FUGA (Sair)\n\n");
    printf("Escolha uma opção: ");
       
}

void menuOrdenacao() {
    printf("\n---Estrategia de Organização ---\n\n");
    printf("1. Por Nome [Ordem Alfabética]\n");
    printf("2. Por TIPO\n");
    printf("3. Por Prioridade de Montagem\n");
    printf("0. Cancelar\n");
}

int main(void) {
    Componente componentes[MAX_ITENS];
    int n = 0;
    UltimaOrdenacao ultima = NAO_ORDENADO;

    while (1) {
        exibirMenu(n, ultima);
        char opcao[8];
        if (!fgets(opcao, sizeof(opcao), stdin)) break;
        int escolha = atoi(opcao);

        if (escolha == 0) {
            printf("\nEncerrando... Até Logo!\n\n");
            break;
        }

        switch (escolha) {
            case 1: // cadastrar
                cadastrarComponente(componentes, &n);
                pausar();
                break;
            
            case 2: // Remocao
                if (n == 0){ 
                printf("\n --- Nenhum componente cadastrado para Remover ---\n");
                pausar();
                }
                else removerComponente(componentes, &n);
                break;

            case 3: // listar
                if (n == 0){ 
                printf("\n --- Nenhum componente cadastrado para listar ---\n");
                
                }
                else mostrarComponentes(componentes, n);    
                pausar();     
                break;                

            case 4: //Ordenacao
               if (n < 2) {
                    printf("\n--- Para visualizar a ordenação, cadastre pelo menos 2 itens ---\n");
                    pausar();
                    break;
                }
                menuOrdenacao();
                int escolhaOrd = lerInteiroIntervalo("\nEscolha o Critério: ", 0, 3);
                if (escolhaOrd == 0) break;

                long comps = 0;
                double t = 0.0;
                switch (escolhaOrd) {
                    case 1:
                        t = medirTempo(bubbleSortNome, componentes, n, &comps);
                        ultima = ORDENADO_POR_NOME;
                        break;
                    case 2:
                        t = medirTempo(insertionSortTipo, componentes, n, &comps);
                        ultima = ORDENADO_POR_TIPO;
                        break;
                    case 3:
                        t = medirTempo(selectionSortPrioridade, componentes, n, &comps);
                        ultima = ORDENADO_POR_PRIORIDADE;
                        break;
                }
                printf("\nOrdenação concluída. Comparações: %ld | Tempo: %.6f s\n", comps, t);
                mostrarComponentes(componentes, n);
                pausar();
                break;      
                                        
            case 5: {// busca binária por nome
                if (n == 0) { 
                    printf("\nNão há componentes para buscar.\n");
                    pausar();
                    break; 
                }
                if (ultima != ORDENADO_POR_NOME) {
                    printf("\nALERTA! A busca binária requer que a mochila esteja ORDENADA por NOME.\n" 
                           "Use a opção 4 para organizar a mochila primeiro.\n");
                           pausar();
                    break;
                }
                char chave[TAM_NOME];
                printf("\n--- Busca Binaria - [POR NOME] ---\n\n");
                lerString("Nome do componente-chave: ", chave, sizeof(chave));
                long compsBusca = 0;
                int idx = buscaBinariaPorNome(componentes, n, chave, &compsBusca);
                if (idx >= 0) {
                    printf("\n --- RESULTADO DA BUSCA BINARIA ---\n\n");
                    printf("[OK] Componente-chave ENCONTRADO!\n\n");
                    printf("Posição [INDICE]: %d\n", idx);
                    printf("Detalhes: Nome = '%s' | Tipo = '%s' | Prioridade = %d\n", 
                           componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
                           
                } else {
                    printf("\n[ALERTA] Componente-chave NÃO encontrado.\n");
                    
                }
                printf("Comparações (busca binária): %ld\n", compsBusca);
                pausar();
                break;
            }
            
            default:
                printf("Opção inválida. Tente novamente.\n\n");
        }
    }

    return 0;
}

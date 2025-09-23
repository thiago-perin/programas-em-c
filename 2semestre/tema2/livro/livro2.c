// ######################################################
// SISTEMA DE BIBLIOTECA - mod 1
// ######################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // biblioteca necessária para funções de string (strcpy, strcmp)

// Define uma constante para o comprimento máximo que cada texto pode ter.
#define MAX_STR_LEN 50 // Tamanho máximo de cada string

// ######################################################
// PARTE 1: LISTA ESTÁTICA (VETOR DE STRING)
// ######################################################

#define TAM_MAX 10 // Define o número máximo de itens na lista estática

typedef struct {
    char dados[TAM_MAX][MAX_STR_LEN]; // Matriz 10 linhas (itens), 50 colunas (caracteres por item)
    int quantidade; // Contador de quantos itens estão realmente armazenados.
} ListaEstatica;


// Protótipos das Funções da Lista Estática
void iniciarListaEstatica(ListaEstatica *lista);
void inserirListaEstatica(ListaEstatica *lista, const char* texto);
void removerListaEstatica(ListaEstatica *lista, const char* texto);
void listarListaEstatica(const ListaEstatica *lista);

// ######################################################
// Parte 2: LISTA ENCADEADA (DE STRINGS)
// ######################################################

typedef struct No {
    char* dado; // Ponteiro para string (alocada dinamicamente)
    struct No *proximo;
} No;

typedef No* ListaEncadeada;

// Protótipos das Funções da Lista Encadeada
void inicializarListaEncadeada(ListaEncadeada *lista);
void inserirListaEncadeada(ListaEncadeada *lista, const char* texto);
void removerListaEncadeada(ListaEncadeada *lista, const char* texto);
void listarListaEncadeada(const ListaEncadeada lista);
void liberarListaEncadeada(ListaEncadeada *lista);

// ######################################################
// FUNÇÃO PRINCIPAL (main) e MENUS
// ######################################################

// Protótipos dos Menus
void exibirMenuPrincipal();
void menuListaEstatica();
void menuListaEncadeada();
void limparBufferEntrada();


int main () {
    int opcao;

    do {
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        limparBufferEntrada(); // Limpa o buffer depois de ler a opção
        
        switch(opcao) {
            case 1:
                menuListaEstatica();
                break;
            
            case 2:
                menuListaEncadeada(); // Chamada para o menu da lista encadeada agora está ATIVA
                break;

            case 0: // SAIR
                printf("\nSaindo do Sistema...\n");
                break;
            
            default: // OPÇÃO INVÁLIDA
                printf("\nOpcao Invalida! Tente novamente.\n");
        }

        if (opcao != 0) {
             printf("\nPressione Enter para voltar ao menu principal...");
             getchar(); // Pausa antes de mostrar o menu principal novamente
        }

    } while (opcao != 0);

    return 0;
}

// ############################################################################################
// - - - Implementação das funções de Menu - - -
// ############################################################################################

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirMenuPrincipal() {
    printf("\n\n--- MANIPULACAO DE LISTAS (TEXTO) ---\n");
    printf("1. Lista Estatica\n");
    printf("2. Lista Encadeada\n");
    printf("0. Sair do Programa\n");
    printf("---------------------------------------\n");
    printf("Escolha uma opcao: ");
}

void menuListaEstatica() {
    ListaEstatica minhaLista;
    iniciarListaEstatica(&minhaLista);
    
    int opcao;
    char texto[MAX_STR_LEN];

    do {
        printf("\n--- MENU LISTA ESTATICA ---\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("0. Voltar ao menu principal\n");
        printf("---------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                printf("Digite o texto a inserir: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = '\0';
                inserirListaEstatica(&minhaLista, texto);
                break;

            case 2:
                printf("Digite o texto a remover: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = '\0';
                removerListaEstatica(&minhaLista, texto);
                break;
            
            case 3:
                listarListaEstatica(&minhaLista);
                break;

            case 0:
                printf("Voltando ao menu principal...\n");
                break;
        
            default:
                printf("\nOpcao Invalida! Tente novamente.\n");
                break;
        }

        if (opcao != 0) {
             printf("\nPressione Enter para continuar...");
             getchar();
        }

    } while (opcao != 0);
}

void menuListaEncadeada() {
    ListaEncadeada minhaLista; // Declara a lista (que é um ponteiro para No)
    inicializarListaEncadeada(&minhaLista); // Inicializa a lista como vazia (NULL)

    int opcao;
    char texto[MAX_STR_LEN];

    do {
        printf("\n--- MENU LISTA ENCADEADA ---\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("0. Voltar ao menu principal\n");
        printf("----------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                printf("Digite o texto a inserir: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = '\0';
                inserirListaEncadeada(&minhaLista, texto);
                break;
            
            case 2:
                printf("Digite o texto a remover: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = '\0';
                removerListaEncadeada(&minhaLista, texto);
                break;
            
            case 3:
                listarListaEncadeada(minhaLista);
                break;

            case 0:
                // ESSENCIAL: Liberar toda a memória antes de sair do menu
                liberarListaEncadeada(&minhaLista); 
                printf("Memoria da lista encadeada liberada. Voltando...\n");
                break;

            default:
                printf("\nOpcao Invalida! Tente novamente.\n");
                break;
        }
        
        if (opcao != 0) {
             printf("\nPressione Enter para continuar...");
             getchar();
        }

    } while (opcao != 0);
}


// ############################################################################################
// - - - Implementação das funções: LISTA ESTÁTICA - - -
// ############################################################################################

void iniciarListaEstatica(ListaEstatica *lista) {
    lista->quantidade = 0;
}

void inserirListaEstatica(ListaEstatica *lista, const char* texto) {
    if (lista->quantidade == TAM_MAX) {
        printf("Erro: Lista cheia! Nao e possivel inserir.\n");
        return;
    }
    strcpy(lista->dados[lista->quantidade], texto);
    lista->quantidade++;
    printf("Texto \"%s\" inserido com sucesso.\n", texto);
}

void removerListaEstatica(ListaEstatica *lista, const char* texto) {
    int i, pos = -1;
    
    for (i = 0; i < lista->quantidade; i++) {
        if (strcmp(lista->dados[i], texto) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Erro: Texto \"%s\" nao encontrado na lista.\n", texto);
        return;
    }

    for (i = pos; i < lista->quantidade - 1; i++) {
        strcpy(lista->dados[i], lista->dados[i+1]);
    }

    lista->quantidade--;
    printf("Texto \"%s\" removido com sucesso.\n", texto);
}

void listarListaEstatica(const ListaEstatica *lista) {
    if (lista->quantidade == 0) {
        printf("A lista estatica esta vazia.\n");
        return;
    }
    printf("\nItens da Lista Estatica: [ ");
    for (int i = 0; i < lista->quantidade; i++) {
        printf("\"%s\" ", lista->dados[i]);
    }
    printf("]\n");
}


// ############################################################################################
// - - - Implementação das funções: LISTA ENCADEADA - - -
// ############################################################################################

void inicializarListaEncadeada(ListaEncadeada *lista) {
    *lista = NULL;
}

void inserirListaEncadeada(ListaEncadeada *lista, const char* texto) {
    No* novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocacao de memoria para o no.\n");
        return;
    }

    novoNo->dado = (char*) malloc(strlen(texto) + 1);
    if(novoNo->dado == NULL) {
        printf("Erro: Falha na alocacao de memoria para o texto.\n");
        free(novoNo);
        return;
    }

    strcpy(novoNo->dado, texto);
    novoNo->proximo = *lista;
    *lista = novoNo;
    printf("Texto \"%s\" inserido com sucesso.\n", texto);
}

void removerListaEncadeada(ListaEncadeada *lista, const char* texto) {
    No *atual = *lista;
    No *anterior = NULL;

    while (atual != NULL && strcmp(atual->dado, texto) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if(atual == NULL) {
        printf("Erro: Texto \"%s\" nao encontrado na lista.\n", texto);
        return;
    }

    if (anterior == NULL) {
        *lista = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual->dado);
    free(atual);
    printf("Texto \"%s\" removido com sucesso.\n", texto);
}

void listarListaEncadeada(const ListaEncadeada lista) {
    No *temp = lista;
    if (temp == NULL) {
        printf("A lista encadeada esta vazia.\n");
        return;
    }
    printf("\nItens da Lista Encadeada: [ ");
    while (temp != NULL) {
        printf("\"%s\" ", temp->dado);
        temp = temp->proximo;
    }
    printf("]\n");
}

void liberarListaEncadeada(ListaEncadeada *lista) {
    No *atual = *lista;
    No *proximo;

    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual->dado);
        free(atual);
        atual = proximo;
    }
    *lista = NULL;
}

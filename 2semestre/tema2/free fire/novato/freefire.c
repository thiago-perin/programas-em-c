// ######################################################
// FREE FIRE - mod 3
// ######################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // biblioteca necessária para funções de string (strcpy, strcmp)

// Define uma constante para o comprimento máximo que cada texto pode ter.
#define MAX_STR_LEN 50 // Tamanho máximo de cada string
#define TAM_MAX 10     // Define o número máximo de itens em qualquer mochila

// ######################################################
// ESTRUTURA DE DADOS UNIFICADA PARA OS ITENS
// ######################################################
typedef struct {
    char nome[MAX_STR_LEN];
    char tipo[MAX_STR_LEN];
    int quantidade; // << ADICIONADO
} Item;


// ######################################################
// PARTE 1: MOCHILA DE OUTROS ITENS (LISTA ESTÁTICA)
// ######################################################

typedef struct {
    Item dados[TAM_MAX]; // Matriz de Itens
    int quantidade;      // Contador de quantos itens estão armazenados.
} MochilaEstatica;

// Protótipos das Funções da Lista Estática
void iniciarMochilaEstatica(MochilaEstatica *mochila);
void inserirItemEstatico(MochilaEstatica *mochila, const char* nome, const char* tipo, int quantidade);
void removerItemEstatico(MochilaEstatica *mochila, const char* nome);
void listarItensEstaticos(const MochilaEstatica *mochila);


// ######################################################
// Parte 2: MOCHILA DE ARMAS E ROUPAS (LISTA ENCADEADA)
// ######################################################

typedef struct No {
    Item dado; // Armazena o item completo
    struct No *proximo;
} No;

typedef struct {
    No* cabeca;   // Ponteiro para o primeiro item da lista
    int quantidade; // Contador de itens na lista
} MochilaEncadeada;


// Protótipos das Funções da Lista Encadeada
void inicializarMochilaEncadeada(MochilaEncadeada *mochila);
void inserirItemEncadeado(MochilaEncadeada *mochila, const char* nome, const char* tipo, int quantidade);
void removerItemEncadeado(MochilaEncadeada *mochila, const char* nome);
void listarItensEncadeados(const MochilaEncadeada *mochila);
void liberarMochilaEncadeada(MochilaEncadeada *mochila);

// ######################################################
// FUNÇÃO PRINCIPAL (main) e MENUS
// ######################################################

// Protótipos dos Menus
void exibirMenuPrincipal();
void menuMochilaOutrosItens();
void menuMochilaArmasRoupas();
void limparBufferEntrada();


int main () {
    int opcao;

    do {
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        limparBufferEntrada(); 
        
        switch(opcao) {
            case 1:
                menuMochilaArmasRoupas();
                break;
            
            case 2:
                menuMochilaOutrosItens();
                break;

            case 0:
                printf("\nSaindo do Jogo...\n");
                break;
            
            default:
                printf("\nOpcao Invalida! Tente novamente.\n");
        }

        if (opcao != 0 && (opcao == 1 || opcao == 2)) {
             printf("\nPressione Enter para voltar ao menu principal...");
             getchar();
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
    printf("\n\n--- MENU DE MOCHILAS ---\n");
    printf("1. Mochila de Armas e Roupas (Encadeada)\n");
    printf("2. Mochila de Outros Itens (Estatica)\n");
    printf("0. Sair do Jogo\n");
    printf("--------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

void menuMochilaOutrosItens() { 
    MochilaEstatica minhaMochila;
    iniciarMochilaEstatica(&minhaMochila);
    
    int opcao;
    char nome[MAX_STR_LEN];
    char tipo[MAX_STR_LEN];
    int quantidade;

    do {
        printf("\n--- MOCHILA DE OUTROS ITENS ---\n");
        printf("Itens na mochila: %d/%d\n", minhaMochila.quantidade, TAM_MAX);
        printf("---------------------------\n");
        printf("1. Adicionar um item\n");
        printf("2. Remover um item\n");
        printf("3. Listar os itens\n");
        printf("0. Voltar ao menu principal\n");
        printf("---------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                printf("\n\n - - - Adicionar novo item - - - \n");

                printf("Digite o NOME do item a inserir: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                
                printf("Digite o TIPO do item (ex: Cura, Pocao, Chave): ");
                fgets(tipo, MAX_STR_LEN, stdin);
                tipo[strcspn(tipo, "\n")] = '\0';

                printf("Digite a QUANTIDADE: ");
                scanf("%d", &quantidade);
                limparBufferEntrada();

                inserirItemEstatico(&minhaMochila, nome, tipo, quantidade);

                break;

            case 2:
                printf("Digite o NOME do item a remover: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                removerItemEstatico(&minhaMochila, nome);

                break;
            
            case 3:
                listarItensEstaticos(&minhaMochila);
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


void menuMochilaArmasRoupas() { 
    MochilaEncadeada minhaMochila; 
    inicializarMochilaEncadeada(&minhaMochila); 

    int opcao;
    char nome[MAX_STR_LEN];
    char tipo[MAX_STR_LEN];
    int quantidade;

    do {
        printf("\n--- MOCHILA DE ARMAS E ROUPAS ---\n");
        printf("Itens na mochila: %d/%d\n", minhaMochila.quantidade, TAM_MAX);
        printf("----------------------------------\n");
        printf("1. Adicionar um item\n");
        printf("2. Remover um item\n");
        printf("3. Listar os itens\n");
        printf("0. Voltar ao menu principal\n");
        printf("----------------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                printf("\n\n - - - Adicionar novo item - - - \n");

                printf("Nome do item: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("Tipo do item (Arma, municao, protecao, etc.): ");
                fgets(tipo, MAX_STR_LEN, stdin);
                tipo[strcspn(tipo, "\n")] = '\0';

                printf("Quantidade: ");
                scanf("%d", &quantidade);
                limparBufferEntrada();

                inserirItemEncadeado(&minhaMochila, nome, tipo, quantidade);
                break;
            
            case 2:
                printf("Nome do item a remover: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                removerItemEncadeado(&minhaMochila, nome);

                break;
            
            case 3:
                listarItensEncadeados(&minhaMochila);
                break;

            case 0:
                liberarMochilaEncadeada(&minhaMochila); 
                printf("Memoria da mochila de armas/roupas liberada. Voltando...\n");
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
// - - - Implementação das funções: MOCHILA ESTÁTICA (OUTROS ITENS) - - -
// ############################################################################################

void iniciarMochilaEstatica(MochilaEstatica *mochila) {
    mochila->quantidade = 0;
}

void inserirItemEstatico(MochilaEstatica *mochila, const char* nome, const char* tipo, int quantidade) {
    if (mochila->quantidade == TAM_MAX) {
        printf("Erro: Mochila de outros itens esta cheia!\n");
        return;
    }
    strcpy(mochila->dados[mochila->quantidade].nome, nome);
    strcpy(mochila->dados[mochila->quantidade].tipo, tipo);
    mochila->dados[mochila->quantidade].quantidade = quantidade;
    mochila->quantidade++;
    printf("\nItem \"%s\" inserido com sucesso.\n", nome);
}

void removerItemEstatico(MochilaEstatica *mochila, const char* nome) {
    int i, pos = -1;
    
    for (i = 0; i < mochila->quantidade; i++) {
        if (strcmp(mochila->dados[i].nome, nome) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Erro: Item \"%s\" nao encontrado na mochila.\n", nome);
        return;
    }

    for (i = pos; i < mochila->quantidade - 1; i++) {
        mochila->dados[i] = mochila->dados[i + 1];
    }

    mochila->quantidade--;
    printf("\nItem \"%s\" removido com sucesso.\n", nome);
}

void listarItensEstaticos(const MochilaEstatica *mochila) {
    // 1. O cabeçalho da tabela, mostrando a quantidade de itens
    printf("\n--- ITENS NA MOCHILA DE OUTROS ITENS (%d/%d) ---\n", mochila->quantidade, TAM_MAX);
    printf("----------------------------------------------------\n");
    // 2. O cabeçalho das colunas. Use %-Ns para alinhar à esquerda.
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("----------------------------------------------------\n");

    // 3. Verifica se a mochila está vazia
    if (mochila->quantidade == 0) {
        printf("A mochila esta vazia.\n");
    } else {
        // 4. Loop para imprimir cada item formatado na tabela
        for (int i = 0; i < mochila->quantidade; i++) {
            printf("%-20s | %-15s | %d\n", 
                   mochila->dados[i].nome, 
                   mochila->dados[i].tipo, 
                   mochila->dados[i].quantidade);
        }
    }
    
    // 5. Linha final para fechar a tabela
    printf("----------------------------------------------------\n");
}


// ############################################################################################
// - - - Implementação das funções: MOCHILA ENCADEADA (ARMAS E ROUPAS) - - -
// ############################################################################################

void inicializarMochilaEncadeada(MochilaEncadeada *mochila) {
    mochila->cabeca = NULL;
    mochila->quantidade = 0;
}

void inserirItemEncadeado(MochilaEncadeada *mochila, const char* nome, const char* tipo, int quantidade) {
    if (mochila->quantidade == TAM_MAX) {
        printf("Erro: Mochila de armas e roupas cheia!\n");
        return;
    }

    No* novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocacao de memoria.\n");
        return;
    }

    strcpy(novoNo->dado.nome, nome);
    strcpy(novoNo->dado.tipo, tipo);
    novoNo->dado.quantidade = quantidade;

    novoNo->proximo = mochila->cabeca;
    mochila->cabeca = novoNo;
    mochila->quantidade++;

    printf("\nItem \"%s\" inserido com sucesso.\n", nome);
}

void removerItemEncadeado(MochilaEncadeada *mochila, const char* nome) {
    No *atual = mochila->cabeca;
    No *anterior = NULL;

    while (atual != NULL && strcmp(atual->dado.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if(atual == NULL) {
        printf("Erro: Item \"%s\" nao encontrado na mochila.\n", nome);
        return;
    }

    if (anterior == NULL) {
        mochila->cabeca = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    mochila->quantidade--;
    printf("\nItem \"%s\" removido com sucesso.\n", nome);
}

void listarItensEncadeados(const MochilaEncadeada *mochila) {
    No *temp = mochila->cabeca;
    
    // 1. O cabeçalho da tabela
    printf("\n--- ITENS NA MOCHILA DE ARMAS E ROUPAS (%d/%d) ---\n", mochila->quantidade, TAM_MAX);
    printf("----------------------------------------------------\n");
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("----------------------------------------------------\n");

    // 2. Verifica se a mochila está vazia
    if (temp == NULL) {
        printf("A mochila esta vazia.\n");
    } else {
        // 3. Loop para imprimir cada item da lista encadeada
        while (temp != NULL) {
            printf("%-20s | %-15s | %d\n", 
                   temp->dado.nome, 
                   temp->dado.tipo, 
                   temp->dado.quantidade);
            temp = temp->proximo;
        }
    }

    // 4. Linha final da tabela
    printf("----------------------------------------------------\n");
}

void liberarMochilaEncadeada(MochilaEncadeada *mochila) {
    No *atual = mochila->cabeca;
    No *proximo;

    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    mochila->cabeca = NULL;
    mochila->quantidade = 0;
}

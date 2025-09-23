// ######################################################
// FREE FIRE - mod 5 (Recurso de Cancelar Ação)
// ######################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // biblioteca necessária para funções de string (strcpy, strcmp)
#include <time.h>   // Para a função de tempo no save

// ######################################################
// DEFINIÇÕES GLOBAIS
// ######################################################
#define MAX_STR_LEN 50 // Tamanho máximo de cada string
#define TAM_MAX 10     // Define o número máximo de itens em qualquer mochila
#define MAX_SAVES 5    // Define o número máximo de slots para salvar

// ######################################################
// ESTRUTURAS DE DADOS
// ######################################################

// Estrutura base para um item
typedef struct {
    char nome[MAX_STR_LEN];
    char tipo[MAX_STR_LEN];
    int quantidade;
} Item;

// Estrutura da Mochila Estática
typedef struct {
    Item dados[TAM_MAX];
    int quantidade;
} MochilaEstatica;

// Estruturas para a Mochila Encadeada (Nó e a própria Mochila)
typedef struct No {
    Item dado;
    struct No *proximo;
} No;

typedef struct {
    No* cabeca;
    int quantidade;
} MochilaEncadeada;

// Estrutura para salvar o estado do jogo em um arquivo
typedef struct {
    int slotOcupado;
    char nomeSave[50];
    char dataHora[30];
    MochilaEstatica mochilaEstatica;
    Item mochilaEncadeadaItens[TAM_MAX];
    int mochilaEncadeadaQuantidade;
} SaveState;


// ######################################################
// VARIÁVEIS GLOBAIS
// ######################################################
MochilaEstatica mochila_outros;  // Mochila global para outros itens
MochilaEncadeada mochila_armas; // Mochila global para armas e roupas


// ######################################################
// PROTÓTIPOS DAS FUNÇÕES
// ######################################################

// --- Funções de Menu ---
void exibirMenuPrincipal();
void exibirMenuSecundario();
void entrarNoJogo();
void gerenciarCarregamento(int *jogoIniciadoFlag);
void gerenciarSalvamento(int jogoIniciadoFlag);
void gerenciarReset();
void limparBufferEntrada();
void menuMochilaArmasRoupas();
void menuMochilaOutrosItens();

// --- Funções da Mochila Estática ---
void iniciarMochilaEstatica(MochilaEstatica *mochila);
void inserirItemEstatico(MochilaEstatica *mochila, const char* nome, const char* tipo, int quantidade);
void removerItemEstatico(MochilaEstatica *mochila, const char* nome);
void listarItensEstaticos(const MochilaEstatica *mochila);
void buscarItemEstatico(const MochilaEstatica *mochila, const char* nome);

// --- Funções da Mochila Encadeada ---
void inicializarMochilaEncadeada(MochilaEncadeada *mochila);
void inserirItemEncadeado(MochilaEncadeada *mochila, const char* nome, const char* tipo, int quantidade);
void removerItemEncadeado(MochilaEncadeada *mochila, const char* nome);
void listarItensEncadeados(const MochilaEncadeada *mochila);
void liberarMochilaEncadeada(MochilaEncadeada *mochila);
void buscarItemEncadeado(const MochilaEncadeada *mochila, const char* nome);

// --- Funções de Arquivo (Save/Load/Reset) ---
void getNomeArquivo(int slot, char* nomeArquivo);
void mostrarSlots();
void salvarJogo(int slot);
int carregarJogo(int slot);
void resetarSlot(int slot);


// ######################################################
// FUNÇÃO PRINCIPAL (main)
// ######################################################
int main () {
    int opcao;
    int jogoIniciado = 0; // Flag: 0 = Não, 1 = Sim. Impede de entrar ou salvar um jogo vazio.

    do {
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        limparBufferEntrada(); 
        
        switch(opcao) {
            case 1: // Novo Jogo
                printf("\nIniciando um novo jogo! As mochilas foram esvaziadas.\n");
                iniciarMochilaEstatica(&mochila_outros);
                inicializarMochilaEncadeada(&mochila_armas);
                jogoIniciado = 1;
                break;

            case 2: // Entrar no Jogo
                if (jogoIniciado) {
                    entrarNoJogo();
                } else {
                    printf("\nVoce precisa iniciar um 'Novo Jogo' ou 'Carregar Jogo' primeiro!\n");
                }
                break;

            case 3: // Carregar Jogo
                gerenciarCarregamento(&jogoIniciado);
                break;

            case 4: // Salvar Jogo
                gerenciarSalvamento(jogoIniciado);
                break;

            case 5: // Resetar um Save
                gerenciarReset();
                break;

            case 0:
                printf("\nSaindo do Jogo...\n");
                break;
            
            default:
                printf("\nOpcao Invalida! Tente novamente.\n");
        }

        if (opcao != 0) {
             printf("\nPressione Enter para voltar ao menu principal...");
             getchar();
        }

    } while (opcao != 0);

    // Libera a memória da mochila encadeada ao sair do programa
    liberarMochilaEncadeada(&mochila_armas);
    return 0;
}


// ############################################################################################
// - - - Implementação das funções de Menu e Gerenciamento - - -
// ############################################################################################

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirMenuPrincipal() {
    printf("\n\n#########################################\n");
    printf("        FREE FIRE - MENU PRINCIPAL\n");
    printf("#########################################\n");
    printf("1 - Novo Jogo\n");
    printf("2 - Entrar no Jogo\n");
    printf("3 - Carregar Jogo (Load Game)\n");
    printf("4 - Salvar Jogo (Save Game)\n");
    printf("5 - Resetar um Save\n");
    printf("0 - Sair\n");
    printf("-----------------------------------------\n");
    printf("Escolha uma opcao: ");
}

void exibirMenuSecundario() {
    printf("\n\n#########################################\n");    
    printf("\n\n--- MENU DE MOCHILAS ---\n");
    printf("\n\n#########################################\n");
    printf("1. Gerenciar Mochila de Armas e Roupas\n");
    printf("2. Gerenciar Mochila de Outros Itens\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("--------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

// Loop do menu secundário, onde o jogador gerencia as mochilas
void entrarNoJogo() {
    int opcao;
    do {
        exibirMenuSecundario();
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
                printf("\nVoltando ao menu principal...\n");
                break;
            default:
                printf("\nOpcao Invalida!\n");
        }

    } while (opcao != 0);
}

void gerenciarCarregamento(int *jogoIniciadoFlag) {
    mostrarSlots();
    printf("Escolha o slot para carregar (0 para cancelar): ");
    int slot;
    scanf("%d", &slot);
    limparBufferEntrada();
    if (slot > 0 && slot <= MAX_SAVES) {
        if (carregarJogo(slot)) {
            *jogoIniciadoFlag = 1; // Permite entrar e salvar após carregar
        }
    } else {
        printf("Carregamento cancelado.\n");
    }
}

void gerenciarSalvamento(int jogoIniciadoFlag) {
    if (!jogoIniciadoFlag) {
        printf("\nVoce precisa iniciar ou carregar um jogo para poder salvar!\n");
        return;
    }
    mostrarSlots();
    printf("Escolha o slot para salvar (0 para cancelar): ");
    int slot;
    scanf("%d", &slot);
    limparBufferEntrada();
    if (slot > 0 && slot <= MAX_SAVES) {
        salvarJogo(slot);
    } else {
        printf("Salvamento cancelado.\n");
    }
}

void gerenciarReset() {
    mostrarSlots();
    printf("Escolha o slot para resetar (0 para cancelar): ");
    int slot;
    scanf("%d", &slot);
    limparBufferEntrada();
    if (slot > 0 && slot <= MAX_SAVES) {
        resetarSlot(slot);
    } else {
        printf("Reset cancelado.\n");
    }
}


// ############################################################################################
// - - - Implementação das funções de Gerenciamento de Mochila (Menus Terciários) - - -
// ############################################################################################

void menuMochilaOutrosItens() { 
    int opcao;
    char nome[MAX_STR_LEN];
    char tipo[MAX_STR_LEN];
    int quantidade;

    do {
        printf("\n\n#########################################\n");    
        printf("\n--- MOCHILA DE OUTROS ITENS ---\n");
        printf("\n\n#########################################\n");
        printf("Itens na mochila: %d/%d\n", mochila_outros.quantidade, TAM_MAX);
        printf("---------------------------\n");
        printf("1. Adicionar item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens da Mochila\n");
        printf("4. Buscar item por Nome\n");
        printf("0. Voltar\n");
        printf("---------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                printf("\n--- Adicionar Novo Item (digite '0' no nome para cancelar) ---\n");
                printf("NOME do item: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                if (strcmp(nome, "0") == 0) {
                    printf("Adicao de item cancelada.\n");
                    break; 
                }
                
                printf("TIPO do item (ex: Cura, Pocao, Chave): ");
                fgets(tipo, MAX_STR_LEN, stdin);
                tipo[strcspn(tipo, "\n")] = '\0';

                printf("QUANTIDADE: ");
                scanf("%d", &quantidade);
                limparBufferEntrada();

                inserirItemEstatico(&mochila_outros, nome, tipo, quantidade);
                break;

            case 2:
                printf("Digite o NOME do item a remover (ou '0' para cancelar): ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                
                if (strcmp(nome, "0") == 0) {
                    printf("Remocao cancelada.\n");
                    break;
                }

                removerItemEstatico(&mochila_outros, nome);
                break;
            
            case 3:
                listarItensEstaticos(&mochila_outros);
                break;
            
            case 4:
                printf("Nome do item para buscar (ou '0' para cancelar): ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                if (strcmp(nome, "0") == 0) {
                    printf("Busca cancelada.\n");
                    break;
                }
                
                buscarItemEstatico(&mochila_outros, nome);
                break;

            case 0:
                printf("Voltando ao menu de mochilas...\n");
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
    int opcao;
    char nome[MAX_STR_LEN];
    char tipo[MAX_STR_LEN];
    int quantidade;

    do {
        printf("\n--- MOCHILA DE ARMAS E ROUPAS ---\n");
        printf("Itens na mochila: %d/%d\n", mochila_armas.quantidade, TAM_MAX);
        printf("----------------------------------\n");
        printf("1. Adicionar um item\n");
        printf("2. Remover um item\n");
        printf("3. Listar os itens\n");
        printf("4. Buscar um item\n");
        printf("0. Voltar\n");
        printf("----------------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                printf("\n--- Adicionar Novo Item (digite '0' no nome para cancelar) ---\n");
                printf("Nome do item: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                if (strcmp(nome, "0") == 0) {
                    printf("Adicao de item cancelada.\n");
                    break;
                }

                printf("Tipo do item (Arma, Municao, Protecao): ");
                fgets(tipo, MAX_STR_LEN, stdin);
                tipo[strcspn(tipo, "\n")] = '\0';

                printf("Quantidade: ");
                scanf("%d", &quantidade);
                limparBufferEntrada();

                inserirItemEncadeado(&mochila_armas, nome, tipo, quantidade);
                break;
            
            case 2:
                printf("Nome do item que deseja remover (ou '0' para cancelar): ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                if (strcmp(nome, "0") == 0) {
                    printf("Remocao cancelada.\n");
                    break;
                }

                removerItemEncadeado(&mochila_armas, nome);
                break;
            
            case 3:
                listarItensEncadeados(&mochila_armas);
                break;
            
            case 4:
                printf("Nome do item que deseja buscar (ou '0' para cancelar): ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                if (strcmp(nome, "0") == 0) {
                    printf("Busca cancelada.\n");
                    break;
                }

                buscarItemEncadeado(&mochila_armas, nome);
                break;

            case 0:
                printf("Voltando ao menu de mochilas...\n");
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
// - - - Implementação das funções: MOCHILA ESTÁTICA - - -
// ############################################################################################

void iniciarMochilaEstatica(MochilaEstatica *mochila) {
    mochila->quantidade = 0;
}

void inserirItemEstatico(MochilaEstatica *mochila, const char* nome, const char* tipo, int quantidade) {
    if (mochila->quantidade >= TAM_MAX) {
        printf("\nErro: Mochila de outros itens esta cheia!\n");
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
        printf("\nErro: Item \"%s\" nao encontrado na mochila.\n", nome);
        return;
    }

    for (i = pos; i < mochila->quantidade - 1; i++) {
        mochila->dados[i] = mochila->dados[i + 1];
    }

    mochila->quantidade--;
    printf("\nItem \"%s\" removido com sucesso.\n", nome);
}

void listarItensEstaticos(const MochilaEstatica *mochila) {
    printf("\n--- ITENS NA MOCHILA DE OUTROS ITENS (%d/%d) ---\n", mochila->quantidade, TAM_MAX);
    printf("----------------------------------------------------\n");
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("----------------------------------------------------\n");

    if (mochila->quantidade == 0) {
        printf("%-20s | %-15s | %s\n", "Mochila vazia.", "", "");
    } else {
        for (int i = 0; i < mochila->quantidade; i++) {
            printf("%-20s | %-15s | %d\n", mochila->dados[i].nome, mochila->dados[i].tipo, mochila->dados[i].quantidade);
        }
    }
    printf("----------------------------------------------------\n");
}

void buscarItemEstatico(const MochilaEstatica *mochila, const char* nome) {
    for (int i = 0; i < mochila->quantidade; i++) {
        if (strcmp(mochila->dados[i].nome, nome) == 0) {
            printf("\n--- Item encontrado! ---\n");
            printf("Nome:       %s\n", mochila->dados[i].nome);
            printf("Tipo:       %s\n", mochila->dados[i].tipo);
            printf("Quantidade: %d\n", mochila->dados[i].quantidade);
            printf("----------------------------------\n");
            return;
        }
    }
    printf("\n--- Item \"%s\" nao encontrado na mochila. ---\n", nome);
}


// ############################################################################################
// - - - Implementação das funções: MOCHILA ENCADEADA - - -
// ############################################################################################

void inicializarMochilaEncadeada(MochilaEncadeada *mochila) {
    mochila->cabeca = NULL;
    mochila->quantidade = 0;
}

void inserirItemEncadeado(MochilaEncadeada *mochila, const char* nome, const char* tipo, int quantidade) {
    if (mochila->quantidade >= TAM_MAX) {
        printf("\nErro: Mochila de armas e roupas cheia!\n");
        return;
    }

    No* novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("\nErro: Falha na alocacao de memoria.\n");
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
        printf("\nErro: Item \"%s\" nao encontrado na mochila.\n", nome);
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
    
    printf("\n--- ITENS NA MOCHILA DE ARMAS E ROUPAS (%d/%d) ---\n", mochila->quantidade, TAM_MAX);
    printf("----------------------------------------------------\n");
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("----------------------------------------------------\n");

    if (temp == NULL) {
        printf("%-20s | %-15s | %s\n", "Mochila vazia.", "", "");
    } else {
        while (temp != NULL) {
            printf("%-20s | %-15s | %d\n", temp->dado.nome, temp->dado.tipo, temp->dado.quantidade);
            temp = temp->proximo;
        }
    }
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

void buscarItemEncadeado(const MochilaEncadeada *mochila, const char* nome) {
    No *temp = mochila->cabeca;
    while (temp != NULL) {
        if (strcmp(temp->dado.nome, nome) == 0) {
            printf("\n--- Item encontrado! ---\n");
            printf("Nome:       %s\n", temp->dado.nome);
            printf("Tipo:       %s\n", temp->dado.tipo);
            printf("Quantidade: %d\n", temp->dado.quantidade);
            printf("----------------------------------\n");
            return;
        }
        temp = temp->proximo;
    }
    printf("\n--- Item \"%s\" nao encontrado na mochila. ---\n", nome);
}


// ############################################################################################
// - - - FUNÇÕES DE ARQUIVO (SAVE/LOAD/RESET) - - -
// ############################################################################################

void getNomeArquivo(int slot, char* nomeArquivo) {
    sprintf(nomeArquivo, "save_ff_%d.dat", slot); // Nome de arquivo diferente para não misturar com outros jogos
}

void mostrarSlots() {
    printf("\n--- SLOTS DE JOGO ---\n");
    for (int i = 1; i <= MAX_SAVES; i++) {
        char nomeArquivo[20];
        getNomeArquivo(i, nomeArquivo);
        FILE* arquivo = fopen(nomeArquivo, "rb");
        if (arquivo == NULL) {
            printf("%d. [ Vazio ]\n", i);
        } else {
            SaveState save;
            fread(&save, sizeof(SaveState), 1, arquivo);
            fclose(arquivo);
            printf("%d. %s (%s)\n", i, save.nomeSave, save.dataHora);
        }
    }
    printf("-----------------------\n");
}

void salvarJogo(int slot) {
    SaveState save;
    save.slotOcupado = 1;

    save.mochilaEstatica = mochila_outros;

    No* temp = mochila_armas.cabeca;
    int i = 0;
    while (temp != NULL && i < TAM_MAX) {
        save.mochilaEncadeadaItens[i] = temp->dado;
        temp = temp->proximo;
        i++;
    }
    save.mochilaEncadeadaQuantidade = mochila_armas.quantidade;

    printf("Digite um nome para este save (ou '0' para cancelar): ");
    fgets(save.nomeSave, 50, stdin);
    save.nomeSave[strcspn(save.nomeSave, "\n")] = '\0';

    if (strcmp(save.nomeSave, "0") == 0) {
        printf("Salvamento cancelado pelo usuario.\n");
        return; 
    }

    time_t t = time(NULL);
    strftime(save.dataHora, 30, "%d/%m/%Y %H:%M", localtime(&t));

    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo, "wb");
    if (arquivo) {
        fwrite(&save, sizeof(SaveState), 1, arquivo);
        fclose(arquivo);
        printf("\nJogo salvo com sucesso no Slot %d!\n", slot);
    } else {
        printf("\nErro ao abrir o arquivo para salvar!\n");
    }
}

int carregarJogo(int slot) {
    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("\nSlot vazio ou nao encontrado! Nao foi possivel carregar.\n");
        return 0;
    }

    SaveState save;
    fread(&save, sizeof(SaveState), 1, arquivo);
    fclose(arquivo);

    mochila_outros = save.mochilaEstatica;

    liberarMochilaEncadeada(&mochila_armas);
    for (int i = save.mochilaEncadeadaQuantidade - 1; i >= 0; i--) {
        // Ao carregar, não queremos a mensagem "Item inserido", então usamos uma versão
        // modificada da lógica de inserção aqui diretamente, ou modificamos a função.
        // Por simplicidade, vou manter a chamada para a função original que agora está corrigida.
        inserirItemEncadeado(&mochila_armas, 
                             save.mochilaEncadeadaItens[i].nome, 
                             save.mochilaEncadeadaItens[i].tipo, 
                             save.mochilaEncadeadaItens[i].quantidade);
    }
    // Descomentei a mensagem de sucesso em inserirItemEncadeado para consistência
    // então o carregamento será um pouco "barulhento". Uma solução avançada seria
    // ter uma função inserirItemSilencioso ou passar um flag.

    printf("\nJogo '%s' carregado com sucesso do Slot %d!\n", save.nomeSave, slot);
    return 1;
}

void resetarSlot(int slot) {
    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    if (remove(nomeArquivo) == 0) {
        printf("\nSlot %d resetado com sucesso!\n", slot);
    } else {
        printf("\nSlot %d ja estava vazio.\n", slot);
    }
}

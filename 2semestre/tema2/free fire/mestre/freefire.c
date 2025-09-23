// ######################################################
// FREE FIRE - Sistema de organização tática
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
    int prioridade;
    int equipado;
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

// Estrutura para a Mochila de Componentes (separada)
typedef struct {
    No* cabeca;
    int quantidade;
} MochilaComponente;


// Estrutura para salvar o estado do jogo em um arquivo
typedef struct {
    int slotOcupado;
    char nomeSave[50];
    char dataHora[30];
    MochilaEstatica mochilaEstatica;
    Item mochilaEncadeadaItens[TAM_MAX]; // Para salvar a mochila de armas
    int mochilaEncadeadaQuantidade;
    Item mochilaComponentesItens[TAM_MAX]; // Para salvar a mochila de componentes
    int mochilaComponentesQuantidade;
} SaveState;

// ######################################################
// VARIÁVEIS GLOBAIS
// ######################################################
MochilaEstatica mochila_outros;  // Mochila global para outros itens
MochilaEncadeada mochila_armas; // Mochila global para armas e roupas
MochilaComponente mochila_componentes; // Mochila global para componentes

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
void menuMochilaComponentes();
void mostrarCreditos();
void menuPersonagem();

// --- Funções da Mochila Estática ---
void iniciarMochilaEstatica(MochilaEstatica *mochila);
void inserirItemEstatico(MochilaEstatica *mochila, const char* nome, const char* tipo, int quantidade);
void removerItemEstatico(MochilaEstatica *mochila, const char* nome);
void listarItensEstaticos(const MochilaEstatica *mochila);
void buscaBinariaEstatica(const MochilaEstatica *mochila, const char* nome);

// --- Funções da Mochila Encadeada ---
void iniciarMochilaEncadeada(MochilaEncadeada *mochila);
void inserirItemEncadeado(MochilaEncadeada *mochila, const char* nome, const char* tipo, int quantidade);
void removerItemEncadeado(MochilaEncadeada *mochila, const char* nome);
void listarItensEncadeados(const MochilaEncadeada *mochila);
void liberarMochilaEncadeada(MochilaEncadeada *mochila);
void buscaBinariaEncadeada(const MochilaEncadeada *mochila, const char* nome);
void equiparItem(MochilaEncadeada *mochila);
void verPersonagem(const MochilaEncadeada *mochila);

// --- Funções da Mochila Componente ---
void iniciarMochilaComponente(MochilaComponente *mochila);
void inserirComponente(MochilaComponente *mochila, const char* nome, const char* tipo, int quantidade, int prioridade);
void removerComponente(MochilaComponente *mochila, const char* nome);
void listarComponentes(const MochilaComponente *mochila);
void buscaBinariaComponente(const MochilaComponente *mochila, const char* nome);
void liberarMochilaComponente(MochilaComponente *mochila);

// --- Funções de Ordenação ---
void gerenciarOrdenacaoOutrosItens(char** statusOrdem);
void gerenciarOrdenacaoArmasRoupas(char** statusOrdem);
void gerenciarOrdenacaoComponentes(char** statusOrdem);
void ordenarMochilaEstaticaBubbleSort(MochilaEstatica *mochila, int criterio);
void ordenarMochilaEncadeadaInsertionSort(MochilaEncadeada *mochila, int criterio);
void ordenarMochilaComponenteSelectionSort(MochilaComponente *mochila, int criterio);

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
                iniciarMochilaEncadeada(&mochila_armas);
                iniciarMochilaComponente(&mochila_componentes);
                jogoIniciado = 1;
                break;

            case 2: // Entrar no Jogo
                if (jogoIniciado) {
                    entrarNoJogo();
                } else {
                    printf("\nVoce precisa iniciar um 'Novo Jogo' ou 'Carregar um Jogo salvo' primeiro!\n");
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
            
            case 6: // Creditos
                mostrarCreditos();
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

    // Libera a memória das mochilas encadeadas ao sair do programa
    liberarMochilaEncadeada(&mochila_armas);
    liberarMochilaComponente(&mochila_componentes);
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
    printf("6 - Creditos\n"); 
    printf("0 - Sair\n");
    printf("-----------------------------------------\n");
    printf("Escolha uma opcao: ");
}

void exibirMenuSecundario() {
    printf("\n\n#########################################\n");    
    printf("--- FREE FIRE - MENU SECUNDÁRIO ---\n");
    printf("#########################################\n\n");
    printf("1. Ver personagem\n");    
    printf("2. Gerenciar Mochila de Armas e Roupas\n");
    printf("3. Gerenciar Mochila de Outros Itens\n");
    printf("4. Gerenciar Componentes\n");
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
                menuPersonagem();
                break;
            case 2:
                menuMochilaArmasRoupas();
                break;
            case 3:
                menuMochilaOutrosItens();
                break;
            case 4:
                menuMochilaComponentes();
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
            *jogoIniciadoFlag = 1;
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

void menuPersonagem() {
    int opcao;
    do {
        printf("\n\n#########################################\n");
        printf("        FREE FIRE - PERSONAGEM\n");
        printf("#########################################\n");
        printf("1 - Iniciar Partida\n");
        printf("2 - Ver Itens Equipados\n");
        printf("0 - Voltar\n");
        printf("-----------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                printf("\nFuncionalidade 'Iniciar Partida' ainda nao implementada.\n");
                break;
            case 2:
                verPersonagem(&mochila_armas);
                break;
            case 0:
                printf("\nVoltando...\n");
                break;
            default:
                printf("\nOpcao Invalida!\n");
        }

        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
    } while (opcao != 0);
}

// ############################################################################################
// - - - Implementação das funções de Gerenciamento de Mochila (Menus Terciários) - - -
// ############################################################################################

void menuMochilaOutrosItens() { 
    int opcao;
    char nome[MAX_STR_LEN];
    char tipo[MAX_STR_LEN];
    int quantidade;
    char* ordem = "Nao Ordenado";

    do {
        printf("\n\n#########################################\n");    
        printf("--- MOCHILA DE ITENS (ESTATICA) ---\n");
        printf("#########################################\n");
        printf("Itens na mochila: %d/%d\n", mochila_outros.quantidade, TAM_MAX);
        printf("Status da Ordenacao: %s\n\n", ordem);

        printf("1. Adicionar item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Organizar itens\n");
        printf("5. Buscar item por Nome (Busca Binaria)\n");
        printf("0. Voltar\n");
        printf("---------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                printf("\n--- Adicionar Novo item (digite '0' no nome para cancelar) ---\n");
                printf("NOME do item: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                if (strcmp(nome, "0") == 0) {
                    printf("Adicao de item cancelada.\n");
                    break; 
                }
                
                printf("TIPO do item (ex: Cura, Municao, ...): ");
                fgets(tipo, MAX_STR_LEN, stdin);
                tipo[strcspn(tipo, "\n")] = '\0';

                printf("QUANTIDADE: ");
                scanf("%d", &quantidade);
                limparBufferEntrada();

                inserirItemEstatico(&mochila_outros, nome, tipo, quantidade);
                listarItensEstaticos(&mochila_outros); // Mostra a mochila
                ordem = "Nao Ordenado";
                break;

            case 2:
                printf("Digite o nome do item a remover (ou '0' para cancelar): ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                
                if (strcmp(nome, "0") == 0) {
                    printf("Remocao cancelada.\n");
                    break;
                }

                removerItemEstatico(&mochila_outros, nome);
                ordem = "Nao Ordenado";
                break;
            
            case 3:
                listarItensEstaticos(&mochila_outros);
                break;
            
            case 4:
                gerenciarOrdenacaoOutrosItens(&ordem); // Chama o sub-menu de ordenação
                break;
            
            case 5:
                if (strcmp(ordem, "Nao Ordenado") == 0) {
                    printf("\n\"ALERTA\": A busca binaria requer que a mochila esteja ordenada por Nome.\n");
                    printf("Use a opcao 4 para organizar a mochila primeiro.\n");
                } else {
                    printf("Nome do item para buscar (ou '0' para cancelar): ");
                    fgets(nome, MAX_STR_LEN, stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    if (strcmp(nome, "0") == 0) {
                        printf("Busca cancelada.\n");
                        break;
                    }
                    buscaBinariaEstatica(&mochila_outros, nome);
                }
                break;

            case 0:
                printf("Voltando ao menu do jogo...\n");
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
    char* ordem = "Nao Ordenado";

    do {
        printf("\n\n#########################################\n");    
        printf("--- MOCHILA DE ARMAS E ROUPAS (ENCADEADA)---\n");
        printf("#########################################\n");
        printf("Itens na mochila: %d/%d\n", mochila_armas.quantidade, TAM_MAX);
        printf("Status da Ordenacao: %s\n\n", ordem);

        printf("1. Adicionar arma ou roupa\n");
        printf("2. Remover arma ou roupa\n");
        printf("3. Listar armas e roupas\n");
        printf("4. Organizar itens\n");
        printf("5. Buscar arma ou roupa por Nome (Busca Binaria)\n");
        printf("6. Equipar um item\n");
        printf("0. Voltar\n");
        printf("----------------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                printf("\n--- Adicionar Nova Arma ou Roupa (digite '0' no nome para cancelar) ---\n");
                printf("Nome do item: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                if (strcmp(nome, "0") == 0) {
                    printf("Adicao de item cancelada.\n");
                    break;
                }

                printf("Tipo do item (Arma, Roupa, ...): ");
                fgets(tipo, MAX_STR_LEN, stdin);
                tipo[strcspn(tipo, "\n")] = '\0';

                printf("Quantidade: ");
                scanf("%d", &quantidade);
                limparBufferEntrada();

                inserirItemEncadeado(&mochila_armas, nome, tipo, quantidade);
                listarItensEncadeados(&mochila_armas); // Mostra a mochila
                ordem = "Nao Ordenado";
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
                ordem = "Nao Ordenado";
                break;
            
            case 3:
                listarItensEncadeados(&mochila_armas);
                break;
            
            case 4:
                gerenciarOrdenacaoArmasRoupas(&ordem); // Chama o sub-menu de ordenação
                break;

             case 5:
                if (strcmp(ordem, "Nao Ordenado") == 0) {
                    printf("\n\"ALERTA\": A busca binaria requer que a mochila esteja ordenada por Nome.\n");
                    printf("Use a opcao 4 para organizar a mochila primeiro.\n");
                } else {
                    printf("Nome do item que deseja buscar (ou '0' para cancelar): ");
                    fgets(nome, MAX_STR_LEN, stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    if (strcmp(nome, "0") == 0) {
                        printf("Busca cancelada.\n");
                        break;
                    }
                    buscaBinariaEncadeada(&mochila_armas, nome);
                }
                break;

             case 6:
                equiparItem(&mochila_armas);
                break;

            case 0:
                printf("Voltando ao menu do jogo...\n");
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

void menuMochilaComponentes() { 
    int opcao;
    char nome[MAX_STR_LEN];
    char tipo[MAX_STR_LEN];
    int quantidade, prioridade;
    char* ordem = "Nao Ordenado";

    do {
        printf("\n\n#########################################\n");    
        printf("--- MOCHILA DE COMPONENTES (ENCADEADA)---\n");
        printf("#########################################\n");
        printf("Itens na mochila: %d/%d\n", mochila_componentes.quantidade, TAM_MAX);
        printf("Status da Ordenacao: %s\n\n", ordem);

        printf("1. Adicionar um componente\n");
        printf("2. Remover um componente\n");
        printf("3. Listar os componentes\n");
        printf("4. Organizar componentes\n");
        printf("5. Buscar um componente (por nome - Busca Binaria)\n");
        printf("6. Ativar torre de fuga \n");
        printf("0. Voltar\n");
        printf("----------------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                printf("\n--- Adicionar Novo Componente (digite '0' no nome para cancelar) ---\n");
                printf("Nome do componente: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                if (strcmp(nome, "0") == 0) {
                    printf("Adicao de componente cancelada.\n");
                    break;
                }

                printf("Tipo do componente (Estrutural, Eletronico, Energia, ...): ");
                fgets(tipo, MAX_STR_LEN, stdin);
                tipo[strcspn(tipo, "\n")] = '\0';

                printf("Quantidade: ");
                scanf("%d", &quantidade);

                printf("Prioridade de Montagem (1-5): ");
                scanf("%d", &prioridade);
                limparBufferEntrada();

                inserirComponente(&mochila_componentes, nome, tipo, quantidade, prioridade);
                listarComponentes(&mochila_componentes); // Mostra a mochila
                ordem = "Nao Ordenado";
                break;
            
            case 2:
                printf("Nome do item que deseja remover (ou '0' para cancelar): ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                if (strcmp(nome, "0") == 0) {
                    printf("Remocao cancelada.\n");
                    break;
                }

                removerComponente(&mochila_componentes, nome);
                ordem = "Nao Ordenado";
                break;
            
            case 3:
                listarComponentes(&mochila_componentes);
                break;
            
            case 4:
                gerenciarOrdenacaoComponentes(&ordem); // Chama o sub-menu de ordenação
                break;
            
            case 5: 
                if (strcmp(ordem, "Nao Ordenado") == 0) {
                    printf("\n\"ALERTA\": A busca binaria requer que a mochila esteja ordenada por Nome.\n");
                    printf("Use a opcao 4 para organizar a mochila primeiro.\n");
                } else {
                    printf(" - - - Busca Binária por Componente-Chave - - - \n");
                    printf("Digite 0 para cancelar \n");
                    printf("Nome do componente que deseja buscar: ");
                    fgets(nome, MAX_STR_LEN, stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    if (strcmp(nome, "0") == 0) {
                        printf("Busca cancelada.\n");
                        break;
                    }
                    buscaBinariaComponente(&mochila_componentes, nome);
                }
                break;
            
            case 6:
                printf("\nFuncionalidade 'Ativar torre de fuga' ainda nao implementada.\n");
                break;

            case 0:
                printf("Voltando ao menu do jogo...\n");
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
        printf("\nErro: Mochila de itens esta cheia!\n");
        return;
    }
    strcpy(mochila->dados[mochila->quantidade].nome, nome);
    strcpy(mochila->dados[mochila->quantidade].tipo, tipo);
    mochila->dados[mochila->quantidade].quantidade = quantidade;
    mochila->dados[mochila->quantidade].prioridade = 0;
    mochila->dados[mochila->quantidade].equipado = 0; // Inicializa como não equipado
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
    printf("\n--- ITENS NA MOCHILA DE ITENS (%d/%d) ---\n", mochila->quantidade, TAM_MAX);
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

void buscaBinariaEstatica(const MochilaEstatica *mochila, const char* nome) {
    int inicio = 0;
    int fim = mochila->quantidade - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int comparacao = strcmp(mochila->dados[meio].nome, nome);

        if (comparacao == 0) {
            printf("\n--- Item encontrado! ---\n");
            printf("Nome:       %s\n", mochila->dados[meio].nome);
            printf("Tipo:       %s\n", mochila->dados[meio].tipo);
            printf("Quantidade: %d\n", mochila->dados[meio].quantidade);
            printf("----------------------------------\n");
            return;
        }
        if (comparacao < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    printf("\n--- Item \"%s\" nao encontrado na mochila. ---\n", nome);
}

// ############################################################################################
// - - - Implementação das funções: MOCHILA ENCADEADA - - -
// ############################################################################################

void iniciarMochilaEncadeada(MochilaEncadeada *mochila) {
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
    novoNo->dado.prioridade = 0;
    novoNo->proximo = mochila->cabeca;
    mochila->cabeca = novoNo;
    mochila->quantidade++;

    printf("\nArma/Roupa \"%s\" inserido com sucesso.\n", nome);
}

void removerItemEncadeado(MochilaEncadeada *mochila, const char* nome) {
    No *atual = mochila->cabeca;
    No *anterior = NULL;

    while (atual != NULL && strcmp(atual->dado.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if(atual == NULL) {
        printf("\nErro: Arma/Roupa \"%s\" nao encontrado na mochila.\n", nome);
        return;
    }

    if (anterior == NULL) {
        mochila->cabeca = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    mochila->quantidade--;
    printf("\nArma/Roupa \"%s\" removido com sucesso.\n", nome);
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

void buscaBinariaEncadeada(const MochilaEncadeada *mochila, const char* nome) {
    if (mochila->quantidade == 0) {
        printf("\n--- Item \"%s\" nao encontrado na mochila. ---\n", nome);
        return;
    }

    Item tempArray[mochila->quantidade];
    No* atual = mochila->cabeca;
    for (int i = 0; i < mochila->quantidade; i++) {
        tempArray[i] = atual->dado;
        atual = atual->proximo;
    }

    int inicio = 0;
    int fim = mochila->quantidade - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int comparacao = strcmp(tempArray[meio].nome, nome);

        if (comparacao == 0) {
            printf("\n--- Arma/Roupa encontrado! ---\n");
            printf("Nome:       %s\n", tempArray[meio].nome);
            printf("Tipo:       %s\n", tempArray[meio].tipo);
            printf("Quantidade: %d\n", tempArray[meio].quantidade);
            printf("----------------------------------\n");
            return;
        }
        if (comparacao < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    printf("\n--- Arma/Roupa \"%s\" nao encontrado na mochila. ---\n", nome);
}

void equiparItem(MochilaEncadeada *mochila) {
    char nome[MAX_STR_LEN];
    printf("\n--- Equipar Arma/Roupa ---\n");
    
    listarItensEncadeados(mochila);

    if (mochila->quantidade == 0) {
        printf("Nao ha Arma/Roupa para equipar.\n");
        return;
    }

    printf("Digite o NOME da Arma/Roupa a equipar (ou '0' para cancelar): ");
    fgets(nome, MAX_STR_LEN, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    
    if (strcmp(nome, "0") == 0) {
        printf("Operacao cancelada.\n");
        return;
    }

    No *temp = mochila->cabeca;
    int encontrado = 0;
    while (temp != NULL) {
        if (strcmp(temp->dado.nome, nome) == 0) {
            encontrado = 1;
            break;
        }
        temp = temp->proximo;
    }

    if (encontrado) {
        printf("\nArma/Roupa \"%s\" equipado com sucesso!\n", nome);
        temp->dado.equipado = 1; // Marca como equipado
    } else {
        printf("\nArma/Roupa \"%s\" nao encontrado na mochila.\n", nome);
    }
}

void verPersonagem(const MochilaEncadeada *mochila) {
    No *temp = mochila->cabeca;
    int itensEquipados = 0;
    
    printf("\n--- PERSONAGEM EQUIPADO ---\n");
    printf("----------------------------------------------------\n");
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("----------------------------------------------------\n");

    while (temp != NULL) {
        if (temp->dado.equipado) {
            printf("%-20s | %-15s | %d\n", temp->dado.nome, temp->dado.tipo, temp->dado.quantidade);
            itensEquipados++;
        }
        temp = temp->proximo;
    }

    if (itensEquipados == 0) {
        printf("%-20s | %-15s | %s\n", "Nenhum item equipado.", "", "");
    }
    printf("----------------------------------------------------\n");
}

// ############################################################################################
// - - - Implementação das funções: MOCHILA COMPONENTES - - -
// ############################################################################################

void iniciarMochilaComponente(MochilaComponente *mochila) {
    mochila->cabeca = NULL;
    mochila->quantidade = 0;
}

void inserirComponente(MochilaComponente *mochila, const char* nome, const char* tipo, int quantidade, int prioridade) {
    if (mochila->quantidade >= TAM_MAX) {
        printf("\nErro: Mochila de componentes cheia!\n");
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
    novoNo->dado.prioridade = prioridade;
    novoNo->proximo = NULL; // O novo nó sempre será o último, então seu 'proximo' aponta para NULL.

    // Verifica se a lista está vazia. Se estiver, o novo nó se torna a cabeça.
    if (mochila->cabeca == NULL) {
        mochila->cabeca = novoNo;
    } else {
        // Se a lista não estiver vazia, percorre até o final
        No* ultimo = mochila->cabeca;
        while (ultimo->proximo != NULL) {
            ultimo = ultimo->proximo;
        }
        // Adiciona o novo nó depois do último elemento encontrado
        ultimo->proximo = novoNo;
    }
    mochila->quantidade++;
    printf("\nComponente \"%s\" inserido com sucesso.\n", nome);
}

void removerComponente(MochilaComponente *mochila, const char* nome) {
    No *atual = mochila->cabeca;
    No *anterior = NULL;
    while (atual != NULL && strcmp(atual->dado.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }
    if(atual == NULL) {
        printf("\nErro: Componente \"%s\" nao encontrado na mochila.\n", nome);
        return;
    }
    if (anterior == NULL) {
        mochila->cabeca = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    free(atual);
    mochila->quantidade--;
    printf("\nComponente \"%s\" removido com sucesso.\n", nome);
}

void listarComponentes(const MochilaComponente *mochila) {
    No *temp = mochila->cabeca;
    printf("\n--- ITENS NA MOCHILA DE COMPONENTES (%d/%d) ---\n", mochila->quantidade, TAM_MAX);
    printf("------------------------------------------------------------------\n");
    printf("%-20s | %-15s | %-10s | %s\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("------------------------------------------------------------------\n");
    if (temp == NULL) {
        printf("%-20s | %-15s | %-10s | %s\n", "Mochila vazia.", "", "", "");
    } else {
        while (temp != NULL) {
            printf("%-20s | %-15s | %-10d | %d\n", temp->dado.nome, temp->dado.tipo, temp->dado.quantidade, temp->dado.prioridade);
            temp = temp->proximo;
        }
    }
    printf("------------------------------------------------------------------\n");
}

void liberarMochilaComponente(MochilaComponente *mochila) {
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

void buscaBinariaComponente(const MochilaComponente *mochila, const char* nome) {
    if (mochila->quantidade == 0) {
        printf("\n--- Componente \"%s\" nao encontrado na mochila. ---\n", nome);
        return;
    }

    Item tempArray[mochila->quantidade];
    No* atual = mochila->cabeca;
    for (int i = 0; i < mochila->quantidade; i++) {
        tempArray[i] = atual->dado;
        atual = atual->proximo;
    }

    int inicio = 0;
    int fim = mochila->quantidade - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int comparacao = strcmp(tempArray[meio].nome, nome);

        if (comparacao == 0) {
            printf("\n - - - Componente-Chave encontrado! - - - \n");
            printf("Nome:       %s\n", tempArray[meio].nome);
            printf("Tipo:       %s\n", tempArray[meio].tipo);
            printf("Quantidade: %d\n", tempArray[meio].quantidade);
            printf("Prioridade: %d\n", tempArray[meio].prioridade);
            printf("----------------------------------\n");
            return;
        }
        if (comparacao < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    printf("\n--- Componente \"%s\" nao encontrado na mochila. ---\n", nome);
}

// ############################################################################################
// - - - Implementação das funções de ORDENAÇÃO - - -
// ############################################################################################

// Funções de sub-menu de ordenação

void gerenciarOrdenacaoOutrosItens(char** statusOrdem) {
    int criterio;
    printf("\n- - - Estrategia de Organizacao - - -\n");
    printf("1. Por Nome (ordem alfabetica)\n");
    printf("2. Por Tipo\n");
    printf("0. Cancelar\n");
    printf("Escolha o criterio: ");
    scanf("%d", &criterio);
    limparBufferEntrada();

    if (criterio == 1 || criterio == 2) {
        ordenarMochilaEstaticaBubbleSort(&mochila_outros, criterio);
        printf("\nMochila de itens organizada com sucesso!\n");
        *statusOrdem = (criterio == 1) ? "Ordenado por Nome" : "Ordenado por Tipo";
    } else if (criterio == 0) {
        printf("\nOrganizacao cancelada.\n");
    } else {
        printf("\nCriterio invalido!\n");
    }
}

void gerenciarOrdenacaoArmasRoupas(char** statusOrdem) {
    int criterio;
    printf("\n- - - Estrategia de Organizacao - - -\n");
    printf("1. Por Nome (ordem alfabetica)\n");
    printf("2. Por Tipo\n");
    printf("0. Cancelar\n");
    printf("Escolha o criterio: ");
    scanf("%d", &criterio);
    limparBufferEntrada();

    if (criterio == 1 || criterio == 2) {
        ordenarMochilaEncadeadaInsertionSort(&mochila_armas, criterio);
        printf("\nMochila de armas e roupas organizada com sucesso!\n");
        *statusOrdem = (criterio == 1) ? "Ordenado por Nome" : "Ordenado por Tipo";
    } else if (criterio == 0) {
        printf("\nOrganizacao cancelada.\n");
    } else {
        printf("\nCriterio invalido!\n");
    }
}

void gerenciarOrdenacaoComponentes(char** statusOrdem) {
    int criterio;
    printf("\n- - - Estrategia de Organizacao - - -\n");
    printf("1. Por Nome (ordem alfabetica)\n");
    printf("2. Por Tipo\n");
    printf("3. Por Prioridade de Montagem\n");
    printf("0. Cancelar\n");
    printf("Escolha o criterio: ");
    scanf("%d", &criterio);
    limparBufferEntrada();

    if (criterio >= 1 && criterio <= 3) {
        ordenarMochilaComponenteSelectionSort(&mochila_componentes, criterio);
        printf("\nMochila de componentes organizada com sucesso!\n");
        if (criterio == 1) *statusOrdem = "Ordenado por Nome";
        else if (criterio == 2) *statusOrdem = "Ordenado por Tipo";
        else *statusOrdem = "Ordenado por Prioridade";
    } else if (criterio == 0) {
        printf("\nOrganizacao cancelada.\n");
    } else {
        printf("\nCriterio invalido!\n");
    }
}

// Funções de sub-menu de ordenação

// Funções de ordenação agora recebem um critério

void ordenarMochilaEstaticaBubbleSort(MochilaEstatica *mochila, int criterio) {
    int i, j;
    Item temp;
    int n = mochila->quantidade;
    int trocou;
    int comparacao;

    for (i = 0; i < n - 1; i++) {
        trocou = 0;
        for (j = 0; j < n - i - 1; j++) {
            comparacao = 0; // Reset
            if (criterio == 1) { // Por Nome
                comparacao = strcmp(mochila->dados[j].nome, mochila->dados[j + 1].nome);
            } else if (criterio == 2) { // Por Tipo
                comparacao = strcmp(mochila->dados[j].tipo, mochila->dados[j + 1].tipo);
            }

            if (comparacao > 0) {
                temp = mochila->dados[j];
                mochila->dados[j] = mochila->dados[j + 1];
                mochila->dados[j + 1] = temp;
                trocou = 1;
            }
        }
        if (trocou == 0) {
            break;
        }
    }
}

void ordenarMochilaEncadeadaInsertionSort(MochilaEncadeada *mochila, int criterio) {
    if (mochila->cabeca == NULL || mochila->cabeca->proximo == NULL) {
        return;
    }
    No *ordenada = NULL;
    No *atual = mochila->cabeca;
    while (atual != NULL) {
        No *proximo = atual->proximo;
        
        int comp_atual_vs_ordenada = 0;
        if (criterio == 1) comp_atual_vs_ordenada = strcmp(ordenada->dado.nome, atual->dado.nome);
        else if (criterio == 2) comp_atual_vs_ordenada = strcmp(ordenada->dado.tipo, atual->dado.tipo);

        if (ordenada == NULL || comp_atual_vs_ordenada >= 0) {
            atual->proximo = ordenada;
            ordenada = atual;
        } else {
            No *cursor = ordenada;

            int comp_cursor_vs_atual = 0;
            if (criterio == 1) comp_cursor_vs_atual = strcmp(cursor->proximo->dado.nome, atual->dado.nome);
            else if (criterio == 2) comp_cursor_vs_atual = strcmp(cursor->proximo->dado.tipo, atual->dado.tipo);

            while (cursor->proximo != NULL && comp_cursor_vs_atual < 0) {
                cursor = cursor->proximo;
                if (cursor->proximo != NULL) {
                    if (criterio == 1) comp_cursor_vs_atual = strcmp(cursor->proximo->dado.nome, atual->dado.nome);
                    else if (criterio == 2) comp_cursor_vs_atual = strcmp(cursor->proximo->dado.tipo, atual->dado.tipo);
                }
            }
            atual->proximo = cursor->proximo;
            cursor->proximo = atual;
        }
        atual = proximo;
    }
    mochila->cabeca = ordenada;
}

void ordenarMochilaComponenteSelectionSort(MochilaComponente *mochila, int criterio) {
    No *i, *j;
    Item temp;

    for (i = mochila->cabeca; i != NULL && i->proximo != NULL; i = i->proximo) {
        No *min = i;
        for (j = i->proximo; j != NULL; j = j->proximo) {
            int deveTrocar = 0;
            if (criterio == 1) { // Por Nome
                if (strcmp(j->dado.nome, min->dado.nome) < 0) deveTrocar = 1;
            } else if (criterio == 2) { // Por Tipo
                if (strcmp(j->dado.tipo, min->dado.tipo) < 0) deveTrocar = 1;
            } else if (criterio == 3) { // Por Prioridade
                if (j->dado.prioridade > min->dado.prioridade) deveTrocar = 1;
            }

            if (deveTrocar) {
                min = j;
            }
        }
        if (min != i) {
            temp = i->dado;
            i->dado = min->dado;
            min->dado = temp;
        }
    }
}

// ############################################################################################
// - - - FUNÇÕES DE ARQUIVO (SAVE/LOAD/RESET) - - -
// ############################################################################################

void getNomeArquivo(int slot, char* nomeArquivo) {
    sprintf(nomeArquivo, "save_ff_%d.dat", slot);
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

    No* temp_armas = mochila_armas.cabeca;
    int i = 0;
    while (temp_armas != NULL && i < TAM_MAX) {
        save.mochilaEncadeadaItens[i] = temp_armas->dado;
        temp_armas = temp_armas->proximo;
        i++;
    }
    save.mochilaEncadeadaQuantidade = mochila_armas.quantidade;

    No* temp_comp = mochila_componentes.cabeca;
    i = 0;
    while (temp_comp != NULL && i < TAM_MAX) {
        save.mochilaComponentesItens[i] = temp_comp->dado;
        temp_comp = temp_comp->proximo;
        i++;
    }
    save.mochilaComponentesQuantidade = mochila_componentes.quantidade;

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
        inserirItemEncadeado(&mochila_armas, 
                             save.mochilaEncadeadaItens[i].nome, 
                             save.mochilaEncadeadaItens[i].tipo, 
                             save.mochilaEncadeadaItens[i].quantidade);
    }

    liberarMochilaComponente(&mochila_componentes);
    for (int i = save.mochilaComponentesQuantidade - 1; i >= 0; i--) {
        inserirComponente(&mochila_componentes, 
                             save.mochilaComponentesItens[i].nome, 
                             save.mochilaComponentesItens[i].tipo, 
                             save.mochilaComponentesItens[i].quantidade,
                             save.mochilaComponentesItens[i].prioridade);
    }

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

void mostrarCreditos() {
    printf("\n\n#########################################\n");
    printf("        FREE FIRE - CREDITOS\n");
    printf("#########################################\n");
    printf("Jogo desenvolvido por: Thiago Marcel\n");
    printf("Disciplina: Estrutura de Dados\n");
    printf("Professor: Daisy Albuquerque\n");
    printf("#########################################\n");
}

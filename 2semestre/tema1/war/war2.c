// ##################
// WAR PARTE 2
// ##################

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // biblioteca que tem strings
#include <time.h> // Para que os dados sejam diferentes a cada vez que você joga, precisamos de números aleatórios.

// Adicione as duas linhas abaixo
#define TAM_STRING 20
#define TAM_INT 3
#define MAX_TER 5
#define MAX_SAVES 5 // Ela conterá os territórios e as informações extras.

// ######################
// Definição da estrutura
// ######################

struct Territorio {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropa;
};

struct SaveState {
    int slotOcupado; // 1 se o slot tem dados, 0 se está vazio
    char nomeSave[50];
    char dataHora[30];
    int totalTerritorios;
    struct Territorio territorios[MAX_TER];
};

// ######################################
// Função para limpar o buffer de entrada
// ######################################

void limparBufferEntrada() {
    int c;
    while (
        (c = getchar()
        ) != '\n' && c != EOF);
}

// #######################################################
// Função para salvar os territórios em um arquivo binário
// #######################################################
    
// Gera o nome do arquivo para um slot (ex: "save_1.dat")
void getNomeArquivo(int slot, char* nomeArquivo) {
    sprintf(nomeArquivo, "save_%d.dat", slot);
}

// Mostra a lista de saves disponíveis
void mostrarSlots() {
    printf("\n--- Escolha um Slot ---\n");
    for (int i = 0; i < MAX_SAVES; i++) {
        char nomeArquivo[20];
        getNomeArquivo(i + 1, nomeArquivo);
        FILE* arquivo = fopen(nomeArquivo, "rb");

        if (arquivo == NULL) {
            printf("%d. [ Vazio ]\n", i + 1);
        } else {
            struct SaveState save;
            fread(&save, sizeof(struct SaveState), 1, arquivo);
            fclose(arquivo);
            printf("%d. %s (%s)\n", i + 1, save.nomeSave, save.dataHora);
        }
    }
    printf("0. Cancelar\n");
    printf("-----------------------\n");
}

// Salva o estado atual do jogo em um slot específico
void salvarJogo(int slot, struct Territorio biblioteca[], int total) {
    if (slot < 1 || slot > MAX_SAVES) {
        printf("Slot invalido!\n");
        return;
    }

    struct SaveState save;
    save.slotOcupado = 1;
    save.totalTerritorios = total;
    for (int i = 0; i < total; i++) {
        save.territorios[i] = biblioteca[i];
    }
    
    printf("Digite um nome para este save: ");
    fgets(save.nomeSave, 50, stdin);
    save.nomeSave[strcspn(save.nomeSave, "\n")] = '\0';

    time_t t = time(NULL);
    strftime(save.dataHora, 30, "%d/%m/%Y %H:%M", localtime(&t));

    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo, "wb");
    fwrite(&save, sizeof(struct SaveState), 1, arquivo);
    fclose(arquivo);

    printf("\nJogo salvo com sucesso no Slot %d!\n", slot);
}

// Carrega um jogo de um slot e o coloca no estado atual do programa
int carregarJogo(int slot, struct Territorio biblioteca[]) {
    if (slot < 1 || slot > MAX_SAVES) {
        printf("Slot invalido!\n");
        return 0;
    }

    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo, "rb");

    if (arquivo == NULL) {
        printf("Slot vazio ou nao encontrado!\n");
        return 0;
    }
    
    struct SaveState save;
    fread(&save, sizeof(struct SaveState), 1, arquivo);
    fclose(arquivo);

    for (int i = 0; i < save.totalTerritorios; i++) {
        biblioteca[i] = save.territorios[i];
    }

    printf("\nJogo carregado com sucesso do Slot %d!\n", slot);
    return save.totalTerritorios;
}

// Apaga (reseta) um slot de save
void resetarSlot(int slot) {
    if (slot < 1 || slot > MAX_SAVES) {
        printf("Slot invalido!\n");
        return;
    }
    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    if (remove(nomeArquivo) == 0) {
        printf("Slot %d resetado com sucesso!\n", slot);
    } else {
        printf("Slot %d ja estava vazio.\n", slot);
    }
}

// ######################################
// Função Principal (main)
// ######################################
int main() {
    srand(time(NULL));

    struct Territorio biblioteca[MAX_TER];
    int totalTerritorios = 0; // Começa sempre com 0, pois o jogador escolhe o que carregar
    int opcao;
    int slotEscolhido;

    do {
        printf("#########################################\n");
        printf("         WAR - TERRITORIOS\n");
        printf("#########################################\n");
        printf("1 - Novo Jogo (Cadastrar Territorios)\n");
        printf("2 - Entrar no Jogo\n");
        printf("3 - Carregar Jogo (Load Game)\n");
        printf("4 - Salvar Jogo (Save Game)\n");
        printf("5 - Resetar um Save\n");
        printf("0 - Sair\n");
        printf("-----------------------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1: // NOVO JOGO
                printf("Iniciando novo jogo...\n");
                // A única diferença é que ele deve começar com:
                printf("Vamos cadastrar os 5 territorios iniciais do nosso mundo \n\n");
                
                for (int i = 0; i < MAX_TER; i++) {
                    printf("- - - Cadastrando o %dº Territorio - - - \n", i + 1);

                    printf("Nome do Territorio: ");
                    fgets (biblioteca[i].nome, TAM_STRING, stdin);

                    printf("Cor do Exercito (ex: Azul, verde, etc):");
                    fgets (biblioteca[i].cor, TAM_STRING, stdin);

                    printf("Numero de tropas: ");
                    scanf("%d", &biblioteca[i].tropa);
                    limparBufferEntrada(); // Limpar o buffer apos o scanf

                    // Remove o ˋ\nˋ que o fgets deixa no final
                    biblioteca[i].nome[strcspn(biblioteca[i].nome, "\n")] = '\0';
                    biblioteca[i].cor[strcspn(biblioteca[i].cor, "\n")] = '\0';

                    printf(" --> Territorio %d cadastrado com sucesso!\n\n", i + 1);
                }
                totalTerritorios = MAX_TER;
                printf("Cadastro concluido! Voce ja pode jogar ou salvar.\n");
                break;

            case 2: // ENTRAR NO JOGO
                if (totalTerritorios == 0) {
                    printf("\nNenhum jogo em andamento. Inicie um Novo Jogo ou Carregue um save.\n");
                } else {
                    int atacante, defensor;

                    // Laço para permitir múltiplos ataques em sequência
                    while (1) {
                        // Mostra o mapa atual antes de cada ataque
                        printf("##########################################\n");
                        printf("- - - MAPA DO MUNDO - ESTADO ATUAL - - -\n");
                        printf("##########################################\n\n");
                        for (int i = 0; i < totalTerritorios; i++) {
                            printf("%d. %s (Cor: %s, Tropas: %d)\n", i + 1, biblioteca[i].nome, biblioteca[i].cor, biblioteca[i].tropa);
                        }
                        printf("---------------------------------------\n\n");

                        // 1. ESCOLHA DOS TERRITÓRIOS
                        printf("- - - FASE DE ATAQUE - - -\n");
                        printf("Escolha o territorio ATACANTE (1 a %d, ou 0 para voltar ao menu): ", MAX_TER);
                        scanf("%d", &atacante);
                        limparBufferEntrada();

                        // Se o jogador digitar 0, sai da fase de ataque e volta ao menu
                        if (atacante == 0) {
                            break;
                        }

                        printf("Escolha o territorio DEFENSOR (1 a %d): ", MAX_TER);
                        scanf("%d", &defensor);
                        limparBufferEntrada();
                        
                        // Validação das escolhas
                        if (atacante < 1 || atacante > MAX_TER || defensor < 1 || defensor > MAX_TER) {
                            printf("\nOpcao invalida! Escolha territorios entre 1 e %d.\n\n", MAX_TER);
                            continue; // Volta para o início do laço de ataque
                        }
                        if (atacante == defensor) {
                            printf("\nUm territorio nao pode atacar a si mesmo!\n\n");
                            continue;
                        }
                        // O índice do array é sempre o número - 1 (ex: território 1 está no índice 0)
                        int idxAtacante = atacante - 1;
                        int idxDefensor = defensor - 1;

                        if (biblioteca[idxAtacante].tropa < 2) {
                            printf("\nO territorio atacante precisa ter pelo menos 2 tropas para atacar!\n\n");
                            continue;
                        }
                        
                        // 2. ROLAGEM E RESULTADO DOS DADOS (lógica simplificada)
                        int dadoAtaque = rand() % 6 + 1; // Gera número de 1 a 6
                        int dadoDefesa = rand() % 6 + 1; // Gera número de 1 a 6

                        printf("\n- - - RESULTADO DOS DADOS - - -\n");
                        printf("O exercito %s (atacante) rolou o dado e tirou: %d\n", biblioteca[idxAtacante].cor, dadoAtaque);
                        printf("O exercito %s (defensor) rolou o dado e tirou: %d\n", biblioteca[idxDefensor].cor, dadoDefesa);
                        
                        // 3. RESULTADO DA BATALHA
                        printf("\n- - - RESULTADO DE BATALHA - - -\n");
                        if (dadoAtaque > dadoDefesa) {
                            printf("VITORIA DO ATACANTE! O defensor perdeu 1 tropa.\n\n");
                            biblioteca[idxDefensor].tropa--; // Diminui uma tropa do defensor
                        } else {
                            printf("VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n\n");
                            biblioteca[idxAtacante].tropa--; // Diminui uma tropa do atacante
                        }
                        
                        // Verifica se o território foi conquistado
                        if (biblioteca[idxDefensor].tropa <= 0) {
                            printf("!!! O territorio %s foi conquistado pelo exercito %s !!!\n", biblioteca[idxDefensor].nome, biblioteca[idxAtacante].cor);
                            // Muda a cor do território conquistado
                            strcpy(biblioteca[idxDefensor].cor, biblioteca[idxAtacante].cor);
                            // Move uma tropa para o novo território
                            biblioteca[idxDefensor].tropa = 1;
                            biblioteca[idxAtacante].tropa--;
                        }
                    } // Fim do laço de ataque
                }
                break;

            case 3: // CARREGAR JOGO
                mostrarSlots();
                printf("Escolha o slot para carregar: ");
                scanf("%d", &slotEscolhido);
                limparBufferEntrada();
                if (slotEscolhido != 0) {
                    int carregados = carregarJogo(slotEscolhido, biblioteca);
                    if (carregados > 0) {
                        totalTerritorios = carregados;
                    }
                }
                break;

            case 4: // SALVAR JOGO
                if (totalTerritorios == 0) {
                    printf("\nNao ha nada para salvar! Inicie ou carregue um jogo primeiro.\n");
                } else {
                    mostrarSlots();
                    printf("Escolha o slot para salvar: ");
                    scanf("%d", &slotEscolhido);
                    limparBufferEntrada();
                    if (slotEscolhido != 0) {
                        salvarJogo(slotEscolhido, biblioteca, totalTerritorios);
                    }
                }
                break;
            
            case 5: // RESETAR SAVE
                mostrarSlots();
                printf("Escolha o slot para resetar: ");
                scanf("%d", &slotEscolhido);
                limparBufferEntrada();
                if (slotEscolhido != 0) {
                    resetarSlot(slotEscolhido);
                }
                break;

            case 0: // SAIR
                printf("\nSaindo do Sistema...\n");
                break;

            default: // OPÇÃO INVÁLIDA
                printf("\nOpcao Invalida! Tente novamente.\n");
                break;
        }
        printf("\nPressione Enter para voltar ao menu...");
        getchar();

    } while (opcao != 0);

    return 0;
}

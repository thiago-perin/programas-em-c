// ##################
// WAR PARTE 4 - MISSÕES
// ##################

// ######################
// Includes
// ######################

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // biblioteca que tem strings
#include <time.h>   // Para números aleatórios

// ######################
// Definições globais
// ######################

#define TAM_STRING 20
#define MAX_TER 5
#define MAX_SAVES 5

// ######################
// Definição das Estruturas
// ######################

struct Territorio {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int isPlayer; // 1 para jogador, 0 para bot
    int tropa;
};

struct SaveState {
    int slotOcupado;
    char nomeSave[50];
    char dataHora[30];
    int totalTerritorios;
    struct Territorio territorios[MAX_TER];
};

// --- NOVAS ESTRUTURAS PARA AS MISSÕES ---
enum TipoMissao { ELIMINAR_COR, CONQUISTAR_TERRITORIOS };

struct Missao {
    enum TipoMissao tipo;
    int completa;

    union {
        char corAlvo[TAM_STRING];
        int territoriosAlvo[2];
    } alvo;
};

// ######################################
// Protótipos das Funções
// ######################################

void limparBufferEntrada();
void exibirMenu();
void novoJogo(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida, struct Missao missoes[]);
void entrarNoJogo(struct Territorio biblioteca[], int totalTerritorios, int ordemJogadores[], int ordemDefinida);
void gerenciarCarregamento(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida, struct Missao missoes[]);
void gerenciarSalvamento(struct Territorio biblioteca[], int totalTerritorios);
void gerenciarReset();
void getNomeArquivo(int slot, char* nomeArquivo);
void mostrarSlots();
void salvarJogo(int slot, struct Territorio biblioteca[], int total);
int carregarJogo(int slot, struct Territorio biblioteca[]);
void resetarSlot(int slot);
void embaralharOrdem(int *array, int n);
void turnoDoJogador(struct Territorio biblioteca[], int totalTerritorios);
void turnoDoBot(struct Territorio biblioteca[], int totalTerritorios, int indiceBot);
void gerarMissoes(struct Missao missoes[], const struct Territorio biblioteca[]);


// ######################################
// Função Principal (main)
// ######################################
int main() {
    srand(time(NULL));

    struct Territorio biblioteca[MAX_TER];
    struct Missao missoes[MAX_TER];
    int totalTerritorios = 0;
    int opcao;

    int ordemJogadores[MAX_TER];
    int ordemDefinida = 0; // 0 = não, 1 = sim

    do {
        exibirMenu();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                novoJogo(biblioteca, &totalTerritorios, ordemJogadores, &ordemDefinida, missoes);
                break;
            case 2:
                entrarNoJogo(biblioteca, totalTerritorios, ordemJogadores, ordemDefinida);
                break;
            case 3:
                gerenciarCarregamento(biblioteca, &totalTerritorios, ordemJogadores, &ordemDefinida, missoes);
                break;
            case 4:
                gerenciarSalvamento(biblioteca, totalTerritorios);
                break;
            case 5:
                gerenciarReset();
                break;
            case 0:
                printf("\nSaindo do Sistema...\n");
                break;
            default:
                printf("\nOpcao Invalida! Tente novamente.\n");
                break;
        }
        
        if (opcao != 0) {
            printf("\nPressione Enter para voltar ao menu...");
            getchar();
        }

    } while (opcao != 0);

    return 0;
}

// ######################################
// Implementação das Funções
// ######################################

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirMenu() {
    printf("\n\n#########################################\n");
    printf("        WAR - TERRITORIOS\n");
    printf("#########################################\n");
    printf("1 - Novo Jogo (Cadastrar Territorios)\n");
    printf("2 - Entrar no Jogo\n");
    printf("3 - Carregar Jogo (Load Game)\n");
    printf("4 - Salvar Jogo (Save Game)\n");
    printf("5 - Resetar um Save\n");
    printf("0 - Sair\n");
    printf("-----------------------------------------\n");
    printf("Escolha uma opcao: ");
}

void embaralharOrdem(int *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void gerarMissoes(struct Missao missoes[], const struct Territorio biblioteca[]) {
    char coresUnicas[MAX_TER][TAM_STRING];
    for (int i = 0; i < MAX_TER; i++) {
        strcpy(coresUnicas[i], biblioteca[i].cor);
    }
    
    for (int i = 0; i < MAX_TER; i++) {
        missoes[i].completa = 0;
        int tipoDaMissao = rand() % 2;

        if (tipoDaMissao == 0) { // Missão: ELIMINAR_COR
            missoes[i].tipo = ELIMINAR_COR;
            int indiceCorAlvo;
            do {
                indiceCorAlvo = rand() % MAX_TER;
            } while (strcmp(coresUnicas[indiceCorAlvo], biblioteca[i].cor) == 0);
            strcpy(missoes[i].alvo.corAlvo, coresUnicas[indiceCorAlvo]);
        } else { // Missão: CONQUISTAR_TERRITORIOS
            missoes[i].tipo = CONQUISTAR_TERRITORIOS;
            int t1, t2;
            do {
                t1 = rand() % MAX_TER;
                t2 = rand() % MAX_TER;
            } while (t1 == t2 || t1 == i || t2 == i);
            missoes[i].alvo.territoriosAlvo[0] = t1;
            missoes[i].alvo.territoriosAlvo[1] = t2;
        }
    }
}

void novoJogo(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida, struct Missao missoes[]) {
    printf("Iniciando novo jogo...\n");
    printf("Vamos cadastrar os 5 territorios iniciais do nosso mundo \n\n");
    
    for (int i = 0; i < MAX_TER; i++) {
        printf("- - - Cadastrando o %dº Territorio - - - \n", i + 1);
        printf("Nome do Territorio: ");
        fgets(biblioteca[i].nome, TAM_STRING, stdin);
        printf("Cor do Exercito (ex: Azul, verde, etc):");
        fgets(biblioteca[i].cor, TAM_STRING, stdin);
        printf("Numero de tropas: ");
        scanf("%d", &biblioteca[i].tropa);
        limparBufferEntrada(); 
        biblioteca[i].nome[strcspn(biblioteca[i].nome, "\n")] = '\0';
        biblioteca[i].cor[strcspn(biblioteca[i].cor, "\n")] = '\0';
        printf(" --> Territorio %d cadastrado com sucesso!\n\n", i + 1);
    }
    
    char corEscolhida[TAM_STRING];
    int corValida = 0;
    while (!corValida) {
        printf("\n--- Escolha seu Exercito ---\n");
        printf("Digite o nome da cor que voce deseja controlar (ex: Azul): ");
        fgets(corEscolhida, TAM_STRING, stdin);
        corEscolhida[strcspn(corEscolhida, "\n")] = '\0';
        for (int i = 0; i < MAX_TER; i++) {
            if (strcmp(biblioteca[i].cor, corEscolhida) == 0) {
                corValida = 1;
                break;
            }
        }
        if (!corValida) printf("\nCor invalida! Por favor, digite uma das cores que voce cadastrou.\n");
    }

    for (int i = 0; i < MAX_TER; i++) {
        if (strcmp(biblioteca[i].cor, corEscolhida) == 0) biblioteca[i].isPlayer = 1;
        else biblioteca[i].isPlayer = 0;
    }
    printf("\nVoce escolheu o exercito %s! Boa sorte.\n", corEscolhida);

    *totalTerritorios = MAX_TER;

    for(int i = 0; i < *totalTerritorios; i++) ordemJogadores[i] = i; 
    embaralharOrdem(ordemJogadores, *totalTerritorios);
    *ordemDefinida = 1;

    // Gera e mostra a missão
    gerarMissoes(missoes, biblioteca);
    for (int i = 0; i < MAX_TER; i++) {
        if (biblioteca[i].isPlayer) {
            printf("\n=-=-=-=-=- SUA MISSAO SECRETA -=-=-=-=-=\n");
            if (missoes[i].tipo == ELIMINAR_COR) {
                printf("Objetivo: Eliminar completamente o exercito %s do mapa.\n", missoes[i].alvo.corAlvo);
            } else {
                printf("Objetivo: Conquistar os territorios %s e %s.\n", 
                    biblioteca[missoes[i].alvo.territoriosAlvo[0]].nome, 
                    biblioteca[missoes[i].alvo.territoriosAlvo[1]].nome);
            }
            printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
            break;
        }
    }
    
    printf("Cadastro concluido! Voce ja pode jogar ou salvar.\n");
}

void entrarNoJogo(struct Territorio biblioteca[], int totalTerritorios, int ordemJogadores[], int ordemDefinida) {
    if (totalTerritorios == 0 || !ordemDefinida) {
        printf("\nInicie um Novo Jogo ou Carregue um save primeiro.\n");
        return;
    }

    printf("\n--- O Jogo Comecou! ---\n");
    
    while(1) {
        printf("\n--- A ordem de jogada e: ---\n");
        for(int i = 0; i < totalTerritorios; i++) {
            int jogadorIdx = ordemJogadores[i];
            printf("%dº a jogar: Exercito %s (%s)\n", i + 1, biblioteca[jogadorIdx].cor, biblioteca[jogadorIdx].isPlayer ? "Voce" : "Bot");
        }
        printf("----------------------------------------\n\n");

        for (int i = 0; i < totalTerritorios; i++) {
            int jogadorAtualIdx = ordemJogadores[i];
            if (biblioteca[jogadorAtualIdx].isPlayer) {
                turnoDoJogador(biblioteca, totalTerritorios);
            } else {
                turnoDoBot(biblioteca, totalTerritorios, jogadorAtualIdx);
            }
            printf("\nPressione Enter para o proximo turno...");
            getchar();
        }
        printf("\n--- Fim da Rodada! Iniciando proxima rodada... ---\n");
    }
}

void turnoDoJogador(struct Territorio biblioteca[], int totalTerritorios) {
    printf("\n--- SEU TURNO ---\n");
    int atacante, defensor;
    
    while(1) {
        printf("##########################################\n");
        printf("- - - MAPA DO MUNDO - ESTADO ATUAL - - -\n");
        for (int i = 0; i < totalTerritorios; i++) {
            printf("%d. %s (Cor: %s, Tropas: %d)\n", i + 1, biblioteca[i].nome, biblioteca[i].cor, biblioteca[i].tropa);
        }
        printf("---------------------------------------\n\n");

        printf("Escolha seu territorio ATACANTE (1 a %d) ou digite 0 para terminar o turno: ", MAX_TER);
        scanf("%d", &atacante);
        limparBufferEntrada();

        if (atacante == 0) break;

        printf("Escolha o territorio DEFENSOR (1 a %d): ", MAX_TER);
        scanf("%d", &defensor);
        limparBufferEntrada();
        
        int idxAtacante = atacante - 1;
        int idxDefensor = defensor - 1;

        if (atacante < 1 || atacante > MAX_TER || defensor < 1 || defensor > MAX_TER || atacante == defensor || !biblioteca[idxAtacante].isPlayer) {
            printf("\nAtaque invalido! Verifique se o territorio atacante e seu.\n");
            continue;
        }
        if (biblioteca[idxAtacante].tropa < 2) {
            printf("\nO territorio atacante precisa ter pelo menos 2 tropas!\n");
            continue;
        }
        if (biblioteca[idxDefensor].isPlayer) {
            printf("\nVoce nao pode atacar seus proprios territorios!\n");
            continue;
        }

        int dadoAtaque = rand() % 6 + 1;
        int dadoDefesa = rand() % 6 + 1;
        printf("\nVoce rolou %d no dado de ataque!\n", dadoAtaque);
        printf("O defensor %s rolou %d no dado de defesa!\n", biblioteca[idxDefensor].nome, dadoDefesa);

        if (dadoAtaque > dadoDefesa) {
            printf("VITORIA! O defensor perdeu 1 tropa.\n\n");
            biblioteca[idxDefensor].tropa--;
            if (biblioteca[idxDefensor].tropa <= 0) {
                printf("!!! O territorio %s foi conquistado por voce !!!\n", biblioteca[idxDefensor].nome);
                strcpy(biblioteca[idxDefensor].cor, biblioteca[idxAtacante].cor);
                biblioteca[idxDefensor].isPlayer = 1;
                biblioteca[idxDefensor].tropa = 1;
                biblioteca[idxAtacante].tropa--;
            }
        } else {
            printf("DERROTA! Voce perdeu 1 tropa.\n\n");
            biblioteca[idxAtacante].tropa--;
        }
    }
}

void turnoDoBot(struct Territorio biblioteca[], int totalTerritorios, int indiceBot) {
    printf("\n--- TURNO DO EXERCITO %s (Bot) ---\n", biblioteca[indiceBot].cor);
    
    int idxAtacante = -1;
    for (int i = 0; i < totalTerritorios; i++) {
        if (!biblioteca[i].isPlayer && strcmp(biblioteca[i].cor, biblioteca[indiceBot].cor) == 0 && biblioteca[i].tropa > 1) {
            idxAtacante = i;
            break;
        }
    }

    if (idxAtacante == -1) {
        printf("O exercito %s nao tem territorios para atacar. Passando o turno.\n", biblioteca[indiceBot].cor);
        return;
    }

    int idxDefensor = -1;
    int alvosPossiveis[MAX_TER];
    int numAlvos = 0;
    for(int i = 0; i < totalTerritorios; i++){
        if(strcmp(biblioteca[i].cor, biblioteca[idxAtacante].cor) != 0){
            alvosPossiveis[numAlvos++] = i;
        }
    }

    if (numAlvos == 0) {
         printf("O exercito %s nao tem alvos para atacar. Passando o turno.\n", biblioteca[indiceBot].cor);
         return;
    }
    
    idxDefensor = alvosPossiveis[rand() % numAlvos];

    printf("O exercito %s ataca %s!\n", biblioteca[idxAtacante].nome, biblioteca[idxDefensor].nome);

    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    if (dadoAtaque > dadoDefesa) {
        printf("VITORIA DO BOT! O defensor perdeu 1 tropa.\n\n");
        biblioteca[idxDefensor].tropa--;
        if (biblioteca[idxDefensor].tropa <= 0) {
            printf("!!! O territorio %s foi conquistado pelo exercito %s !!!\n", biblioteca[idxDefensor].nome, biblioteca[idxAtacante].cor);
            strcpy(biblioteca[idxDefensor].cor, biblioteca[idxAtacante].cor);
            biblioteca[idxDefensor].isPlayer = 0;
            biblioteca[idxDefensor].tropa = 1;
            biblioteca[idxAtacante].tropa--;
        }
    } else {
        printf("DERROTA DO BOT! O atacante perdeu 1 tropa.\n\n");
        biblioteca[idxAtacante].tropa--;
    }
}


void gerenciarCarregamento(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida, struct Missao missoes[]) {
    int slotEscolhido;
    mostrarSlots();
    printf("Escolha o slot para carregar: ");
    scanf("%d", &slotEscolhido);
    limparBufferEntrada();
    if (slotEscolhido != 0) {
        int carregados = carregarJogo(slotEscolhido, biblioteca);
        if (carregados > 0) {
            *totalTerritorios = carregados;
            
            // Lógica de escolha de jogador após carregar
            char corEscolhida[TAM_STRING];
            int corValida = 0;
            while(!corValida){
                printf("\nJogo Carregado. Qual cor voce estava jogando? ");
                fgets(corEscolhida, TAM_STRING, stdin);
                corEscolhida[strcspn(corEscolhida, "\n")] = '\0';
                for (int i = 0; i < *totalTerritorios; i++) {
                    if (strcmp(biblioteca[i].cor, corEscolhida) == 0) {
                        corValida = 1;
                        break;
                    }
                }
                if (!corValida) printf("Cor invalida!\n");
            }
            for (int i = 0; i < *totalTerritorios; i++) {
                if (strcmp(biblioteca[i].cor, corEscolhida) == 0) biblioteca[i].isPlayer = 1;
                else biblioteca[i].isPlayer = 0;
            }

            // Define a ordem após carregar com sucesso
            for(int i = 0; i < *totalTerritorios; i++) ordemJogadores[i] = i; 
            embaralharOrdem(ordemJogadores, *totalTerritorios);
            *ordemDefinida = 1; 

            // Gera e mostra a missão
            gerarMissoes(missoes, biblioteca);
            for (int i = 0; i < MAX_TER; i++) {
                if (biblioteca[i].isPlayer) {
                    printf("\n=-=-=-=-=- SUA MISSAO SECRETA -=-=-=-=-=\n");
                    if (missoes[i].tipo == ELIMINAR_COR) {
                        printf("Objetivo: Eliminar completamente o exercito %s do mapa.\n", missoes[i].alvo.corAlvo);
                    } else {
                        printf("Objetivo: Conquistar os territorios %s e %s.\n", 
                            biblioteca[missoes[i].alvo.territoriosAlvo[0]].nome, 
                            biblioteca[missoes[i].alvo.territoriosAlvo[1]].nome);
                    }
                    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                    break;
                }
            }
        }
    }
}

void gerenciarSalvamento(struct Territorio biblioteca[], int totalTerritorios) {
    if (totalTerritorios == 0) {
        printf("\nNao ha nada para salvar! Inicie ou carregue um jogo primeiro.\n");
    } else {
        int slotEscolhido;
        mostrarSlots();
        printf("Escolha o slot para salvar: ");
        scanf("%d", &slotEscolhido);
        limparBufferEntrada();
        if (slotEscolhido != 0) {
            salvarJogo(slotEscolhido, biblioteca, totalTerritorios);
        }
    }
}

void gerenciarReset() {
    int slotEscolhido;
    mostrarSlots();
    printf("Escolha o slot para resetar: ");
    scanf("%d", &slotEscolhido);
    limparBufferEntrada();
    if (slotEscolhido != 0) {
        resetarSlot(slotEscolhido);
    }
}

// Funções de arquivo (Save/Load)

void getNomeArquivo(int slot, char* nomeArquivo) {
    sprintf(nomeArquivo, "save_%d.dat", slot);
}

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

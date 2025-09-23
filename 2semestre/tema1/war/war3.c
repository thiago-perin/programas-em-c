// ##################
// WAR PARTE 3
// ##################

// ######################
// Includes
// ######################

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // biblioteca que tem strings
#include <time.h> // Para que os dados sejam diferentes a cada vez que você joga, precisamos de números aleatórios.

// ######################
// Definições globais
// ######################

#define TAM_STRING 20
#define TAM_INT 3
#define MAX_TER 5
#define MAX_SAVES 5 // Ela conterá os territórios e as informações extras.

// ######################
// Definição da estrutura
// ######################
enum TipoMissao { ELIMINAR_COR, CONQUISTAR_TERRITORIOS };

struct Missao {
    enum TipoMissao tipo;
    int completa; // 0 = não, 1 = sim

    union {
        char corAlvo[TAM_STRING];
        int territoriosAlvo[2];
    } alvo;
};

struct Territorio {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int isPlayer;
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
// Protótipos de funções
// ######################################

void limparBufferEntrada();
void exibirMenu();
void novoJogo(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida);
void entrarNoJogo(struct Territorio biblioteca[], int totalTerritorios, int ordemJogadores[], int ordemDefinida);
void gerenciarCarregamento(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida);
void gerenciarSalvamento(struct Territorio biblioteca[], int totalTerritorios);
void gerenciarReset();
void getNomeArquivo(int slot, char* nomeArquivo);
void mostrarSlots();
void salvarJogo(int slot, struct Territorio biblioteca[], int total);
int carregarJogo(int slot, struct Territorio biblioteca[]);
void resetarSlot(int slot);
void turnoDoJogador(struct Territorio biblioteca[], int totalTerritorios);
void turnoDoBot(struct Territorio biblioteca[], int totalTerritorios, int indiceBot);

// ######################################
// Função Principal (main)
// ######################################
int main() {
    srand(time(NULL));

    struct Territorio biblioteca[MAX_TER];
    int totalTerritorios = 0; // Começa sempre com 0, pois o jogador escolhe o que carregar
    int opcao;

    int ordemJogadores[MAX_TER];
    int ordemDefinida = 0; // 0 = não, 1 = sim

    do {
        exibirMenu();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1: // NOVO JOGO
                novoJogo(biblioteca, &totalTerritorios, ordemJogadores, &ordemDefinida);
                break;

            case 2: // ENTRAR NO JOGO
                entrarNoJogo(biblioteca, totalTerritorios, ordemJogadores, ordemDefinida);
                break;

            case 3: // CARREGAR JOGO
                gerenciarCarregamento(biblioteca, &totalTerritorios, ordemJogadores, &ordemDefinida);
                break;

            case 4: // SALVAR JOGO
                gerenciarSalvamento(biblioteca, totalTerritorios);
                break;
    
            case 5: // RESETAR SAVE
                gerenciarReset();
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

void limparBufferEntrada() {
    int c;
    while (
        (c = getchar()
        ) != '\n' && c != EOF);
}

void exibirMenu() {
    printf("#########################################\n");
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

void novoJogo(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida) {
    printf("Iniciando novo jogo...\n");
    printf("Vamos cadastrar os 5 territorios iniciais do nosso mundo \n\n");
    
    for (int i = 0; i < MAX_TER; i++) {
        printf("- - - Cadastrando o %dº Territorio - - - \n", i + 1);
        // ... (cadastro dos territórios) ...
        
        printf("Nome do Territorio: ");
        fgets (biblioteca[i].nome, TAM_STRING, stdin);
        
        printf("Cor do Exercito (ex: Azul, verde, etc):");
        fgets (biblioteca[i].cor, TAM_STRING, stdin);
        
        printf("Numero de tropas: ");
        scanf("%d", &biblioteca[i].tropa);
        
        limparBufferEntrada(); 
        biblioteca[i].nome[strcspn(biblioteca[i].nome, "\n")] = '\0';
        biblioteca[i].cor[strcspn(biblioteca[i].cor, "\n")] = '\0';
        printf(" --> Territorio %d cadastrado com sucesso!\n\n", i + 1);
    }
    
    // ... (escolha da cor do jogador) ...
    char corEscolhida[TAM_STRING];
    int corValida = 0;
    
    // Loop para garantir que o jogador escolha uma cor que realmente existe no jogo
    while (!corValida) {
        printf("\n--- Escolha seu Exercito ---\n");
        printf("Digite o nome da cor que voce deseja controlar (ex: Azul): ");
        fgets(corEscolhida, TAM_STRING, stdin);
        corEscolhida[strcspn(corEscolhida, "\n")] = '\0';

        // Verifica se a cor digitada existe
        for (int i = 0; i < MAX_TER; i++) {
            if (strcmp(biblioteca[i].cor, corEscolhida) == 0) {
                corValida = 1; // Encontrou a cor, pode sair do loop while
                break;
            }
        }
        if (!corValida) {
            printf("\nCor invalida! Por favor, digite uma das cores que voce cadastrou.\n");
        }
    }

    // Define quem é o jogador e quem é o bot
    for (int i = 0; i < MAX_TER; i++) {
        if (strcmp(biblioteca[i].cor, corEscolhida) == 0) {
            biblioteca[i].isPlayer = 1;  // Território do jogador
        } else {
            biblioteca[i].isPlayer = 0;  // Território do bot
        }
    }
    printf("\nVoce escolheu o exercito %s! Boa sorte.\n", corEscolhida);

    *totalTerritorios = MAX_TER;

    // CRIAMOS E EMBARALHAMOS A ORDEM AQUI!
    for(int i = 0; i < *totalTerritorios; i++) {
        ordemJogadores[i] = i; 
    }
    embaralharOrdem(ordemJogadores, *totalTerritorios);
    *ordemDefinida = 1; // Avisa ao main que a ordem foi criada

    printf("Cadastro concluido! Voce ja pode jogar ou salvar.\n");
}

void entrarNoJogo(struct Territorio biblioteca[], int totalTerritorios, int ordemJogadores[], int ordemDefinida) {
    if (totalTerritorios == 0 || !ordemDefinida) {
        printf("\nInicie um Novo Jogo ou Carregue um save primeiro.\n");
        return;
    }

    printf("\n--- O Jogo Comecou! ---\n");
    
    while(1) { // Loop principal do jogo (cada iteração é uma rodada completa)
        
        // Passa por cada jogador na ordem sorteada
        for (int i = 0; i < totalTerritorios; i++) {
            int jogadorAtualIdx = ordemJogadores[i];

            if (biblioteca[jogadorAtualIdx].isPlayer) {
                turnoDoJogador(biblioteca, totalTerritorios);
            } else {
                turnoDoBot(biblioteca, totalTerritorios, jogadorAtualIdx);
            }

            // Pausa para o jogador ler o que aconteceu
            printf("\nPressione Enter para o proximo turno...");
            getchar();
        }

        // Aqui poderia entrar uma verificação de vitória no final de cada rodada
        printf("\n--- Fim da Rodada! Iniciando proxima rodada... ---\n");
    }
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

void gerenciarCarregamento(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida) {
    int slotEscolhido;
    mostrarSlots();
    printf("Escolha o slot para carregar: ");
    scanf("%d", &slotEscolhido);
    limparBufferEntrada();
    if (slotEscolhido != 0) {
        int carregados = carregarJogo(slotEscolhido, biblioteca);
        if (carregados > 0) {
            *totalTerritorios = carregados;

            // Define a ordem após carregar com sucesso
            for(int i = 0; i < *totalTerritorios; i++) {
                ordemJogadores[i] = i; 
            }
            embaralharOrdem(ordemJogadores, *totalTerritorios);
            *ordemDefinida = 1; 
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

void turnoDoJogador(struct Territorio biblioteca[], int totalTerritorios) {
    printf("\n--- SEU TURNO ---\n");
    // Lógica de ataque para o jogador humano
    int atacante, defensor;
    
    while(1) { // Loop para permitir vários ataques no mesmo turno
        printf("##########################################\n");
        printf("- - - MAPA DO MUNDO - ESTADO ATUAL - - -\n");
        for (int i = 0; i < totalTerritorios; i++) {
            printf("%d. %s (Cor: %s, Tropas: %d)\n", i + 1, biblioteca[i].nome, biblioteca[i].cor, biblioteca[i].tropa);
        }
        printf("---------------------------------------\n\n");

        printf("Escolha seu territorio ATACANTE (1 a %d) ou digite 0 para terminar o turno: ", MAX_TER);
        scanf("%d", &atacante);
        limparBufferEntrada();

        if (atacante == 0) break; // Termina o turno

        printf("Escolha o territorio DEFENSOR (1 a %d): ", MAX_TER);
        scanf("%d", &defensor);
        limparBufferEntrada();
        
        int idxAtacante = atacante - 1;
        int idxDefensor = defensor - 1;

        // Validações (ataque válido, etc.)
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

        // Lógica da batalha (dados, etc.)
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
    
    // IA Simples: Encontra um território para atacar
    int idxAtacante = -1;
    for (int i = 0; i < totalTerritorios; i++) {
        if (!biblioteca[i].isPlayer && strcmp(biblioteca[i].cor, biblioteca[indiceBot].cor) == 0 && biblioteca[i].tropa > 1) {
            idxAtacante = i; // Encontra um território do bot que pode atacar
            break;
        }
    }

    if (idxAtacante == -1) {
        printf("O exercito %s nao tem territorios para atacar. Passando o turno.\n", biblioteca[indiceBot].cor);
        return; // Bot não pode atacar
    }

    // IA Simples: Encontra um alvo para o ataque (qualquer um que não seja dele)
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
         return; // Não há inimigos
    }
    
    idxDefensor = alvosPossiveis[rand() % numAlvos]; // Escolhe um alvo aleatório

    printf("O exercito %s ataca %s!\n", biblioteca[idxAtacante].nome, biblioteca[idxDefensor].nome);

    // Lógica da batalha
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

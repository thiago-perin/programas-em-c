#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // Necessário para a função time(), usada para gerar números aleatórios
#include <string.h> // Necessário para funções de manipulação de strings, como strcpy() e strcmp()

// ######################################################
// DEFINIÇÕES GLOBAIS
// ######################################################
#define MAX 5           // Define o tamanho máximo da fila de peças. O jogo tentará manter 5 peças na fila.
#define MAX_SAVES 5     // Define o número máximo de slots para salvar o jogo.

// ######################################################
// ESTRUTURAS DE DADOS
// ######################################################

// Define a estrutura de uma única peça do Tetris.
typedef struct {
    char nome[30];  // Nome da peça para exibição (ex: "Barra Reta").
    int tipo;       // Um número de 0 a 3 que identifica o tipo da peça.
    int ID;         // Um identificador único para cada peça gerada no jogo.
} Pecas;

// Define a estrutura da Fila, que é uma fila circular baseada em array.
typedef struct {
    Pecas itens[MAX]; // O array que armazena as peças. Seu tamanho é definido por MAX.
    int inicio;       // Índice que aponta para o início da fila.
    int fim;          // Índice que aponta para a próxima posição livre no final da fila.
    int total;        // A quantidade atual de elementos na fila.
} Fila;

// Define a estrutura para salvar o estado do jogo em um arquivo.
typedef struct {
    int slotOcupado;      // Flag (0 ou 1) para indicar se o slot de save está em uso.
    char nomeSave[50];    // Nome que o jogador deu ao save.
    char dataHora[30];    // Data e hora em que o jogo foi salvo.
    Fila filaDePecas;     // A cópia da fila de peças no momento do save.
} SaveState;

// Contador global para garantir que cada peça tenha um ID único.
// Ele é incrementado toda vez que uma nova peça é criada.
int proximoIdPeca = 1;

// ######################################################
// PROTÓTIPOS DAS FUNÇÕES
// ######################################################

// --- Funções de Menu ---
void exibirMenuPrincipal();
void exibirMenuSecundario();
void entrarNoJogo(Fila *f);
void limparBufferEntrada();
void mostrarCreditos();

// --- Funções da Fila ---
void inicializarFila(Fila *f);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
void inserir(Fila *f, Pecas p);
void remover(Fila *f, Pecas *p);
void mostrarFila(Fila *f);
void manterFilaDePecas(Fila *f);
int contarPecasDoTipo(Fila *f, int tipo);

// --- Funções de Jogo ---
void gerarPecaAleatoriaEInserir(Fila *f);

// --- Funções de Arquivo (Save/Load/Reset) ---
void gerenciarCarregamento(int *jogoIniciadoFlag, Fila *f);
void gerenciarSalvamento(int jogoIniciadoFlag, Fila *f);
void gerenciarReset();
void getNomeArquivo(int slot, char* nomeArquivo);
void mostrarSlots();
void salvarJogo(int slot, Fila *f);
int carregarJogo(int slot, Fila *f);
void resetarSlot(int slot);


// ######################################################
// FUNÇÃO PRINCIPAL (main)
// ######################################################
int main() {
    // Inicializa o gerador de números aleatórios com a hora atual como semente.
    // Isso garante que a sequência de peças seja diferente a cada execução do jogo.
    srand(time(NULL));

    int opcao;          // Variável para armazenar a escolha do usuário no menu.
    int jogoIniciado = 0; // Flag para controlar se um jogo está ativo (1) ou não (0).
    Fila filaDePecas;   // Declara a variável principal da fila de peças para o jogo.

    // Loop principal do jogo, continua executando até que o usuário escolha a opção 0 para sair.
    do {
        exibirMenuPrincipal();  // Mostra o menu principal na tela.
        scanf("%d", &opcao);    // Lê a opção digitada pelo usuário.
        limparBufferEntrada();  // Limpa o buffer do teclado para evitar leituras indesejadas.

        // Estrutura switch para tratar a opção escolhida pelo usuário.
        switch (opcao) {
            case 1: // Opção: Novo Jogo
                printf("\nIniciando novo jogo de Tetris!\n");
                proximoIdPeca = 1; // Reseta o contador de ID para um novo jogo.
                inicializarFila(&filaDePecas); // Prepara a fila para um novo jogo (zera contadores).
                manterFilaDePecas(&filaDePecas); // Garante que a fila seja preenchida com as 5 peças iniciais.
                jogoIniciado = 1; // Marca que um jogo foi iniciado.
                entrarNoJogo(&filaDePecas); // Leva o jogador diretamente para o menu secundário do jogo.
                break;
            case 2: // Opção: Entrar no Jogo
                if (jogoIniciado) { // Só permite entrar se um jogo já foi iniciado ou carregado.
                    entrarNoJogo(&filaDePecas);
                } else {
                    printf("\nVoce precisa iniciar um 'Novo Jogo' ou 'Carregar um Jogo salvo' primeiro!\n");
                }
                break;
            case 3: // Opção: Carregar Jogo
                gerenciarCarregamento(&jogoIniciado, &filaDePecas);
                break;
            case 4: // Opção: Salvar Jogo
                gerenciarSalvamento(jogoIniciado, &filaDePecas);
                break;
            case 5: // Opção: Resetar um Save
                gerenciarReset();
                break;
            case 6: // Opção: Créditos
                mostrarCreditos();
                break;
            case 0: // Opção: Sair
                printf("\nSaindo do Tetris...\n");
                break;
            default: // Caso o usuário digite uma opção inválida.
                printf("\nOpcao Invalida! Tente novamente.\n");
        }

        // Pausa a execução para que o usuário possa ler as mensagens antes de voltar ao menu.
        // Não pausa se o usuário saiu (0) ou iniciou um novo jogo (1), pois já entra em outro menu.
        if (opcao != 0 && opcao != 1) {
             printf("\nPressione Enter para voltar ao menu principal...");
             getchar();
        }

    } while (opcao != 0); // O loop continua enquanto a opção for diferente de 0.

    return 0; // Termina o programa.
}


// ############################################################################################
// - - - Implementação das funções de Menu e Gerenciamento - - -
// ############################################################################################

// Função para limpar o buffer de entrada do teclado (stdin).
// Evita que caracteres extras (como o "Enter") atrapalhem as próximas leituras.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Imprime o menu principal do jogo na tela.
void exibirMenuPrincipal() {
    printf("\n\n#########################################\n");
    printf("              TETRIS - MENU\n");
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

// Imprime o menu secundário (quando o jogador está "em jogo").
void exibirMenuSecundario() {
    printf("\n\n#########################################\n");
    printf("           --- EM JOGO ---\n");
    printf("#########################################\n\n");
    printf("1. Continuar Jogo (nao implementado)\n");
    printf("2. Ver fila de proximas pecas\n");
    printf("3. Usar proxima peca da fila\n");
    printf("4. Gerar nova peca para o final da fila\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("--------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

// Controla o loop do menu secundário, onde as ações do jogo acontecem.
void entrarNoJogo(Fila *f) {
    int opcao;
    Pecas pecaRemovida; // Variável para armazenar a peça que for removida da fila.

    do { // Loop do menu secundário
        exibirMenuSecundario();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1: // Opção: Continuar Jogo
                printf("\nFuncionalidade 'Continuar Jogo' ainda nao implementada.\n");
                break;
            case 2: // Opção: Ver Fila
                mostrarFila(f);
                break;
            case 3: // Opção: Usar Próxima Peça
                remover(f, &pecaRemovida); // Remove a peça do início da fila.
                if (pecaRemovida.ID != 0) { // Se o ID for 0, significa que a fila estava vazia.
                    printf("\nPeca [Nome: %s, ID: %d] entrou no jogo!\n", pecaRemovida.nome, pecaRemovida.ID);
                }
                mostrarFila(f); // Mostra a fila atualizada.
                manterFilaDePecas(f); // Chama a função para preencher a fila até 5 peças novamente.
                break;
            case 4: // Opção: Gerar Nova Peça Manualmente
                if (filaCheia(f)) { // Verifica se a fila já está no máximo (5 peças).
                    printf("\nA fila ja esta com 5 pecas. Nao e possivel adicionar mais.\n");
                } else {
                    gerarPecaAleatoriaEInserir(f);
                    printf("\nUma nova peca foi adicionada manualmente.\n");
                }
                mostrarFila(f);
                break;
            case 0: // Opção: Voltar ao Menu Principal
                printf("\nVoltando ao menu principal...\n");
                break;
            default:
                printf("\nOpcao Invalida!\n");
        }

        if (opcao != 0) { // Pausa para o usuário ler a saída.
             printf("\nPressione Enter para continuar...");
             getchar();
        }

    } while (opcao != 0);
}

// ############################################################################################
// - - - Implementação das funções de Jogo - - -
// ############################################################################################

// Gera e insere uma peça aleatória, respeitando a regra de no máximo 2 por tipo, de forma garantida, se possível.
void gerarPecaAleatoriaEInserir(Fila *f) {
    // Modelos de peças disponíveis.
    Pecas pecasDisponiveis[4] = {
        {"Formato I", 0, 0}, {"Formato O", 1, 0}, {"Formato T", 2, 0}, {"Formato L", 3, 0}
    };

    if (filaCheia(f)) {
        return; // Se a fila está cheia, não faz nada.
    }

    // 1. Cria uma lista de tipos de peças que são permitidos na fila.
    int tiposValidos[4];
    int nValidos = 0; // Contador de quantos tipos são válidos.
    for (int i = 0; i < 4; i++) {
        // Se o tipo atual aparece menos de 2 vezes na fila, ele é válido.
        if (contarPecasDoTipo(f, pecasDisponiveis[i].tipo) < 2) {
            tiposValidos[nValidos] = pecasDisponiveis[i].tipo;
            nValidos++;
        }
    }

    // 2. Se não há nenhum tipo válido que possa ser inserido, a função termina.
    if (nValidos == 0) {
        return;
    }

    // 3. Sorteia um índice aleatório da lista de tipos válidos.
    int indiceSorteado = rand() % nValidos;
    int tipoEscolhido = tiposValidos[indiceSorteado];

    // 4. Encontra o modelo da peça correspondente ao tipo que foi sorteado.
    Pecas novaPeca;
    for (int i = 0; i < 4; i++) {
        if (pecasDisponiveis[i].tipo == tipoEscolhido) {
            novaPeca = pecasDisponiveis[i];
            break;
        }
    }

    // 5. Atribui um ID exclusivo e insere a peça na fila.
    novaPeca.ID = proximoIdPeca;
    proximoIdPeca++;
    inserir(f, novaPeca);
}

// Conta quantas peças de um tipo específico existem na fila.
int contarPecasDoTipo(Fila *f, int tipo) {
    int contagem = 0;
    if (filaVazia(f)) {
        return 0;
    }
    // Itera por todos os elementos válidos na fila circular.
    // Começa em 'f->inicio' e dá a volta no array se necessário.
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
        if (f->itens[idx].tipo == tipo) {
            contagem++;
        }
    }
    return contagem;
}

// Garante que a fila sempre tenha 5 peças, se possível.
void manterFilaDePecas(Fila *f) {
    // Enquanto o total de peças for menor que 5 E a fila não estiver cheia (capacidade máxima)...
    while (f->total < 5 && !filaCheia(f)) {
        gerarPecaAleatoriaEInserir(f); // Gera uma nova peça silenciosamente.
    }
}

// ############################################################################################
// - - - Implementação das funções de Fila - - -
// ############################################################################################

// Prepara a fila para ser usada, zerando seus valores iniciais.
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Retorna 1 (verdadeiro) se a fila estiver cheia, 0 (falso) caso contrário.
int filaCheia(Fila *f) {
    return f->total == MAX;
}

// Retorna 1 (verdadeiro) se a fila estiver vazia, 0 (falso) caso contrário.
int filaVazia(Fila *f) {
    return f->total == 0;
}

// Adiciona uma peça ao final da fila.
void inserir(Fila *f, Pecas p) {
    if (filaCheia(f)) { // Proteção extra para não inserir em uma fila cheia.
        return;
    }
    f->itens[f->fim] = p; // Adiciona a peça na posição 'fim'.
    f->fim = (f->fim + 1) % MAX; // Move o 'fim' para a próxima posição, dando a volta no array se necessário.
    f->total++; // Incrementa o total de itens.
}

// Remove uma peça do início da fila.
void remover(Fila *f, Pecas *p) {
    if (filaVazia(f)) { // Se a fila estiver vazia, não há o que remover.
        printf("\nFila vazia. Nao ha pecas para remover.\n");
        strcpy(p->nome, "Nenhuma"); // Preenche a variável de retorno com dados nulos.
        p->tipo = -1;
        p->ID = 0;
        return;
    }
    *p = f->itens[f->inicio]; // Copia a peça do início da fila para a variável de retorno.
    f->inicio = (f->inicio + 1) % MAX; // Move o 'inicio' para a próxima posição, dando a volta se necessário.
    f->total--; // Decrementa o total de itens.
}

// Exibe o conteúdo da fila em formato de tabela.
void mostrarFila(Fila *f) {
    printf("\n\n--- FILA DE PROXIMAS PECAS ---\n");
    printf("----------------------------------\n");
    printf("| Posicao | %-12s | ID  |\n", "Nome");
    printf("----------------------------------\n");

    if (filaVazia(f)) {
        printf("| Fila Vazia                     |\n");
    } else {
        int posicao = 1;
        // Loop que percorre os elementos da fila circular.
        for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
            // Imprime cada peça formatada em uma linha da tabela.
            printf("| %-7d | %-12s | %-3d |\n", posicao, f->itens[idx].nome, f->itens[idx].ID);
            posicao++;
        }
    }
    printf("----------------------------------\n");
}


// ############################################################################################
// - - - FUNÇÕES DE ARQUIVO (SAVE/LOAD/RESET) - - -
// ############################################################################################

// Gerencia o processo de carregar um jogo.
void gerenciarCarregamento(int *jogoIniciadoFlag, Fila *f) {
    mostrarSlots(); // Mostra os saves disponíveis.
    printf("Escolha o slot para carregar (0 para cancelar): ");
    int slot;
    scanf("%d", &slot);
    limparBufferEntrada();
    if (slot > 0 && slot <= MAX_SAVES) {
        if (carregarJogo(slot, f)) { // Tenta carregar o jogo.
            *jogoIniciadoFlag = 1; // Se conseguir, marca que um jogo está ativo.
        }
    } else {
        printf("Carregamento cancelado.\n");
    }
}

// Gerencia o processo de salvar um jogo.
void gerenciarSalvamento(int jogoIniciadoFlag, Fila *f) {
    if (!jogoIniciadoFlag) { // Só permite salvar se um jogo estiver em andamento.
        printf("\nVoce precisa iniciar ou carregar um jogo para poder salvar!\n");
        return;
    }
    mostrarSlots();
    printf("Escolha o slot para salvar (0 para cancelar): ");
    int slot;
    scanf("%d", &slot);
    limparBufferEntrada();
    if (slot > 0 && slot <= MAX_SAVES) {
        salvarJogo(slot, f);
    } else {
        printf("Salvamento cancelado.\n");
    }
}

// Gerencia o processo de deletar um arquivo de save.
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

// Cria o nome do arquivo de save com base no número do slot (ex: "save_tetris_1.dat").
void getNomeArquivo(int slot, char* nomeArquivo) {
    sprintf(nomeArquivo, "save_tetris_%d.dat", slot);
}

// Lê os arquivos de save e exibe o status de cada slot (Vazio ou com informações do save).
void mostrarSlots() {
    printf("\n--- SLOTS DE JOGO ---\n");
    for (int i = 1; i <= MAX_SAVES; i++) {
        char nomeArquivo[20];
        getNomeArquivo(i, nomeArquivo);
        FILE* arquivo = fopen(nomeArquivo, "rb"); // Abre o arquivo para leitura binária.
        if (arquivo == NULL) { // Se o arquivo não existe, o slot está vazio.
            printf("%d. [ Vazio ]\n", i);
        } else { // Se o arquivo existe, lê as informações e as exibe.
            SaveState save;
            fread(&save, sizeof(SaveState), 1, arquivo);
            fclose(arquivo);
            printf("%d. %s (%s)\n", i, save.nomeSave, save.dataHora);
        }
    }
    printf("-----------------------\n");
}

// Salva o estado atual do jogo em um arquivo binário.
void salvarJogo(int slot, Fila *f) {
    SaveState save;
    save.slotOcupado = 1;
    save.filaDePecas = *f; // Copia a fila atual do jogo para a estrutura de save.

    printf("Digite um nome para este save (ou '0' para cancelar): ");
    fgets(save.nomeSave, 50, stdin);
    save.nomeSave[strcspn(save.nomeSave, "\n")] = '\0'; // Remove o '\n' do final.

    if (strcmp(save.nomeSave, "0") == 0) {
        printf("Salvamento cancelado pelo usuario.\n");
        return;
    }

    time_t t = time(NULL); // Pega a hora atual do sistema.
    strftime(save.dataHora, 30, "%d/%m/%Y %H:%M", localtime(&t)); // Formata a data e hora.

    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo, "wb"); // Abre o arquivo para escrita binária.
    if (arquivo) {
        fwrite(&save, sizeof(SaveState), 1, arquivo); // Escreve a estrutura inteira no arquivo.
        fclose(arquivo);
        printf("\nJogo salvo com sucesso no Slot %d!\n", slot);
    } else {
        printf("\nErro ao abrir o arquivo para salvar!\n");
    }
}

// Carrega um jogo salvo a partir de um arquivo binário.
int carregarJogo(int slot, Fila *f) {
    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo, "rb"); // Abre para leitura binária.
    if (arquivo == NULL) {
        printf("\nSlot vazio ou nao encontrado! Nao foi possivel carregar.\n");
        return 0; // Retorna 0 indicando falha.
    }

    SaveState save;
    fread(&save, sizeof(SaveState), 1, arquivo); // Lê a estrutura do arquivo.
    fclose(arquivo);

    *f = save.filaDePecas; // Restaura a fila do jogo com os dados do save.

    // Garante que o próximo ID de peça continue de onde parou no jogo salvo.
    proximoIdPeca = 1; // Reseta o contador.
    if (f->total > 0) {
        int maiorId = 0;
        // Encontra o maior ID na fila carregada.
        for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
            if (f->itens[idx].ID > maiorId) {
                maiorId = f->itens[idx].ID;
            }
        }
        proximoIdPeca = maiorId + 1; // Define o próximo ID como o maior + 1.
    }

    manterFilaDePecas(f); // Garante que a fila carregada tenha 5 peças.
    printf("\nJogo '%s' carregado com sucesso do Slot %d!\n", save.nomeSave, slot);
    return 1; // Retorna 1 indicando sucesso.
}

// Deleta um arquivo de save.
void resetarSlot(int slot) {
    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    if (remove(nomeArquivo) == 0) { // Tenta remover o arquivo.
        printf("\nSlot %d resetado com sucesso!\n", slot);
    } else {
        printf("\nSlot %d ja estava vazio ou nao pode ser removido.\n", slot);
    }
}

// Mostra os créditos do jogo.
void mostrarCreditos() {
    printf("\n\n#########################################\n");
    printf("               TETRIS\n");
    printf("#########################################\n");
    printf("Jogo desenvolvido por: Thiago Marcel\n");
    printf("Disciplina: Estrutura de Dados\n");
    printf("Professor: Nathan de Freitas\n");
    printf("#########################################\n");
}

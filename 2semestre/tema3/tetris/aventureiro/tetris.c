#include <stdio.h>  // Padrão de entrada e saída, para funções como printf() e scanf().
#include <stdlib.h> // Funções gerais, como alocação de memória e rand() para números aleatórios.
#include <time.h>   // Necessário para a função time(), usada para gerar uma semente para o rand().
#include <string.h> // Necessário para funções de manipulação de strings, como strcpy() e strcmp().

// ######################################################
// DEFINIÇÕES GLOBAIS
// ######################################################
#define MAX 5       // Define o tamanho máximo da fila de peças e da pilha de reserva.
#define MAX_SAVES 5 // Define o número máximo de slots para salvar o jogo.

// ######################################################
// ESTRUTURAS DE DADOS
// ######################################################

// Define a estrutura de uma única peça do Tetris.
typedef struct {
    char nome[30];  // Nome da peça para exibição (ex: "Formato I").
    int tipo;       // Um número que identifica o tipo da peça (ex: 0 para 'I', 1 para 'O').
    int ID;         // Um identificador único para cada peça gerada no jogo.
} Pecas;

// Define a estrutura da Fila, que é uma fila circular baseada em array.
// Usada para gerenciar as próximas peças que entrarão no jogo.
typedef struct {
    Pecas itens[MAX]; // O array que armazena as peças.
    int inicio;       // Índice que aponta para o início da fila (de onde a peça sai).
    int fim;          // Índice que aponta para a próxima posição livre no final da fila (onde a peça entra).
    int total;        // A quantidade atual de elementos na fila.
} Fila;

// Define a estrutura da Pilha para as peças de reserva.
// Lógica LIFO (Last-In, First-Out): a última peça guardada é a primeira a sair.
typedef struct {
    Pecas itens[MAX]; // O array que armazena as peças reservadas.
    int topo;         // Índice que aponta para o topo da pilha.
} Pilha;

// Define a estrutura para salvar o estado do jogo em um arquivo.
typedef struct {
    int slotOcupado;      // Flag (0 ou 1) para indicar se o slot de save está em uso.
    char nomeSave[50];    // Nome que o jogador deu ao save.
    char dataHora[30];    // Data e hora em que o jogo foi salvo.
    Fila filaDePecas;     // A cópia da fila de peças no momento do save.
    Pilha pilhaDeReserva; // A cópia da pilha de reserva no momento do save.
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
void entrarNoJogo(Fila *f, Pilha *p); // Precisa da fila e da pilha.
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

// --- Funções da Pilha (Reserva) ---
void inicializarPilha(Pilha *p);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
void push(Pilha *p, Pecas peca); // 'push' é a operação de inserir na pilha.
void pop(Pilha *p, Pecas *peca);  // 'pop' é a operação de remover da pilha.
void mostrarPilha(Pilha *p);

// --- Funções de Jogo ---
void gerarPecaAleatoriaEInserir(Fila *f);

// --- Funções de Arquivo (Save/Load/Reset) ---
// Precisam da fila e da pilha para salvar/carregar o estado completo.
void gerenciarCarregamento(int *jogoIniciadoFlag, Fila *f, Pilha *p);
void gerenciarSalvamento(int jogoIniciadoFlag, Fila *f, Pilha *p);
void gerenciarReset();
void getNomeArquivo(int slot, char* nomeArquivo);
void mostrarSlots();
void salvarJogo(int slot, Fila *f, Pilha *p);
int carregarJogo(int slot, Fila *f, Pilha *p);
void resetarSlot(int slot);


// ######################################################
// FUNÇÃO PRINCIPAL (main)
// ######################################################
int main() {
    // Inicializa o gerador de números aleatórios com a hora atual como semente.
    // Isso garante que a sequência de peças seja diferente a cada execução do jogo.
    srand(time(NULL));

    int opcao;                // Variável para armazenar a escolha do usuário no menu.
    int jogoIniciado = 0;     // Flag para controlar se um jogo está ativo (1) ou não (0).
    Fila filaDePecas;         // Declara a variável principal da fila de peças para o jogo.
    Pilha pilhaDeReserva;     // Declara a variável para a pilha de peças reservadas.

    // Loop principal do jogo, continua executando até que o usuário escolha a opção 0 para sair.
    do {
        exibirMenuPrincipal();  // Mostra o menu principal na tela.
        scanf("%d", &opcao);    // Lê a opção digitada pelo usuário.
        limparBufferEntrada();  // Limpa o buffer do teclado para evitar leituras indesejadas na próxima iteração.

        // Estrutura switch para tratar a opção escolhida pelo usuário.
        switch (opcao) {
            case 1: // Opção: Novo Jogo
                printf("\nIniciando novo jogo de Tetris!\n");
                proximoIdPeca = 1;                    // Reseta o contador de ID para um novo jogo.
                inicializarFila(&filaDePecas);        // Prepara a fila para um novo jogo (zera contadores).
                inicializarPilha(&pilhaDeReserva);    // Prepara a pilha de reserva.
                manterFilaDePecas(&filaDePecas);      // Garante que a fila seja preenchida com as 5 peças iniciais.
                jogoIniciado = 1;                     // Marca que um jogo foi iniciado.
                entrarNoJogo(&filaDePecas, &pilhaDeReserva); // Leva o jogador diretamente para o menu secundário do jogo.
                break;
            case 2: // Opção: Entrar no Jogo
                if (jogoIniciado) { // Só permite entrar se um jogo já foi iniciado ou carregado.
                    entrarNoJogo(&filaDePecas, &pilhaDeReserva);
                } else {
                    printf("\nVoce precisa iniciar um 'Novo Jogo' ou 'Carregar um Jogo salvo' primeiro!\n");
                }
                break;
            case 3: // Opção: Carregar Jogo
                gerenciarCarregamento(&jogoIniciado, &filaDePecas, &pilhaDeReserva);
                break;
            case 4: // Opção: Salvar Jogo
                gerenciarSalvamento(jogoIniciado, &filaDePecas, &pilhaDeReserva);
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
        // Não pausa se o usuário saiu (0) ou iniciou/entrou em um jogo, pois já há outra tela/menu.
        if (opcao != 0 && opcao != 1 && opcao != 2) {
             printf("\nPressione Enter para voltar ao menu principal...");
             getchar();
        }

    } while (opcao != 0); // O loop continua enquanto a opção for diferente de 0.

    return 0; // Termina o programa com sucesso.
}

// ############################################################################################
// - - - Implementação das funções de Menu e Gerenciamento - - -
// ############################################################################################

// Função para limpar o buffer de entrada do teclado (stdin).
// Evita que caracteres extras (como o "Enter") atrapalhem as próximas leituras (ex: um fgets() depois de um scanf()).
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Imprime o menu principal do jogo na tela.
void exibirMenuPrincipal() {
    printf("\n\n#########################################\n");
    printf("           TETRIS - MENU\n");
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
    printf("             --- EM JOGO ---\n");
    printf("#########################################\n\n");
    printf("1. Ver fila de proximas pecas\n");
    printf("2. Ver pilha de reserva\n");
    printf("3. Usar proxima peca da fila\n");
    printf("4. Armazenar proxima peca na reserva\n");
    printf("5. Usar peca da pilha de reserva\n");
    printf("0. Voltar ao Menu Principal\n");
    printf("--------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

// Controla o loop do menu secundário, onde as ações do jogo acontecem.
// Recebe ponteiros para a fila e a pilha do jogo atual.
void entrarNoJogo(Fila *f, Pilha *p) {
    int opcao;
    // Variável única para armazenar a peça que for removida da fila ou da pilha.
    Pecas pecaManipulada; 

    do { // Loop do menu secundário
        exibirMenuSecundario();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1: // Ver Fila
                mostrarFila(f);
                break;

            case 2: // Ver Pilha de Reserva
                mostrarPilha(p);
                break;

            case 3: // Usar Próxima Peça da Fila
                remover(f, &pecaManipulada); // Remove a peça do início da fila.
                if (pecaManipulada.ID != 0) { // Se o ID for 0, a fila estava vazia.
                    printf("\nPeca [Nome: %s, ID: %d] entrou no jogo vinda da FILA!\n", pecaManipulada.nome, pecaManipulada.ID);
                    manterFilaDePecas(f); // Garante que a fila seja preenchida novamente para ter 5 peças.
                }
                mostrarFila(f); // Mostra a fila atualizada.
                break;

            case 4: // Armazenar Próxima Peça na Reserva
                if (pilhaCheia(p)) {
                    printf("\nA pilha de reserva esta cheia! Impossivel armazenar.\n");
                } else {
                    remover(f, &pecaManipulada); // Tira a peça da fila.
                    if (pecaManipulada.ID != 0) {
                        push(p, pecaManipulada); // Coloca a mesma peça na pilha.
                        printf("\nPeca [Nome: %s, ID: %d] armazenada na reserva.\n", pecaManipulada.nome, pecaManipulada.ID);
                        manterFilaDePecas(f); // Preenche a fila novamente.
                    }
                }
                mostrarFila(f);
                mostrarPilha(p);
                break;

            case 5: // Usar Peça da Reserva
                pop(p, &pecaManipulada); // Tira a peça do topo da pilha.
                if (pecaManipulada.ID != 0) { // Se o ID for 0, a pilha estava vazia.
                    printf("\nPeca [Nome: %s, ID: %d] entrou no jogo vinda da RESERVA!\n", pecaManipulada.nome, pecaManipulada.ID);
                }
                mostrarPilha(p); // Mostra a pilha atualizada.
                break;

            case 0: // Voltar ao Menu Principal
                printf("\nVoltando ao menu principal...\n");
                break;

            default:
                printf("\nOpcao Invalida!\n");
        }

        if (opcao != 0) { // Pausa para o usuário ler a saída antes de mostrar o menu de novo.
            printf("\nPressione Enter para continuar...");
            getchar();
        }

    } while (opcao != 0);
}

// ############################################################################################
// - - - Implementação das funções de Jogo - - -
// ############################################################################################

// Gera e insere uma peça aleatória, respeitando a regra de no máximo 2 por tipo.
void gerarPecaAleatoriaEInserir(Fila *f) {
    // Modelos de peças disponíveis.
    Pecas pecasDisponiveis[4] = {
        {"Formato I", 0, 0}, {"Formato O", 1, 0}, {"Formato T", 2, 0}, {"Formato L", 3, 0}
    };

    if (filaCheia(f)) {
        return; // Se a fila está cheia, não faz nada.
    }

    // 1. Cria uma lista temporária apenas com os tipos de peças que podem ser inseridos.
    int tiposValidos[4];
    int nValidos = 0; // Contador de quantos tipos são válidos.
    for (int i = 0; i < 4; i++) {
        // Se um tipo de peça aparece menos de 2 vezes na fila, ele é considerado válido.
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
    // Começa em 'f->inicio' e dá a volta no array se necessário, usando o operador módulo (%).
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
        if (f->itens[idx].tipo == tipo) {
            contagem++;
        }
    }
    return contagem;
}

// Garante que a fila sempre tenha o número máximo de peças, se possível.
void manterFilaDePecas(Fila *f) {
    // Enquanto o total de peças for menor que o máximo E a fila não estiver cheia...
    while (f->total < MAX && !filaCheia(f)) {
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

// Adiciona uma peça ao final da fila (enqueue).
void inserir(Fila *f, Pecas p) {
    if (filaCheia(f)) {
        return; // Proteção para não inserir em uma fila cheia.
    }
    f->itens[f->fim] = p;           // Adiciona a peça na posição 'fim'.
    f->fim = (f->fim + 1) % MAX;    // Move o 'fim' para a próxima posição, de forma circular.
    f->total++;                     // Incrementa o total de itens.
}

// Remove uma peça do início da fila (dequeue).
void remover(Fila *f, Pecas *p) {
    if (filaVazia(f)) {
        printf("\nFila vazia. Nao ha pecas para remover.\n");
        p->ID = 0; // Usa o ID 0 para sinalizar que nenhuma peça foi removida.
        return;
    }
    *p = f->itens[f->inicio];           // Copia a peça do início da fila para a variável de retorno.
    f->inicio = (f->inicio + 1) % MAX;  // Move o 'inicio' para a próxima posição, de forma circular.
    f->total--;                         // Decrementa o total de itens.
}

// Exibe o conteúdo da fila em formato de tabela.
void mostrarFila(Fila *f) {
    printf("\n\n--- FILA DE PROXIMAS PECAS ---\n");
    printf("----------------------------------\n");
    printf("| Posicao | %-12s | ID  |\n", "Nome");
    printf("----------------------------------\n");

    if (filaVazia(f)) {
        printf("| Fila Vazia                   |\n");
    } else {
        int posicao = 1;
        // Loop que percorre os elementos válidos da fila circular.
        for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
            // Imprime cada peça formatada em uma linha da tabela.
            printf("| %-7d | %-12s | %-3d |\n", posicao, f->itens[idx].nome, f->itens[idx].ID);
            posicao++;
        }
    }
    printf("----------------------------------\n");
}

// ############################################################################################
// - - - Implementação das funções da Pilha (Reserva) - - -
// ############################################################################################

// Prepara a pilha para ser usada, definindo o topo como -1 (vazia).
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Retorna 1 (verdadeiro) se a pilha estiver vazia.
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Retorna 1 (verdadeiro) se a pilha estiver cheia.
int pilhaCheia(Pilha *p) {
    return p->topo == MAX - 1;
}

// Adiciona uma peça ao topo da pilha (push).
void push(Pilha *p, Pecas peca) {
    if (pilhaCheia(p)) {
        printf("\nA pilha de reserva esta cheia! Nao e possivel armazenar.\n");
        return;
    }
    p->topo++;                  // Move o topo para a próxima posição livre.
    p->itens[p->topo] = peca;   // Adiciona o item nessa posição.
}

// Remove uma peça do topo da pilha (pop).
void pop(Pilha *p, Pecas *peca) {
    if (pilhaVazia(p)) {
        printf("\nPilha de reserva vazia! Nao ha pecas para usar.\n");
        peca->ID = 0; // Sinaliza que nenhuma peça foi removida.
        return;
    }
    *peca = p->itens[p->topo]; // Copia a peça do topo para a variável de retorno.
    p->topo--;                 // Decrementa o topo, efetivamente "removendo" o item.
}

// Exibe o conteúdo da pilha de reserva.
void mostrarPilha(Pilha *p) {
    printf("\n\n--- PILHA DE RESERVA ---\n");
    printf("--------------------------\n");
    printf("| %-12s | ID  |\n", "Nome");
    printf("--------------------------\n");
    if (pilhaVazia(p)) {
        printf("| Pilha Vazia        |\n");
    } else {
        // Itera do topo para a base, mostrando a peça mais recente primeiro.
        for (int i = p->topo; i >= 0; i--) {
            printf("| %-12s | %-3d |\n", p->itens[i].nome, p->itens[i].ID);
        }
    }
    printf("--------------------------\n");
}

// ############################################################################################
// - - - FUNÇÕES DE ARQUIVO (SAVE/LOAD/RESET) - - -
// ############################################################################################

// Gerencia o processo de carregar um jogo.
void gerenciarCarregamento(int *jogoIniciadoFlag, Fila *f, Pilha *p) {
    mostrarSlots();
    printf("Escolha o slot para carregar (0 para cancelar): ");
    int slot;
    scanf("%d", &slot);
    limparBufferEntrada();
    if (slot > 0 && slot <= MAX_SAVES) {
        if (carregarJogo(slot, f, p)) { // Tenta carregar o jogo, passando fila e pilha.
            *jogoIniciadoFlag = 1;      // Se conseguir, marca que um jogo está ativo.
        }
    } else {
        printf("Carregamento cancelado.\n");
    }
}

// Gerencia o processo de salvar um jogo.
void gerenciarSalvamento(int jogoIniciadoFlag, Fila *f, Pilha *p) {
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
        salvarJogo(slot, f, p); // Passa a fila e a pilha para serem salvas.
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

// Lê os arquivos de save e exibe o status de cada slot.
void mostrarSlots() {
    printf("\n--- SLOTS DE JOGO ---\n");
    for (int i = 1; i <= MAX_SAVES; i++) {
        char nomeArquivo[20];
        getNomeArquivo(i, nomeArquivo);
        FILE* arquivo = fopen(nomeArquivo, "rb"); // Abre o arquivo para leitura binária ("read binary").
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
void salvarJogo(int slot, Fila *f, Pilha *p) {
    SaveState save;
    save.slotOcupado = 1;
    save.filaDePecas = *f;      // Copia a fila atual do jogo para a estrutura de save.
    save.pilhaDeReserva = *p;   // Copia a pilha atual do jogo.

    printf("Digite um nome para este save (ou '0' para cancelar): ");
    fgets(save.nomeSave, 50, stdin);
    save.nomeSave[strcspn(save.nomeSave, "\n")] = '\0'; // Remove o '\n' do final que o fgets captura.

    if (strcmp(save.nomeSave, "0") == 0) {
        printf("Salvamento cancelado pelo usuario.\n");
        return;
    }

    time_t t = time(NULL); // Pega a hora atual do sistema.
    strftime(save.dataHora, 30, "%d/%m/%Y %H:%M", localtime(&t)); // Formata a data e hora.

    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo, "wb"); // Abre o arquivo para escrita binária ("write binary").
    if (arquivo) {
        fwrite(&save, sizeof(SaveState), 1, arquivo); // Escreve a estrutura inteira no arquivo.
        fclose(arquivo);
        printf("\nJogo salvo com sucesso no Slot %d!\n", slot);
    } else {
        printf("\nErro ao abrir o arquivo para salvar!\n");
    }
}

// Carrega um jogo salvo a partir de um arquivo binário.
int carregarJogo(int slot, Fila *f, Pilha *p) {
    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("\nSlot vazio ou nao encontrado! Nao foi possivel carregar.\n");
        return 0; // Retorna 0 indicando falha.
    }

    SaveState save;
    fread(&save, sizeof(SaveState), 1, arquivo);
    fclose(arquivo);

    *f = save.filaDePecas;     // Restaura a fila do jogo com os dados do save.
    *p = save.pilhaDeReserva;  // Restaura a pilha de reserva.

    // Garante que o próximo ID de peça continue de onde parou no jogo salvo.
    proximoIdPeca = 1; // Reseta o contador.
    if (f->total > 0) {
        int maiorId = 0;
        // Encontra o maior ID na fila carregada para evitar IDs repetidos.
        for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
            if (f->itens[idx].ID > maiorId) {
                maiorId = f->itens[idx].ID;
            }
        }
        proximoIdPeca = maiorId + 1; // Define o próximo ID como o maior + 1.
    }

    // Medida de segurança: Se um save antigo (sem a pilha) for carregado, o valor de 'topo'
    // pode ser lixo de memória. Isso garante que a pilha seja inicializada corretamente.
    if (p->topo < -1 || p->topo >= MAX) {
        inicializarPilha(p);
    }

    manterFilaDePecas(f); // Garante que a fila carregada tenha 5 peças.
    printf("\nJogo '%s' carregado com sucesso do Slot %d!\n", save.nomeSave, slot);
    return 1; // Retorna 1 indicando sucesso.
}

// Deleta um arquivo de save.
void resetarSlot(int slot) {
    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    if (remove(nomeArquivo) == 0) { // Tenta remover o arquivo do disco.
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

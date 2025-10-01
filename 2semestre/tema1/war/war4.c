// ##################
// WAR PARTE 4 - MISSÕES
// ##################

// ######################
// Includes - Inclusão de Bibliotecas
// ######################
/*
    "Includes" são como caixas de ferramentas que nos dão acesso a funções prontas.
    Cada arquivo .h (header file) contém um conjunto de ferramentas para um propósito específico.
*/

#include <stdio.h>    // Standard Input/Output: Essencial para funções como printf() (imprimir na tela) e scanf() (ler do teclado).
#include <stdlib.h>   // Standard Library: Oferece funções úteis como rand() (gerar números aleatórios) e system() (comandos do sistema).
#include <string.h>   // Biblioteca de Strings: Fornece funções para manipular textos (strings), como strcpy() (copiar) e strcmp() (comparar).
#include <time.h>     // Biblioteca de Tempo: Usada aqui para inicializar o gerador de números aleatórios, garantindo que os números sejam diferentes a cada execução do jogo.

// ######################
// Definições globais - Constantes do Jogo
// ######################
/*
    "#define" cria constantes. Usamos nomes em maiúsculas por convenção para identificá-las facilmente.
    Isso torna o código mais legível e fácil de modificar. Se quisermos mudar o número máximo de territórios,
    só precisamos alterar o valor em um único lugar.
*/

#define TAM_STRING 20 // Define o tamanho máximo padrão para nossos textos (nomes de territórios, cores, etc.).
#define MAX_TER 5     // Define o número máximo de territórios no jogo.
#define MAX_SAVES 5   // Define a quantidade de espaços (slots) para salvar o jogo.

// ######################
// Definição das Estruturas - Os "Modelos" de Dados do Jogo
// ######################
/*
    "struct" (estrutura) é um tipo de dado personalizado que agrupa diferentes variáveis sob um único nome.
    É como criar uma "ficha" ou um "molde" para os elementos do nosso jogo.
*/

// Estrutura para representar um único território no mapa.
struct Territorio {
    char nome[TAM_STRING]; // Armazena o nome do território (ex: "Brasil").
    char cor[TAM_STRING];  // Armazena a cor do exército que domina este território (ex: "Verde").
    int isPlayer;          // Funciona como uma bandeira (flag): 1 se o território pertence ao jogador humano, 0 se pertence a um bot.
    int tropa;             // Armazena a quantidade de exércitos (tropas) no território.
};

// Estrutura para armazenar todo o estado de um jogo salvo em um arquivo.
struct SaveState {
    int slotOcupado;                        // Flag: 1 se o slot de save está em uso, 0 se está vazio.
    char nomeSave[50];                      // Nome que o jogador deu para o save (ex: "Minha grande vitoria").
    char dataHora[30];                      // Data e hora em que o jogo foi salvo.
    int totalTerritorios;                   // Guarda o número total de territórios no momento do save.
    struct Territorio territorios[MAX_TER]; // Um "vetor" (array) que guarda a ficha de cada território no jogo.
};

// --- NOVAS ESTRUTURAS PARA AS MISSÕES ---

// "enum" (enumeração) cria um conjunto de constantes nomeadas, melhorando a legibilidade do código.
// Aqui, definimos os dois tipos possíveis de missão. O computador lê ELIMINAR_COR como 0 e CONQUISTAR_TERRITORIOS como 1.
enum TipoMissao { ELIMINAR_COR, CONQUISTAR_TERRITORIOS };

// Estrutura para representar uma missão (objetivo) de um jogador.
struct Missao {
    enum TipoMissao tipo; // Guarda o tipo da missão (será ELIMINAR_COR ou CONQUISTAR_TERRITORIOS).
    int completa;         // Flag: 0 se a missão ainda não foi concluída, 1 se já foi.

    /*
        "union" é uma estrutura especial que permite armazenar diferentes tipos de dados no mesmo local de memória.
        Como uma missão só pode ser de um tipo, não precisamos guardar a cor e a lista de territórios ao mesmo tempo.
        A union economiza memória, guardando OU a cor alvo OU os territórios alvo.
    */
    union {
        char corAlvo[TAM_STRING]; // Usado se o tipo da missão for ELIMINAR_COR.
        int territoriosAlvo[2];   // Usado se o tipo for CONQUISTAR_TERRITORIOS. Armazena os índices dos territórios a serem conquistados.
    } alvo;
};


// ######################################
// Protótipos das Funções - O "Índice" do Código
// ######################################
/*
    Protótipos são declarações que avisam ao compilador sobre a existência de uma função antes de ela ser usada.
    Eles informam o nome da função, o tipo de valor que ela retorna (ex: void para nenhum) e os tipos de dados que ela espera receber (parâmetros).
    Isso nos permite organizar o código, colocando a função main() no início e as implementações das outras funções depois.
*/

void limparBufferEntrada(); // Limpa o "lixo" deixado no buffer de entrada do teclado, evitando bugs com leituras consecutivas.
void exibirMenu();          // Mostra o menu principal do jogo na tela.
// Inicia um novo jogo, cadastrando territórios e definindo o jogador.
void novoJogo(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida, struct Missao missoes[]);
void entrarNoJogo(struct Territorio biblioteca[], int totalTerritorios, int ordemJogadores[], int ordemDefinida); // Inicia o loop principal do jogo (turnos).
// Gerencia o processo de carregar um jogo salvo.
void gerenciarCarregamento(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida, struct Missao missoes[]);
void gerenciarSalvamento(struct Territorio biblioteca[], int totalTerritorios); // Gerencia o processo de salvar o jogo atual.
void gerenciarReset();                                                        // Gerencia a exclusão de um arquivo de save.
void getNomeArquivo(int slot, char* nomeArquivo);                             // Gera o nome do arquivo de save (ex: "save_1.dat").
void mostrarSlots();                                                          // Exibe os slots de save disponíveis (vazios ou ocupados).
void salvarJogo(int slot, struct Territorio biblioteca[], int total);         // Efetivamente escreve os dados do jogo em um arquivo.
int carregarJogo(int slot, struct Territorio biblioteca[]);                   // Efetivamente lê os dados de um arquivo e carrega no jogo. Retorna o número de territórios carregados.
void resetarSlot(int slot);                                                   // Apaga um arquivo de save do disco.
void embaralharOrdem(int *array, int n);                                      // Sorteia a ordem de jogada dos jogadores.
void turnoDoJogador(struct Territorio biblioteca[], int totalTerritorios);    // Executa a lógica do turno do jogador humano.
void turnoDoBot(struct Territorio biblioteca[], int totalTerritorios, int indiceBot); // Executa a lógica do turno de um jogador controlado pelo computador (bot).
void gerarMissoes(struct Missao missoes[], const struct Territorio biblioteca[]); // Cria e atribui missões aleatórias para cada jogador.


// ######################################
// Função Principal (main) - O Coração do Programa
// ######################################
/*
    A função `main` é o ponto de partida de qualquer programa em C.
    É aqui que a execução começa.
*/
int main() {
    // `srand(time(NULL))` inicializa o gerador de números aleatórios.
    // `time(NULL)` pega o tempo atual do sistema (um número que está sempre mudando).
    // Isso garante que a sequência de números aleatórios (rand()) seja diferente a cada vez que o jogo é executado.
    srand(time(NULL));

    // Declaração das variáveis principais que vão armazenar o estado do jogo.
    struct Territorio biblioteca[MAX_TER]; // Um vetor para guardar todos os territórios.
    struct Missao missoes[MAX_TER];        // Um vetor para guardar a missão de cada "cor" ou jogador.
    int totalTerritorios = 0;              // Contador de quantos territórios estão cadastrados. Começa com 0.
    int opcao;                             // Variável para armazenar a escolha do usuário no menu.

    // Variáveis para controlar a ordem de jogada.
    int ordemJogadores[MAX_TER]; // Vetor que vai guardar a sequência de turnos (ex: [2, 0, 1, 4, 3]).
    int ordemDefinida = 0;       // Flag: 0 indica que a ordem ainda não foi definida, 1 que já foi.

    // O `do-while` é um loop que executa o bloco de código PELO MENOS UMA VEZ e depois verifica a condição.
    // O menu continuará aparecendo enquanto a `opcao` escolhida pelo usuário for diferente de 0.
    do {
        exibirMenu();             // Chama a função para mostrar as opções na tela.
        scanf("%d", &opcao);      // Lê o número que o usuário digitou e guarda na variável `opcao`.
        limparBufferEntrada();    // Limpa o buffer para evitar problemas na próxima leitura de texto.

        // `switch` é uma estrutura de decisão que executa um bloco de código baseado no valor de uma variável.
        // É uma alternativa mais organizada a vários `if-else if`.
        switch (opcao) {
            case 1:
                // Se o usuário digitou 1, chama a função para criar um novo jogo.
                // Passamos as variáveis por referência (&) quando queremos que a função altere o valor original delas.
                novoJogo(biblioteca, &totalTerritorios, ordemJogadores, &ordemDefinida, missoes);
                break;
            case 2:
                // Se digitou 2, entra no loop de turnos do jogo.
                entrarNoJogo(biblioteca, totalTerritorios, ordemJogadores, ordemDefinida);
                break;
            case 3:
                // Se digitou 3, gerencia o carregamento de um save.
                gerenciarCarregamento(biblioteca, &totalTerritorios, ordemJogadores, &ordemDefinida, missoes);
                break;
            case 4:
                // Se digitou 4, gerencia o salvamento do jogo.
                gerenciarSalvamento(biblioteca, totalTerritorios);
                break;
            case 5:
                // Se digitou 5, gerencia o reset de um slot de save.
                gerenciarReset();
                break;
            case 0:
                // Se digitou 0, encerra o jogo.
                printf("\nSaindo do Sistema...\n");
                break;
            default:
                // Se digitou qualquer outro número, mostra uma mensagem de erro.
                printf("\nOpcao Invalida! Tente novamente.\n");
                break;
        }

        // Após cada ação (exceto sair), o jogo pausa e espera o usuário pressionar Enter.
        if (opcao != 0) {
            printf("\nPressione Enter para voltar ao menu...");
            getchar(); // Espera por uma tecla ser pressionada.
        }

    } while (opcao != 0); // O loop continua até que o usuário escolha a opção 0.

    return 0; // Retorna 0 para o sistema operacional, indicando que o programa terminou com sucesso.
}

// ######################################
// Implementação das Funções - O "Como Fazer" de Cada Tarefa
// ######################################

/**
 * @brief Limpa o buffer de entrada do teclado.
 *
 * @details Quando usamos `scanf`, o caractere de nova linha (`\n`, gerado ao pressionar Enter)
 * permanece no buffer de entrada. Esta função consome todos os caracteres restantes
 * até encontrar um `\n` ou o fim do arquivo (EOF), evitando que leituras futuras (como `fgets`)
 * peguem esse `\n` indesejado e pulem a entrada do usuário.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Exibe o menu principal do jogo na tela.
 *
 * @details Apenas imprime texto formatado para guiar o usuário.
 */
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

/**
 * @brief Embaralha os elementos de um vetor de inteiros.
 *
 * @details Utiliza o algoritmo Fisher-Yates. Ele percorre o vetor do final para o começo,
 * trocando cada elemento com outro elemento escolhido aleatoriamente de uma posição anterior ou igual.
 * Isso garante que todas as permutações da ordem de jogadores sejam igualmente prováveis.
 *
 * @param array Ponteiro para o vetor de inteiros a ser embaralhado.
 * @param n O número de elementos no vetor.
 */
void embaralharOrdem(int *array, int n) {
    // Loop começa do último elemento e vai até o segundo.
    for (int i = n - 1; i > 0; i--) {
        // Gera um índice aleatório 'j' entre 0 e 'i' (inclusive).
        int j = rand() % (i + 1);
        // Troca os elementos nas posições 'i' e 'j'.
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

/**
 * @brief Gera missões aleatórias para cada jogador (cor).
 *
 * @details A função cria uma missão para cada uma das cores existentes no jogo.
 * A missão pode ser de dois tipos: eliminar outra cor ou conquistar dois territórios específicos.
 * Garante que um jogador não receba a missão de eliminar a si mesmo ou de conquistar territórios que já lhe pertencem.
 *
 * @param missoes Vetor de Missao que será preenchido com as missões geradas.
 * @param biblioteca Vetor constante de Territorio, usado para consulta de dados dos territórios.
 */
void gerarMissoes(struct Missao missoes[], const struct Territorio biblioteca[]) {
    // Primeiro, criamos uma lista com todas as cores únicas presentes no jogo.
    char coresUnicas[MAX_TER][TAM_STRING];
    for (int i = 0; i < MAX_TER; i++) {
        strcpy(coresUnicas[i], biblioteca[i].cor);
    }
    
    // Agora, para cada jogador/cor (índice 'i'), vamos criar uma missão.
    for (int i = 0; i < MAX_TER; i++) {
        missoes[i].completa = 0; // Marca a missão como não completa.
        int tipoDaMissao = rand() % 2; // Sorteia 0 ou 1 para decidir o tipo da missão.

        if (tipoDaMissao == 0) { // Missão: ELIMINAR_COR
            missoes[i].tipo = ELIMINAR_COR;
            int indiceCorAlvo;
            // Sorteia uma cor alvo até encontrar uma que NÃO seja a cor do próprio jogador.
            do {
                indiceCorAlvo = rand() % MAX_TER;
            } while (strcmp(coresUnicas[indiceCorAlvo], biblioteca[i].cor) == 0);
            // Copia a cor sorteada para o campo 'alvo' da missão.
            strcpy(missoes[i].alvo.corAlvo, coresUnicas[indiceCorAlvo]);
        } else { // Missão: CONQUISTAR_TERRITORIOS
            missoes[i].tipo = CONQUISTAR_TERRITORIOS;
            int t1, t2;
            // Sorteia dois índices de territórios (t1, t2) garantindo que:
            // 1. t1 seja diferente de t2 (não pode ser o mesmo território).
            // 2. Nenhum dos territórios alvo seja o território inicial do próprio jogador.
            do {
                t1 = rand() % MAX_TER;
                t2 = rand() % MAX_TER;
            } while (t1 == t2 || t1 == i || t2 == i);
            // Armazena os índices sorteados no campo 'alvo' da missão.
            missoes[i].alvo.territoriosAlvo[0] = t1;
            missoes[i].alvo.territoriosAlvo[1] = t2;
        }
    }
}

/**
 * @brief Configura um novo jogo do zero.
 *
 * @details Pede ao usuário para cadastrar os 5 territórios, escolher qual exército (cor)
 * ele quer controlar, define a ordem de jogada e gera a missão secreta do jogador.
 *
 * @param biblioteca O vetor de territórios que será preenchido.
 * @param totalTerritorios Ponteiro para a variável que controla o número de territórios (será atualizado para MAX_TER).
 * @param ordemJogadores Vetor que guardará a ordem de jogada.
 * @param ordemDefinida Ponteiro para a flag que indica se a ordem foi definida.
 * @param missoes Vetor que será preenchido com as missões geradas.
 */
void novoJogo(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida, struct Missao missoes[]) {
    printf("Iniciando novo jogo...\n");
    printf("Vamos cadastrar os 5 territorios iniciais do nosso mundo \n\n");
    
    // Loop para cadastrar cada um dos 5 territórios.
    for (int i = 0; i < MAX_TER; i++) {
        printf("- - - Cadastrando o %dº Territorio - - - \n", i + 1);
        printf("Nome do Territorio: ");
        fgets(biblioteca[i].nome, TAM_STRING, stdin); // Lê o nome (incluindo espaços).
        printf("Cor do Exercito (ex: Azul, verde, etc):");
        fgets(biblioteca[i].cor, TAM_STRING, stdin); // Lê a cor.
        printf("Numero de tropas: ");
        scanf("%d", &biblioteca[i].tropa); // Lê o número de tropas.
        limparBufferEntrada(); // Essencial após scanf para limpar o '\n'.
        
        // `fgets` captura o '\n' no final da string. A linha abaixo remove esse caractere indesejado.
        biblioteca[i].nome[strcspn(biblioteca[i].nome, "\n")] = '\0';
        biblioteca[i].cor[strcspn(biblioteca[i].cor, "\n")] = '\0';
        printf(" --> Territorio %d cadastrado com sucesso!\n\n", i + 1);
    }
    
    char corEscolhida[TAM_STRING];
    int corValida = 0; // Flag para controlar o loop.
    // Loop `while` que continua pedindo a cor ao usuário até ele digitar uma cor válida.
    while (!corValida) {
        printf("\n--- Escolha seu Exercito ---\n");
        printf("Digite o nome da cor que voce deseja controlar (ex: Azul): ");
        fgets(corEscolhida, TAM_STRING, stdin);
        corEscolhida[strcspn(corEscolhida, "\n")] = '\0'; // Remove o '\n'.
        // Percorre os territórios para verificar se a cor digitada existe.
        for (int i = 0; i < MAX_TER; i++) {
            if (strcmp(biblioteca[i].cor, corEscolhida) == 0) { // `strcmp` retorna 0 se as strings são iguais.
                corValida = 1; // Cor válida encontrada, a flag vira 1 e o loop `while` vai parar.
                break;         // Sai do loop `for`.
            }
        }
        if (!corValida) printf("\nCor invalida! Por favor, digite uma das cores que voce cadastrou.\n");
    }

    // Define qual território pertence ao jogador e quais pertencem aos bots.
    for (int i = 0; i < MAX_TER; i++) {
        if (strcmp(biblioteca[i].cor, corEscolhida) == 0) biblioteca[i].isPlayer = 1; // É do jogador.
        else biblioteca[i].isPlayer = 0; // É de um bot.
    }
    printf("\nVoce escolheu o exercito %s! Boa sorte.\n", corEscolhida);

    // Atualiza o total de territórios e define a ordem de jogada.
    *totalTerritorios = MAX_TER;

    for(int i = 0; i < *totalTerritorios; i++) ordemJogadores[i] = i; // Cria uma sequência inicial (0, 1, 2, 3, 4).
    embaralharOrdem(ordemJogadores, *totalTerritorios); // Embaralha a sequência para definir a ordem dos turnos.
    *ordemDefinida = 1; // Marca que a ordem agora está definida.

    // Gera as missões e exibe a missão do jogador.
    gerarMissoes(missoes, biblioteca);
    for (int i = 0; i < MAX_TER; i++) {
        // Encontra o território do jogador para mostrar a missão correta.
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
            break; // Já encontramos a missão do jogador, podemos sair do loop.
        }
    }
    
    printf("Cadastro concluido! Voce ja pode jogar ou salvar.\n");
}


/**
 * @brief Inicia e gerencia o fluxo de rodadas e turnos do jogo.
 *
 * @details Verifica se um jogo foi iniciado. Se sim, entra em um loop infinito
 * que representa as rodadas. A cada rodada, percorre a ordem de jogadores definida,
 * chamando a função de turno apropriada (jogador ou bot).
 *
 * @param biblioteca Vetor com os dados atuais dos territórios.
 * @param totalTerritorios Número total de territórios no jogo.
 * @param ordemJogadores Vetor com a ordem de jogada.
 * @param ordemDefinida Flag que indica se a ordem foi definida.
 */
void entrarNoJogo(struct Territorio biblioteca[], int totalTerritorios, int ordemJogadores[], int ordemDefinida) {
    // Validação: não é possível jogar sem antes criar um "Novo Jogo" ou "Carregar Jogo".
    if (totalTerritorios == 0 || !ordemDefinida) {
        printf("\nInicie um Novo Jogo ou Carregue um save primeiro.\n");
        return; // Retorna ao menu principal.
    }

    printf("\n--- O Jogo Comecou! ---\n");
    
    // Loop infinito para as rodadas. O jogo só termina se o programa for fechado.
    while(1) {
        printf("\n--- A ordem de jogada e: ---\n");
        // Mostra a ordem de jogada para a rodada atual.
        for(int i = 0; i < totalTerritorios; i++) {
            int jogadorIdx = ordemJogadores[i]; // Pega o índice do jogador da vez.
            printf("%dº a jogar: Exercito %s (%s)\n", i + 1, biblioteca[jogadorIdx].cor, biblioteca[jogadorIdx].isPlayer ? "Voce" : "Bot");
        }
        printf("----------------------------------------\n\n");

        // Loop para os turnos dentro de uma rodada.
        for (int i = 0; i < totalTerritorios; i++) {
            int jogadorAtualIdx = ordemJogadores[i]; // Índice do jogador do turno atual.
            // Verifica se o jogador atual é o humano ou um bot.
            if (biblioteca[jogadorAtualIdx].isPlayer) {
                turnoDoJogador(biblioteca, totalTerritorios); // Chama a função do turno humano.
            } else {
                turnoDoBot(biblioteca, totalTerritorios, jogadorAtualIdx); // Chama a função do turno do bot.
            }
            printf("\nPressione Enter para o proximo turno...");
            getchar(); // Pausa o jogo até o usuário pressionar Enter.
        }
        printf("\n--- Fim da Rodada! Iniciando proxima rodada... ---\n");
    }
}

/**
 * @brief Controla o turno do jogador humano.
 *
 * @details Exibe o mapa, pede para o jogador escolher um território atacante e um defensor,
 * valida a jogada, simula a batalha com dados e atualiza o estado do jogo com base no resultado.
 * O jogador pode atacar quantas vezes quiser até decidir terminar o turno.
 *
 * @param biblioteca Vetor de territórios que será modificado durante os ataques.
 * @param totalTerritorios Número total de territórios.
 */
void turnoDoJogador(struct Territorio biblioteca[], int totalTerritorios) {
    printf("\n--- SEU TURNO ---\n");
    int atacante, defensor;
    
    // Loop para permitir múltiplos ataques no mesmo turno.
    while(1) {
        // Exibe o estado atual do mapa.
        printf("##########################################\n");
        printf("- - - MAPA DO MUNDO - ESTADO ATUAL - - -\n");
        for (int i = 0; i < totalTerritorios; i++) {
            printf("%d. %s (Cor: %s, Tropas: %d)\n", i + 1, biblioteca[i].nome, biblioteca[i].cor, biblioteca[i].tropa);
        }
        printf("---------------------------------------\n\n");

        printf("Escolha seu territorio ATACANTE (1 a %d) ou digite 0 para terminar o turno: ", MAX_TER);
        scanf("%d", &atacante);
        limparBufferEntrada();

        // Se o jogador digitar 0, ele quer terminar o turno.
        if (atacante == 0) break; // `break` sai do loop `while`.

        printf("Escolha o territorio DEFENSOR (1 a %d): ", MAX_TER);
        scanf("%d", &defensor);
        limparBufferEntrada();
        
        // Converte a escolha do usuário (1 a 5) para o índice do vetor (0 a 4).
        int idxAtacante = atacante - 1;
        int idxDefensor = defensor - 1;

        // --- VALIDAÇÃO DA JOGADA ---
        // Verifica se os números são válidos, se o atacante pertence ao jogador, etc.
        if (atacante < 1 || atacante > MAX_TER || defensor < 1 || defensor > MAX_TER || atacante == defensor || !biblioteca[idxAtacante].isPlayer) {
            printf("\nAtaque invalido! Verifique se o territorio atacante e seu.\n");
            continue; // `continue` pula para a próxima iteração do loop, pedindo uma nova jogada.
        }
        // Regra do War: é preciso ter pelo menos 2 tropas para atacar.
        if (biblioteca[idxAtacante].tropa < 2) {
            printf("\nO territorio atacante precisa ter pelo menos 2 tropas!\n");
            continue;
        }
        // Não se pode atacar um território que já é seu.
        if (biblioteca[idxDefensor].isPlayer) {
            printf("\nVoce nao pode atacar seus proprios territorios!\n");
            continue;
        }

        // --- SIMULAÇÃO DA BATALHA ---
        int dadoAtaque = rand() % 6 + 1; // Gera um número aleatório de 1 a 6.
        int dadoDefesa = rand() % 6 + 1;
        printf("\nVoce rolou %d no dado de ataque!\n", dadoAtaque);
        printf("O defensor %s rolou %d no dado de defesa!\n", biblioteca[idxDefensor].nome, dadoDefesa);

        // Compara os dados para determinar o vencedor.
        if (dadoAtaque > dadoDefesa) {
            printf("VITORIA! O defensor perdeu 1 tropa.\n\n");
            biblioteca[idxDefensor].tropa--;
            // Verifica se o território foi conquistado.
            if (biblioteca[idxDefensor].tropa <= 0) {
                printf("!!! O territorio %s foi conquistado por voce !!!\n", biblioteca[idxDefensor].nome);
                // Transfere a propriedade do território para o jogador.
                strcpy(biblioteca[idxDefensor].cor, biblioteca[idxAtacante].cor);
                biblioteca[idxDefensor].isPlayer = 1;
                biblioteca[idxDefensor].tropa = 1; // O território conquistado fica com 1 tropa.
                biblioteca[idxAtacante].tropa--;   // A tropa que se moveu sai do território de origem.
            }
        } else { // Se o dado de defesa for maior ou igual ao de ataque.
            printf("DERROTA! Voce perdeu 1 tropa.\n\n");
            biblioteca[idxAtacante].tropa--;
        }
    }
}

/**
 * @brief Controla o turno de um jogador-bot.
 *
 * @details A IA (Inteligência Artificial) do bot é simples:
 * 1. Procura o primeiro território que pertence a ele e que pode atacar (>1 tropa).
 * 2. Lista todos os territórios inimigos como alvos possíveis.
 * 3. Escolhe um alvo aleatoriamente.
 * 4. Executa um único ataque e termina o turno.
 *
 * @param biblioteca Vetor de territórios a ser modificado.
 * @param totalTerritorios Número total de territórios.
 * @param indiceBot O índice do território que define a cor do bot que está jogando.
 */
void turnoDoBot(struct Territorio biblioteca[], int totalTerritorios, int indiceBot) {
    printf("\n--- TURNO DO EXERCITO %s (Bot) ---\n", biblioteca[indiceBot].cor);
    
    // 1. Encontra um território para atacar.
    int idxAtacante = -1;
    for (int i = 0; i < totalTerritorios; i++) {
        // O território deve pertencer à cor do bot atual e ter mais de 1 tropa.
        if (!biblioteca[i].isPlayer && strcmp(biblioteca[i].cor, biblioteca[indiceBot].cor) == 0 && biblioteca[i].tropa > 1) {
            idxAtacante = i;
            break; // Encontrou um, pode parar de procurar.
        }
    }

    // Se não encontrou nenhum território capaz de atacar, passa o turno.
    if (idxAtacante == -1) {
        printf("O exercito %s nao tem territorios para atacar. Passando o turno.\n", biblioteca[indiceBot].cor);
        return;
    }

    // 2. Encontra um alvo para o ataque.
    int idxDefensor = -1;
    int alvosPossiveis[MAX_TER]; // Vetor para guardar os índices dos alvos.
    int numAlvos = 0;
    // Lista todos os territórios que NÃO pertencem ao bot.
    for(int i = 0; i < totalTerritorios; i++){
        if(strcmp(biblioteca[i].cor, biblioteca[idxAtacante].cor) != 0){
            alvosPossiveis[numAlvos++] = i;
        }
    }

    // Se não há inimigos no mapa, passa o turno.
    if (numAlvos == 0) {
         printf("O exercito %s nao tem alvos para atacar. Passando o turno.\n", biblioteca[indiceBot].cor);
         return;
    }
    
    // 3. Escolhe um alvo aleatório da lista de possíveis alvos.
    idxDefensor = alvosPossiveis[rand() % numAlvos];

    printf("O exercito %s ataca %s!\n", biblioteca[idxAtacante].nome, biblioteca[idxDefensor].nome);

    // 4. Simula a batalha (mesma lógica do turno do jogador).
    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    if (dadoAtaque > dadoDefesa) {
        printf("VITORIA DO BOT! O defensor perdeu 1 tropa.\n\n");
        biblioteca[idxDefensor].tropa--;
        if (biblioteca[idxDefensor].tropa <= 0) {
            printf("!!! O territorio %s foi conquistado pelo exercito %s !!!\n", biblioteca[idxDefensor].nome, biblioteca[idxAtacante].cor);
            strcpy(biblioteca[idxDefensor].cor, biblioteca[idxAtacante].cor);
            biblioteca[idxDefensor].isPlayer = 0; // O território agora pertence a um bot.
            biblioteca[idxDefensor].tropa = 1;
            biblioteca[idxAtacante].tropa--;
        }
    } else {
        printf("DERROTA DO BOT! O atacante perdeu 1 tropa.\n\n");
        biblioteca[idxAtacante].tropa--;
    }
}


/**
 * @brief Gerencia o menu de carregamento de jogo.
 *
 * @details Mostra os slots de save, pede ao usuário para escolher um, e se o carregamento
 * for bem-sucedido, reconfigura o estado do jogo (define o jogador, a ordem de turnos e a missão).
 *
 * @param biblioteca Vetor de territórios que será preenchido com os dados do save.
 * @param totalTerritorios Ponteiro para a variável que será atualizada com o número de territórios do save.
 * @param ordemJogadores Vetor que será preenchido com a nova ordem de jogada.
 * @param ordemDefinida Ponteiro para a flag que será marcada como 1.
 * @param missoes Vetor de missões que será preenchido.
 */
void gerenciarCarregamento(struct Territorio biblioteca[], int *totalTerritorios, int ordemJogadores[], int *ordemDefinida, struct Missao missoes[]) {
    int slotEscolhido;
    mostrarSlots(); // Mostra os saves disponíveis.
    printf("Escolha o slot para carregar: ");
    scanf("%d", &slotEscolhido);
    limparBufferEntrada();
    if (slotEscolhido != 0) { // 0 é a opção de cancelar.
        int carregados = carregarJogo(slotEscolhido, biblioteca); // Tenta carregar.
        if (carregados > 0) { // Se `carregarJogo` retornou um número maior que 0, sucesso!
            *totalTerritorios = carregados;
            
            // Após carregar, o jogo não sabe qual cor o jogador estava usando.
            // Precisamos perguntar novamente para redefinir o campo `isPlayer`.
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
            // Redefine quem é jogador e quem é bot.
            for (int i = 0; i < *totalTerritorios; i++) {
                if (strcmp(biblioteca[i].cor, corEscolhida) == 0) biblioteca[i].isPlayer = 1;
                else biblioteca[i].isPlayer = 0;
            }

            // A ordem de jogada é re-sorteada após o carregamento.
            for(int i = 0; i < *totalTerritorios; i++) ordemJogadores[i] = i; 
            embaralharOrdem(ordemJogadores, *totalTerritorios);
            *ordemDefinida = 1; 

            // Gera uma nova missão para o jogador.
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

/**
 * @brief Gerencia o menu de salvamento de jogo.
 *
 * @details Verifica se há um jogo em andamento para salvar. Se houver, mostra os slots
 * e chama a função `salvarJogo` para o slot escolhido pelo usuário.
 *
 * @param biblioteca Vetor com os dados do jogo atual a serem salvos.
 * @param totalTerritorios Número de territórios no jogo atual.
 */
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

/**
 * @brief Gerencia o menu de reset de um slot de save.
 *
 * @details Mostra os slots de save e pede ao usuário para escolher qual deletar.
 */
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

// --- Funções de arquivo (Save/Load) ---

/**
 * @brief Monta o nome do arquivo de save com base no número do slot.
 *
 * @details Por exemplo, se slot for 1, o nomeArquivo será "save_1.dat".
 * A função `sprintf` é como `printf`, mas em vez de imprimir na tela,
 * ela "imprime" (formata) o texto dentro de uma string.
 *
 * @param slot O número do slot (1 a 5).
 * @param nomeArquivo Ponteiro para a string onde o nome do arquivo será armazenado.
 */
void getNomeArquivo(int slot, char* nomeArquivo) {
    sprintf(nomeArquivo, "save_%d.dat", slot);
}

/**
 * @brief Exibe o status de cada slot de save.
 *
 * @details Itera por todos os slots possíveis. Para cada um, tenta abrir o arquivo
 * correspondente. Se o arquivo não existir, o slot está vazio. Se existir,
 * lê os dados do save (nome, data/hora) e os exibe.
 */
void mostrarSlots() {
    printf("\n--- Escolha um Slot ---\n");
    for (int i = 0; i < MAX_SAVES; i++) {
        char nomeArquivo[20];
        getNomeArquivo(i + 1, nomeArquivo);
        // Tenta abrir o arquivo para leitura binária ("rb" - read binary).
        FILE* arquivo = fopen(nomeArquivo, "rb");
        if (arquivo == NULL) { // Se fopen retorna NULL, o arquivo não existe.
            printf("%d. [ Vazio ]\n", i + 1);
        } else { // Se o arquivo existe...
            struct SaveState save;
            // Lê o conteúdo do arquivo e o carrega na variável 'save'.
            fread(&save, sizeof(struct SaveState), 1, arquivo);
            fclose(arquivo); // Sempre feche um arquivo depois de usá-lo!
            printf("%d. %s (%s)\n", i + 1, save.nomeSave, save.dataHora);
        }
    }
    printf("0. Cancelar\n");
    printf("-----------------------\n");
}

/**
 * @brief Salva o estado atual do jogo em um arquivo binário.
 *
 * @details Preenche uma estrutura `SaveState` com todos os dados do jogo,
 * pede um nome para o save, registra a data/hora e escreve toda a estrutura
 * de uma vez no arquivo.
 *
 * @param slot O slot (1 a 5) onde o jogo será salvo.
 * @param biblioteca O vetor de territórios do jogo atual.
 * @param total O número de territórios.
 */
void salvarJogo(int slot, struct Territorio biblioteca[], int total) {
    if (slot < 1 || slot > MAX_SAVES) {
        printf("Slot invalido!\n");
        return;
    }
    struct SaveState save; // Cria a "ficha" de save.
    save.slotOcupado = 1;
    save.totalTerritorios = total;
    // Copia os dados de cada território para a estrutura de save.
    for (int i = 0; i < total; i++) {
        save.territorios[i] = biblioteca[i];
    }
    printf("Digite um nome para este save: ");
    fgets(save.nomeSave, 50, stdin);
    save.nomeSave[strcspn(save.nomeSave, "\n")] = '\0'; // Remove o '\n'.
    
    // Pega a data e hora atuais e formata para o formato dd/mm/aaaa HH:MM.
    time_t t = time(NULL);
    strftime(save.dataHora, 30, "%d/%m/%Y %H:%M", localtime(&t));
    
    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    // Abre o arquivo para escrita binária ("wb" - write binary). Se o arquivo não existir, ele é criado.
    FILE* arquivo = fopen(nomeArquivo, "wb");
    // Escreve o conteúdo da variável 'save' no arquivo.
    fwrite(&save, sizeof(struct SaveState), 1, arquivo);
    fclose(arquivo);
    printf("\nJogo salvo com sucesso no Slot %d!\n", slot);
}

/**
 * @brief Carrega o estado de um jogo a partir de um arquivo binário.
 *
 * @details Tenta abrir o arquivo do slot especificado. Se conseguir, lê os dados
 * do arquivo para uma estrutura `SaveState` e depois copia os dados dos territórios
 * de volta para o vetor principal do jogo.
 *
 * @param slot O slot (1 a 5) de onde o jogo será carregado.
 * @param biblioteca O vetor de territórios que será preenchido com os dados carregados.
 * @return O número de territórios que foram carregados, ou 0 em caso de falha.
 */
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
        return 0; // Retorna 0 para indicar que o carregamento falhou.
    }
    struct SaveState save;
    fread(&save, sizeof(struct SaveState), 1, arquivo);
    fclose(arquivo);
    // Copia os dados do save de volta para a variável do jogo.
    for (int i = 0; i < save.totalTerritorios; i++) {
        biblioteca[i] = save.territorios[i];
    }
    printf("\nJogo carregado com sucesso do Slot %d!\n", slot);
    return save.totalTerritorios; // Retorna o número de territórios para o jogo saber seu estado.
}

/**
 * @brief Deleta um arquivo de save.
 *
 * @details A função `remove()` da biblioteca `<stdio.h>` deleta um arquivo do disco.
 *
 * @param slot O slot (1 a 5) a ser resetado.
 */
void resetarSlot(int slot) {
    if (slot < 1 || slot > MAX_SAVES) {
        printf("Slot invalido!\n");
        return;
    }
    char nomeArquivo[20];
    getNomeArquivo(slot, nomeArquivo);
    // `remove` retorna 0 se a exclusão foi bem-sucedida.
    if (remove(nomeArquivo) == 0) {
        printf("Slot %d resetado com sucesso!\n", slot);
    } else {
        printf("Slot %d ja estava vazio.\n", slot);
    }
}

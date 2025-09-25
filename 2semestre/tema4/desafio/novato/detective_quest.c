// ========================================================================================
// JOGO: DETECTIVE QUEST
// DESCRIÇÃO: Um jogo de exploração em uma casa misteriosa usando uma árvore binária
//            para representar o mapa. O jogador navega pelos cômodos, encontra
//            itens e desvenda o ambiente.
// ========================================================================================

// ========================================================================================
// BIBLIOTECAS (INCLUDES)
// ========================================================================================
// NOTA DE APRENDIZADO: Bibliotecas são conjuntos de funções pré-escritas que podemos
// usar em nosso código. Incluí-las é o primeiro passo em quase todo programa C.
#include <stdio.h>      // Funções padrão de entrada e saída (printf, scanf, fopen, etc.).
#include <stdlib.h>     // Funções gerais (malloc, free, exit, rand, srand, etc.).
#include <string.h>     // Funções para manipulação de strings (strcpy, strcmp, etc.).
#include <time.h>       // Funções para obter a data e hora do sistema, essencial para o rand().

// ========================================================================================
// DEFINIÇÕES GLOBAIS (CONSTANTES)
// ========================================================================================
// NOTA DE APRENDIZADO: Usar #define para criar constantes torna o código mais legível
// e fácil de manter. Se quisermos mudar o tamanho da mochila, por exemplo, mudamos
// em um só lugar.
#define MAX_SAVES 5             // Define o número máximo de slots para salvar o jogo.
#define TOTAL_COMODOS 8         // O número total de cômodos únicos no jogo.
#define ITENS_POR_COMODO 5      // Cada cômodo terá 5 itens únicos para encontrar.
#define FRASES_POR_COMODO 10    // Cada cômodo terá 10 frases (5 de itens, 5 de ambiente).
#define TOTAL_FRASES_VAZIO 40   // Total de frases genéricas para quando um item já foi pego.
#define TAMANHO_MOCHILA 50      // Define o tamanho máximo da mochila do jogador.

// ========================================================================================
// ESTRUTURAS DE DADOS (STRUCTS)
// ========================================================================================
// NOTA DE APRENDIZADO: A ordem de declaração das estruturas é crucial em C.
// Uma estrutura (como GameState) não pode usar outra (como MochilaEstatica)
// antes que a segunda tenha sido totalmente definida. Por isso, Item e
// MochilaEstatica são definidas primeiro.

/**
 * @struct Item
 * @brief Define a estrutura de um item que pode ser guardado na mochila.
 * Agrupa as informações de um item em um único tipo de dado.
 */
typedef struct {
    char nome[50];  // Nome do item.
    char tipo[50];  // Categoria do item (Ferramenta, Documento, etc.).
} Item;

/**
 * @struct MochilaEstatica
 * @brief Define a estrutura da mochila do jogador, que é um vetor estático.
 * "Estático" significa que seu tamanho é fixo, definido pela constante TAMANHO_MOCHILA.
 */
typedef struct {
    Item dados[TAMANHO_MOCHILA]; // Um array (vetor) para guardar os itens.
    int quantidade;              // Um contador para saber quantos itens temos atualmente.
} MochilaEstatica;

/**
 * @struct No
 * @brief Estrutura de um Nó da árvore, que representa um cômodo ESTRUTURAL no mapa.
 * É o "tijolo" que usamos para construir a árvore binária do mapa.
 */
typedef struct No {
    int comodoId;           // O ID que liga este nó ao seu conteúdo (nome, frases, etc.).
    struct No* esquerda;    // Ponteiro para o cômodo à esquerda. Se for NULL, não há caminho.
    struct No* direita;     // Ponteiro para o cômodo à direita. Se for NULL, não há caminho.
    struct No* pai;         // Ponteiro para o cômodo anterior. Essencial para a função "voltar".
} No;

/**
 * @struct GameState
 * @brief Armazena todo o estado dinâmico do jogo que precisa ser salvo e carregado.
 * Reúne todas as informações que mudam durante o jogo em um único lugar.
 */
typedef struct {
    No* raiz;               // Ponteiro para o início do mapa (a raiz da árvore).
    No* posicaoAtual;       // Ponteiro para o cômodo onde o jogador está no momento.
    MochilaEstatica mochila; // A mochila do jogador.
    int itensJaColetados[TOTAL_COMODOS][ITENS_POR_COMODO]; // Matriz de flags [cômodo][item] para controlar o que já foi pego.
    int totalItensColetados; // Contador geral de itens para o progresso do jogo.
} GameState;

/**
 * @struct SaveState
 * @brief Estrutura completa que é escrita e lida dos arquivos de save.
 * Esta é a estrutura que será gravada diretamente no arquivo binário.
 */
typedef struct {
    int slotOcupado;        // Uma flag (1 ou 0) para saber se o slot de save tem dados válidos.
    char nomeSave[50];      // Nome que o jogador deu ao save.
    char dataHora[30];      // Data e hora em que o jogo foi salvo.
    GameState estadoDoJogo; // Aninhar o GameState aqui simplifica o salvamento/carregamento.
} SaveState;


// ========================================================================================
// BANCO DE DADOS GLOBAL DO JOGO
// ========================================================================================
// NOTA DE APRENDIZADO: Separar os dados (textos, nomes) da lógica (código) é uma ótima
// prática. Isso torna o jogo muito mais fácil de modificar, traduzir ou expandir
// sem precisar alterar as funções principais.

// Estrutura para armazenar o CONTEÚDO de um cômodo.
typedef struct {
    const char* nome;
    const char* frases[FRASES_POR_COMODO];
    const char* itens[ITENS_POR_COMODO];
    const char* tiposItens[ITENS_POR_COMODO];
} ComodoData;

// Array global que funcionará como nosso "banco de dados" de conteúdo.
ComodoData DADOS_COMODOS[TOTAL_COMODOS];

// Array global com frases genéricas para quando o jogador explora um local onde o item já foi pego.
const char* FRASES_GLOBAIS_VAZIO[TOTAL_FRASES_VAZIO] = {
    "Voce sente um calafrio percorrer sua espinha.", "O silencio nesta casa e ensurdecedor.",
    "Uma tabua solta no chao range em algum lugar proximo.", "Voce tem a nitida sensacao de estar sendo observado.",
    "A poeira danca no feixe de luz que entra pela janela.", "Uma teia de aranha roca seu rosto, fazendo voce recuar.",
    "O ar aqui e pesado, carregado de memorias esquecidas.", "Voce esbarra em um movel coberto por um lencol branco.",
    "Um livro aberto em uma pagina amarelada jaz sobre uma poltrona.", "As sombras parecem se mover nos cantos da sua visao.",
    "Voce se pergunta que historias estas paredes poderiam contar.", "O som do seu proprio coracao parece alto demais.",
    "Um retrato na parede parece seguir seus movimentos com os olhos.", "O medo e um companheiro constante neste lugar.",
    "Cada passo a frente parece um passo para mais fundo no desconhecido.", "Um vento subito fecha uma porta com um baque, assustando voce.",
    "A solidao do lugar e quase palpavel.", "Voce encontra uma marca de mao empoeirada na parede.",
    "Um vago cheiro de perfume antigo ainda paira no ar.", "Voce pisa em algo que se quebra, mas nao ve o que e.",
    "A escuridao parece ter vida propria aqui.", "Voce pensa em desistir, mas a curiosidade e mais forte.",
    "Um pequeno ruido faz voce parar e prender a respiraçao.", "A esperanca de encontrar uma saida comeca a diminuir.",
    "Tudo aqui parece congelado no tempo.", "Voce limpa uma janela suja, mas la fora a escuridao e total.",
    "Sua respiracao forma uma nevoa no ar frio.", "Voce se sente como um intruso em um tumulo.",
    "A adrenalina mantem voce alerta a cada segundo.", "Um eco distante responde ao som dos seus passos.",
    "A realidade parece se distorcer levemente neste ambiente.", "Voce se lembra por que veio aqui e reune coragem.",
    "O papel de parede esta descascando, revelando camadas de historia.", "O som de gotas caindo ritmadamente vem de algum lugar.",
    "Voce se sente pequeno e insignificante diante da idade desta casa.", "Uma duvida surge: você esta sozinho aqui?",
    "A luz da sua lanterna parece ser engolida pelas sombras.", "Um sentimento de tristeza profunda emana deste comodo.",
    "Você nota que não ha espelhos em lugar nenhum.", "Sua intuicao grita que voce esta perto de uma descoberta."
};

// ========================================================================================
// PROTÓTIPOS DAS FUNÇÕES
// ========================================================================================
// NOTA DE APRENDIZADO: Protótipos são declarações que informam ao compilador sobre a
// existência de uma função antes de ela ser efetivamente definida. Isso nos permite
// organizar o código de forma mais livre, como chamar uma função na main() mesmo que
// sua definição completa esteja no final do arquivo.

// --- Funções de Inicialização e Utilitárias ---
void inicializarDadosDoJogo();
void limparBufferEntrada();

// --- Funções de Menu e Jogo ---
void exibirMenuPrincipal();
void entrarNoJogo(GameState *estado);
void mostrarCreditos();

// --- Funções do Mapa (Árvore Binária) ---
No* criarNo(int comodoId);
No* criarMapa();
void liberarMapa(No* raiz);

// --- Funções da Mochila ---
void iniciarMochila(MochilaEstatica *mochila);
void adicionarItemNaMochila(MochilaEstatica *mochila, const char* nome, const char* tipo);
void verMochila(const MochilaEstatica *mochila);
void ordenarMochila(MochilaEstatica *mochila);
void menuMochila(GameState* estado);

// --- Funções de Arquivo (Save/Load/Reset) ---
void gerenciarCarregamento(int *jogoIniciadoFlag, GameState *estado);
void gerenciarSalvamento(int jogoIniciadoFlag, GameState *estado);
void gerenciarReset();
void getNomeArquivo(int slot, char* nomeArquivo);
void mostrarSlots();
void salvarJogo(int slot, GameState *estado);
int carregarJogo(int slot, GameState *estado);
void resetarSlot(int slot);

// ========================================================================================
// FUNÇÃO PRINCIPAL (main)
// ========================================================================================
// NOTA DE APRENDIZADO: A função main() é o ponto de entrada de todo programa em C.
// A execução sempre começa aqui.
int main() {
    // Semeia o gerador de números aleatórios. Fazer isso UMA VEZ no início do programa
    // garante que os eventos aleatórios (como as frases que aparecem) sejam diferentes
    // a cada vez que o jogo é executado.
    srand(time(NULL));

    // Carrega todo o conteúdo textual do jogo para a memória.
    inicializarDadosDoJogo();

    int opcao;
    int jogoIniciado = 0; // Flag para controlar se um jogo está ativo.
    GameState estadoDoJogo; // Variável que guardará todo o estado do jogo atual.
    estadoDoJogo.raiz = NULL; // É uma boa prática inicializar ponteiros com NULL.

    // Loop principal do menu. O programa ficará aqui até que o usuário escolha sair (opção 0).
    do {
        exibirMenuPrincipal();
        
        // Validação de entrada: Garante que o usuário digite um NÚMERO.
        // Se ele digitar letras, o programa pedirá novamente em vez de quebrar.
        // O `scanf` retorna o número de itens lidos com sucesso. Esperamos ler 1 inteiro.
        while (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Por favor, digite um numero: ");
            limparBufferEntrada(); // Limpa a entrada inválida (as letras).
        }
        limparBufferEntrada(); // Limpa o "Enter" que sobrou após ler o número.

        // Estrutura switch para lidar com a escolha do usuário.
        switch (opcao) {
            case 1: // Novo Jogo
                printf("\nIniciando um novo jogo de Detective Quest!\n");
                
                // Se já existia um mapa de um jogo anterior, libera a memória dele antes de criar um novo.
                if (estadoDoJogo.raiz != NULL) {
                    liberarMapa(estadoDoJogo.raiz);
                }
                
                // Cria a estrutura do mapa e define a posição inicial do jogador.
                estadoDoJogo.raiz = criarMapa();
                estadoDoJogo.posicaoAtual = estadoDoJogo.raiz;
                jogoIniciado = 1; // Marca que um jogo está ativo.

                // Zera o estado do jogador para um novo jogo.
                iniciarMochila(&estadoDoJogo.mochila);
                estadoDoJogo.totalItensColetados = 0;
                for(int i = 0; i < TOTAL_COMODOS; i++) {
                    for (int j = 0; j < ITENS_POR_COMODO; j++) {
                        estadoDoJogo.itensJaColetados[i][j] = 0; // Zera a matriz de coleta.
                    }
                }
                entrarNoJogo(&estadoDoJogo); // Inicia a exploração.
                break;

            case 2: // Continuar Jogo
                if (jogoIniciado) {
                    entrarNoJogo(&estadoDoJogo);
                } else {
                    printf("\nVoce precisa iniciar um 'Novo Jogo' ou 'Carregar um Jogo' primeiro!\n");
                }
                break;

            case 3: gerenciarCarregamento(&jogoIniciado, &estadoDoJogo); break;
            case 4: gerenciarSalvamento(jogoIniciado, &estadoDoJogo); break;
            case 5: gerenciarReset(); break;
            case 6: mostrarCreditos(); break;
            case 0: printf("\nSaindo de Detective Quest...\n"); break;
            default: printf("\nOpcao Invalida! Tente novamente.\n");
        }

        // Pausa para o usuário ler a saída dos menus de Save/Load/Reset antes de voltar ao menu principal.
        if (opcao > 2 && opcao < 6) {
             printf("\nPressione Enter para voltar ao menu principal...");
             getchar();
        }

    } while (opcao != 0);

    // Antes de terminar o programa, libera a memória do mapa, se ele ainda existir.
    // Isso evita "memory leaks" (vazamentos de memória).
    if (estadoDoJogo.raiz != NULL) {
        liberarMapa(estadoDoJogo.raiz);
    }

    return 0; // Retorna 0 para o sistema operacional, indicando que o programa terminou com sucesso.
}

// ========================================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ========================================================================================

/**
 * @brief Preenche o "banco de dados" global com todo o conteúdo textual do jogo.
 */
void inicializarDadosDoJogo() {
    DADOS_COMODOS[0] = (ComodoData){
        .nome = "Hall de Entrada",
        .frases = {
            "Uma CHAVE DE FENDA repousa sobre uma pequena mesa.",
            "Voce nota um MAPA DA CASA amassado sob o capacho.",
            "Um ISQUEIRO de metal esta no chao.",
            "Uma garrafa de AGUA pela metade foi deixada para tras.",
            "Um pe de CABRA pequeno esta encostado na parede.",
            "O grande lustre de cristal está coberto de teias de aranha.",
            "O papel de parede floral está descascando perto do teto.",
            "Um espelho com uma moldura ornamentada reflete uma imagem distorcida.",
            "O ar e pesado com o cheiro de poeira e madeira velha.",
            "Um relogio de pendulo quebrado marca o tempo que parou."},
        .itens = {"Chave de Fenda", "Mapa da Casa", "Isqueiro", "Garrafa de Agua", "Pe de Cabra Pequeno"},
        .tiposItens = {"Ferramenta", "Documento", "Utilidade", "Consumivel", "Ferramenta"}
    };
    DADOS_COMODOS[1] = (ComodoData){
        .nome = "Sala de Estar",
        .frases = {
            "Uma LANTERNA robusta esta sobre a lareira.",
            "Um rolo de FITA ADESIVA prateada esta no braço de uma poltrona.",
            "Voce encontra um CANIVETE SUICO.",
            "Uma BARRA DE CEREAL embalada esta na mesa de centro.",
            "Um par de LUVAS DE COURO está jogado no sofa.",
            "Uma lareira de mármore domina a parede, cheia de fuligem.",
            "Retratos de familia com olhos vazios observam das paredes.",
            "O tapete persa está desbotado e manchado.",
            "Um piano de cauda coberto por um lençol branco ocupa um canto.",
            "As cortinas pesadas bloqueiam a maior parte da luz exterior."},
        .itens = {"Lanterna", "Fita Adesiva", "Canivete Suico", "Barra de Cereal", "Luvas de Couro"},
        .tiposItens = {"Utilidade", "Utilidade", "Ferramenta", "Consumivel", "Vestimenta"}
    };
    DADOS_COMODOS[2] = (ComodoData){
        .nome = "Sala de Jantar",
        .frases = {
            "Um abridor de garrafas (SACA-ROLHAS) esta sobre a mesa.",
            "Uma caixa de FOSFOROS quase cheia foi deixada ao lado de um candelabro.",
            "Um guardanapo de pano esconde um pequeno BLOCO DE NOTAS.",
            "Um conjunto de TALHERES DE PRATA parece valioso.",
            "Uma garrafa de VINHO ANTIGO, ainda lacrada, esta na cristaleira.",
            "Uma longa mesa de mogno com oito cadeiras ocupa a sala.",
            "A cristaleira de vidro esta empoeirada.",
            "Um lustre de ferro forjado pende baixo sobre a mesa.",
            "Manchas escuras no carpete sugerem que algo foi derramado.",
            "O serviço de cha de porcelana está perfeitamente arrumado."},
        .itens = {"Saca-rolhas", "Fosforos", "Bloco de Notas", "Talheres de Prata", "Vinho Antigo"},
        .tiposItens = {"Ferramenta", "Utilidade", "Documento", "Valor", "Consumivel"}
    };
    DADOS_COMODOS[3] = (ComodoData){
        .nome = "Biblioteca",
        .frases = {
            "Uma LUPA com cabo de marfim esta sobre um livro aberto.",
            "Escondido atras de livros, voce encontra um DIARIO ANTIGO.",
            "Um abridor de cartas (FACA DE PAPEL) esta sobre a escrivaninha.",
            "Uma CANETA-TINTEIRO e um vidro de tinta seca estao em um canto.",
            "Uma CAIXA DE MUSICA empoeirada toca uma melodia fantasmagorica.",
            "Prateleiras do chão ao teto estao repletas de livros.",
            "O cheiro de couro e papel velho enche o ar.",
            "Uma escada de biblioteca de madeira esta encostada em uma estante.",
            "Um globo terrestre antigo mostra fronteiras de paises que nao existem mais.",
            "Uma poltrona de couro gasta esta posicionada perto de uma janela."},
        .itens = {"Lupa", "Diario Antigo", "Faca de Papel", "Caneta-tinteiro", "Caixa de Musica"},
        .tiposItens = {"Investigacao", "Documento", "Ferramenta", "Utilidade", "Curiosidade"}
    };
    DADOS_COMODOS[4] = (ComodoData){
        .nome = "Varanda",
        .frases = {
            "Um par de BINOCULOS foi deixado em uma cadeira de vime.",
            "Um BARBANTE resistente esta enrolado em um dos pilares.",
            "Um velho regador de metal (RECIPIENTE) esta em um canto.",
            "Um pacote de SEMENTES estranhas esta sobre o parapeito.",
            "Uma CHAVE PEQUENA E ORNAMENTADA esta no chao.",
            "A varanda de madeira range perigosamente sob seus pes.",
            "Plantas mortas em vasos de barro se alinham no parapeito.",
            "A vista e de um jardim abandonado e uma floresta escura.",
            "O vento assobia atraves das frestas da madeira.",
            "Uma cadeira de balanço se move lentamente para frente e para tras."},
        .itens = {"Binoculos", "Barbante", "Recipiente", "Sementes Estranhas", "Chave Ornamentada"},
        .tiposItens = {"Investigacao", "Utilidade", "Utilidade", "Curiosidade", "Chave"}
    };
    DADOS_COMODOS[5] = (ComodoData){
        .nome = "Cozinha",
        .frases = {
            "Uma FACA DE COZINHA afiada esta em um cepo.",
            "Um rolo de PAPEL ALUMINIO está sobre o balcao.",
            "Um pote de SAL grosso esta ao lado do fogao.",
            "Uma lata de FEIJAO ENLATADO parece comestivel.",
            "Um MARTELO DE CARNE está pendurado em um gancho.",
            "A cozinha esta surpreendentemente limpa, mas sem uso.",
            "Panelas de cobre pendem de um suporte no teto.",
            "A porta da geladeira esta entreaberta, revelando prateleiras vazias.",
            "Um calendario na parede esta parado em Outubro de 1987.",
            "O cheiro fraco de gas faz você verificar o fogao."},
        .itens = {"Faca de Cozinha", "Papel Aluminio", "Sal", "Feijao Enlatado", "Martelo de Carne"},
        .tiposItens = {"Ferramenta", "Utilidade", "Consumivel", "Consumivel", "Ferramenta"}
    };
    DADOS_COMODOS[6] = (ComodoData){
        .nome = "Escritorio Secreto",
        .frases = {
            "Em cima da mesa, um REVOLVER .38 antigo, descarregado.",
            "Uma caixa de MUNICAO .38 esta dentro de uma gaveta.",
            "Um maço de CARTAS amareladas, amarradas com uma fita.",
            "Um COFRE na parede requer uma combinacao.",
            "Uma garrafa de WHISKY pela metade esta ao lado de um copo sujo.",
            "Este comodo estava escondido atras de uma estante falsa.",
            "Um mapa na parede está cheio de alfinetes e anotacoes.",
            "A unica janela foi tapada com tabuas por dentro.",
            "Um cinzeiro esta cheio de cigarros apagados ha muito tempo.",
            "Ha um sentimento de paranoia e desespero neste lugar."},
        .itens = {"Revolver .38", "Municao .38", "Maco de Cartas", "Cofre", "Whisky"},
        .tiposItens = {"Defesa", "Municao", "Documento", "Enigma", "Consumivel"}
    };
    DADOS_COMODOS[7] = (ComodoData){
        .nome = "Despensa",
        .frases = {
            "Um KIT DE PRIMEIROS SOCORROS esta em uma prateleira.",
            "VELAS grossas estao em uma caixa de madeira.",
            "Um PE DE CABRA esta na parede.",
            "SACOS DE ARROZ E FEIJÃO, aparentemente ainda bons.",
            "Uma armadilha de urso (RATOEIRA GRANDE) esta no chao.",
            "Prateleiras estao cheias de potes de conserva vazios.",
            "O ar e frio e umido aqui.",
            "Ha um cheiro de terra molhada vindo de um canto escuro.",
            "Teias de aranha grossas como algodao pendem do teto.",
            "O som de animais arranhando as paredes é audivel."},
        .itens = {"Kit de Primeiros Socorros", "Velas", "Pe de cabra", "Sacos de Arroz e feijao", "Ratoeira Grande"},
        .tiposItens = {"Cura", "Utilidade", "Ferramenta", "Consumivel", "Armadilha"}
    };
}

/**
 * @brief Limpa o buffer de entrada do teclado (stdin).
 * @details Essencial após usar `scanf("%d", ...)` ou `scanf(" %c", ...)` para ler
 * números/caracteres. Essas funções deixam um caractere de nova linha ('\n')
 * no buffer, que pode ser lido acidentalmente pela próxima função de leitura
 * (como `getchar()` ou `fgets()`), causando comportamento inesperado.
 */
void limparBufferEntrada() {
    int c;
    // Lê e descarta cada caractere do buffer até encontrar uma nova linha ('\n') ou o fim do arquivo (EOF).
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Imprime o menu principal do jogo na tela.
 */
void exibirMenuPrincipal() {
    printf("\n\n#########################################\n");
    printf("        DETECTIVE QUEST - MENU\n");
    printf("#########################################\n");
    printf("1 - Novo Jogo\n");
    printf("2 - Continuar Exploracao\n");
    printf("3 - Carregar Jogo (Load Game)\n");
    printf("4 - Salvar Jogo (Save Game)\n");
    printf("5 - Resetar um Save\n");
    printf("6 - Creditos\n");
    printf("0 - Sair\n");
    printf("-----------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/**
 * @brief Função central que controla a exploração, o menu de ações e a lógica de itens.
 * @param estado Ponteiro para o estado atual do jogo.
 */
void entrarNoJogo(GameState *estado) {
    int opcao;
    char escolha;
    
    // O loop principal de exploração. Ele se redesenha a cada ação ou movimento.
    while(1) { // Usamos um loop infinito que só é quebrado pelo "return" da opção 0.
        int idComodoAtual = estado->posicaoAtual->comodoId;
        const char* nomeComodoAtual = DADOS_COMODOS[idComodoAtual].nome;
        // Verifica se o cômodo atual não tem saídas para a esquerda ou direita (folha da árvore).
        int semSaida = (estado->posicaoAtual->esquerda == NULL && estado->posicaoAtual->direita == NULL);

        // --- Interface do Jogo ---
        printf("\n\n--------------------------------------------\n");
        printf("Voce esta em: %s %s\n", nomeComodoAtual, semSaida ? "(Sem Saida)" : "");
        printf("Itens Coletados: %d/%d\n", estado->totalItensColetados, TOTAL_COMODOS * ITENS_POR_COMODO);
        printf("--------------------------------------------\n");
        printf("O que voce deseja fazer?\n");
        printf("1. Andar pelo Comodo\n");
        printf("2. Ver Mochila\n");
        printf("3. Mover-se para outro comodo\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha: ");
        
        while (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Por favor, digite um numero: ");
            limparBufferEntrada();
        }
        limparBufferEntrada();

        switch(opcao) {
            case 1: // Andar pelo Comodo (encontrar itens/frases)
                { // Chaves criam um novo escopo para declarar variáveis locais.
                    // Sorteia um número de 0 a 9 (índice do array de frases).
                    int indiceFrase = rand() % FRASES_POR_COMODO;
                    printf("\n> %s\n", DADOS_COMODOS[idComodoAtual].frases[indiceFrase]);

                    // As primeiras 5 frases (índices 0-4) correspondem aos 5 itens.
                    if (indiceFrase < ITENS_POR_COMODO) {
                        // Verifica se ESTE item específico NESTE cômodo já foi coletado.
                        if (estado->itensJaColetados[idComodoAtual][indiceFrase] == 0) {
                            printf("Deseja pegar o item? (s/n): ");
                            scanf(" %c", &escolha); // Espaço antes de %c ignora espaços/enters anteriores.
                            limparBufferEntrada();
                            if (escolha == 's' || escolha == 'S') {
                                adicionarItemNaMochila(&estado->mochila, 
                                                       DADOS_COMODOS[idComodoAtual].itens[indiceFrase], 
                                                       DADOS_COMODOS[idComodoAtual].tiposItens[indiceFrase]);
                                estado->itensJaColetados[idComodoAtual][indiceFrase] = 1; // Marca como coletado.
                                estado->totalItensColetados++;
                            } else {
                                printf("Voce deixou o item para tras.\n");
                            }
                        } else { // O item deste local já foi pego, então mostra uma frase genérica.
                            int indiceGlobal = rand() % TOTAL_FRASES_VAZIO;
                            printf("\n> %s\n", FRASES_GLOBAIS_VAZIO[indiceGlobal]);
                        }
                    }
                }
                break;
            case 2: // Ver Mochila
                menuMochila(estado);
                break;
            case 3: // Mover-se
                printf("\n--- Mover-se ---\n");
                // Mostra as opções de movimento apenas se os caminhos existirem (ponteiros não são NULL).
                if (estado->posicaoAtual->esquerda != NULL) printf(" (e) Esquerda -> %s\n", DADOS_COMODOS[estado->posicaoAtual->esquerda->comodoId].nome);
                if (estado->posicaoAtual->direita != NULL) printf(" (d) Direita  -> %s\n", DADOS_COMODOS[estado->posicaoAtual->direita->comodoId].nome);
                if (estado->posicaoAtual->pai != NULL) printf(" (v) Voltar   -> %s\n", DADOS_COMODOS[estado->posicaoAtual->pai->comodoId].nome);
                printf(" (c) Cancelar\n");
                printf("Escolha: ");
                scanf(" %c", &escolha);
                limparBufferEntrada();
                
                No* proximaPosicao = estado->posicaoAtual; // Começa assumindo que não vai se mover.
                switch(escolha) {
                    case 'e': case 'E':
                        if(estado->posicaoAtual->esquerda) proximaPosicao = estado->posicaoAtual->esquerda;
                        else printf("Caminho invalido.\n");
                        break;
                    case 'd': case 'D':
                        if(estado->posicaoAtual->direita) proximaPosicao = estado->posicaoAtual->direita;
                        else printf("Caminho invalido.\n");
                        break;
                    case 'v': case 'V':
                        if(estado->posicaoAtual->pai) proximaPosicao = estado->posicaoAtual->pai;
                        else printf("Caminho invalido.\n");
                        break;
                    default:
                        printf("Movimento cancelado.\n");
                }
                estado->posicaoAtual = proximaPosicao; // Atualiza a posição do jogador.
                break;
            case 0: // Voltar ao Menu
                printf("\nVoltando ao menu principal...\n");
                return; // Sai da função e do loop de exploração.
            default:
                printf("\nOpcao invalida.\n");
        }
        
        // Pausa para o jogador ler o resultado da sua ação antes de redesenhar a interface.
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
    }
}

/**
 * @brief Mostra os créditos do jogo.
 */
void mostrarCreditos() {
    printf("\n\n#####################################\n");
    printf("            DETECTIVE QUEST\n");
    printf("#########################################\n");
    printf("Jogo de exemplo usando Arvores Binarias\n");
    printf("Produtor: Thiago Marcel\n");
    printf("Orientador: Fabio Henrique Silva\n");
    printf("#########################################\n");
}

/**
 * @brief "Fábrica" de nós. Aloca memória para um novo cômodo e o inicializa.
 * @param comodoId O ID do cômodo a ser criado.
 * @return Um ponteiro para o novo nó (cômodo) alocado na memória.
 */
No* criarNo(int comodoId) {
    // Aloca memória do tamanho de um "No" e converte o ponteiro retornado para (No*).
    No* novo = (No*) malloc(sizeof(No));
    // Sempre verifique se a alocação de memória foi bem-sucedida.
    if (novo == NULL) {
        printf("Erro fatal: Falha ao alocar memoria para um novo comodo!\n");
        exit(1); // Encerra o programa se não houver memória.
    }
    // Inicializa os valores do novo nó.
    novo->comodoId = comodoId;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->pai = NULL;
    return novo;
}

/**
 * @brief Cria a estrutura ESTRUTURAL do mapa da casa, conectando os nós em uma árvore binária.
 * @return O ponteiro para a raiz da árvore (o Hall de Entrada).
 */
No* criarMapa() {
    // Nível 0 - Raiz
    No* raiz = criarNo(0); // Hall de Entrada
    // Nível 1
    raiz->esquerda = criarNo(1); // Sala de Estar
    raiz->esquerda->pai = raiz;
    raiz->direita = criarNo(2); // Sala de Jantar
    raiz->direita->pai = raiz;
    // Nível 2
    raiz->esquerda->esquerda = criarNo(3); // Biblioteca
    raiz->esquerda->esquerda->pai = raiz->esquerda;
    raiz->esquerda->direita = criarNo(4); // Varanda
    raiz->esquerda->direita->pai = raiz->esquerda;
    raiz->direita->esquerda = criarNo(5); // Cozinha
    raiz->direita->esquerda->pai = raiz->direita;
    // Nível 3
    raiz->esquerda->esquerda->direita = criarNo(6); // Escritorio Secreto
    raiz->esquerda->esquerda->direita->pai = raiz->esquerda->esquerda;
    raiz->direita->esquerda->esquerda = criarNo(7); // Despensa
    raiz->direita->esquerda->esquerda->pai = raiz->direita->esquerda;
    return raiz;
}

/**
 * @brief Libera (desaloca) toda a memória usada pela estrutura do mapa de forma recursiva.
 * @details Essencial para evitar vazamentos de memória. A função se chama para os filhos
 * (esquerda e direita) antes de se liberar.
 * @param raiz O nó a partir do qual a liberação deve começar.
 */
void liberarMapa(No* raiz) {
    if (raiz != NULL) { // Condição de parada da recursão.
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz); // Libera o nó atual APÓS liberar seus filhos.
    }
}

/**
 * @brief Inicia uma mochila, zerando sua contagem de itens.
 */
void iniciarMochila(MochilaEstatica *mochila) {
    mochila->quantidade = 0;
}

/**
 * @brief Adiciona um item à mochila do jogador.
 */
void adicionarItemNaMochila(MochilaEstatica *mochila, const char* nome, const char* tipo) {
    if (mochila->quantidade >= TAMANHO_MOCHILA) {
        printf("\nSua mochila esta cheia!\n");
        return; // Sai da função se não houver espaço.
    }
    // Copia as strings de nome e tipo para o próximo espaço livre na mochila.
    strcpy(mochila->dados[mochila->quantidade].nome, nome);
    strcpy(mochila->dados[mochila->quantidade].tipo, tipo);
    mochila->quantidade++; // Incrementa o contador de itens.
    printf("\nItem \"%s\" adicionado a mochila.\n", nome);
}

/**
 * @brief Exibe o conteúdo da mochila do jogador em formato de tabela.
 */
void verMochila(const MochilaEstatica *mochila) {
    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n", mochila->quantidade, TAMANHO_MOCHILA);
    printf("----------------------------------------\n");
    printf("%-25s | %-15s\n", "NOME", "TIPO"); // Formatação para alinhar as colunas.
    printf("----------------------------------------\n");
    if (mochila->quantidade == 0) {
        printf("Mochila vazia.\n");
    } else {
        for (int i = 0; i < mochila->quantidade; i++) {
            printf("%-25s | %-15s\n", mochila->dados[i].nome, mochila->dados[i].tipo);
        }
    }
    printf("----------------------------------------\n");
}

/**
 * @brief Ordena os itens da mochila por nome em ordem alfabética, usando o algoritmo Bubble Sort.
 */
void ordenarMochila(MochilaEstatica *mochila) {
    if (mochila->quantidade < 2) {
        printf("\nNao ha itens suficientes para ordenar.\n");
        return;
    }
    Item temp; // Variável temporária para a troca.
    // Bubble Sort: Compara pares de elementos adjacentes e os troca se estiverem na ordem errada.
    for (int i = 0; i < mochila->quantidade - 1; i++) {
        for (int j = 0; j < mochila->quantidade - i - 1; j++) {
            // strcmp retorna > 0 se a primeira string for "maior" (vier depois alfabeticamente).
            if (strcmp(mochila->dados[j].nome, mochila->dados[j + 1].nome) > 0) {
                // Troca os elementos de lugar.
                temp = mochila->dados[j];
                mochila->dados[j] = mochila->dados[j+1];
                mochila->dados[j+1] = temp;
            }
        }
    }
    printf("\nMochila ordenada por nome.\n");
}

/**
 * @brief Exibe o menu de gerenciamento da mochila (ver/ordenar).
 */
void menuMochila(GameState* estado) {
    int opcao;
    do {
        printf("\n--- Menu da Mochila ---\n");
        printf("1. Ver Itens\n");
        printf("2. Ordenar Itens por Nome\n");
        printf("0. Fechar Mochila\n");
        printf("Escolha: ");
        while (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Por favor, digite um numero: ");
            limparBufferEntrada();
        }
        limparBufferEntrada();

        switch(opcao) {
            case 1: verMochila(&estado->mochila); break;
            case 2: ordenarMochila(&estado->mochila); break;
            case 0: break; // Apenas sai do loop.
            default: printf("Opcao invalida.\n");
        }
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
    } while (opcao != 0);
}

// ##################################################################
// IMPLEMENTAÇÃO DAS FUNÇÕES DE SAVE / LOAD / RESET
// ##################################################################

/**
 * @brief Gerencia a interface para o usuário carregar um jogo.
 */
void gerenciarCarregamento(int *jogoIniciadoFlag, GameState *estado) {
    mostrarSlots();
    printf("Escolha o slot para carregar (0 para cancelar): ");
    int slot;
    while (scanf("%d", &slot) != 1) {
        printf("Entrada invalida. Digite um numero: ");
        limparBufferEntrada();
    }
    limparBufferEntrada();

    if (slot > 0 && slot <= MAX_SAVES) {
        if (*jogoIniciadoFlag && estado->raiz != NULL) {
            liberarMapa(estado->raiz);
            estado->raiz = NULL;
        }
        if (carregarJogo(slot, estado)) {
            *jogoIniciadoFlag = 1; // Marca que um jogo foi carregado com sucesso.
        }
    } else {
        printf("Carregamento cancelado.\n");
    }
}

/**
 * @brief Gerencia a interface para o usuário salvar o jogo.
 */
void gerenciarSalvamento(int jogoIniciadoFlag, GameState *estado) {
    if (!jogoIniciadoFlag) {
        printf("\nVoce precisa iniciar ou carregar um jogo para poder salvar!\n");
        return;
    }
    mostrarSlots();
    printf("Escolha o slot para salvar (0 para cancelar): ");
    int slot;
    while (scanf("%d", &slot) != 1) {
        printf("Entrada invalida. Digite um numero: ");
        limparBufferEntrada();
    }
    limparBufferEntrada();

    if (slot > 0 && slot <= MAX_SAVES) {
        salvarJogo(slot, estado);
    } else {
        printf("Salvamento cancelado.\n");
    }
}

/**
 * @brief Gerencia a interface para o usuário resetar um arquivo de save.
 */
void gerenciarReset() {
    mostrarSlots();
    printf("Escolha o slot para resetar (0 para cancelar): ");
    int slot;
    while (scanf("%d", &slot) != 1) {
        printf("Entrada invalida. Digite um numero: ");
        limparBufferEntrada();
    }
    limparBufferEntrada();

    if (slot > 0 && slot <= MAX_SAVES) {
        resetarSlot(slot);
    } else {
        printf("Reset cancelado.\n");
    }
}

/**
 * @brief Gera o nome do arquivo de save com base no número do slot.
 */
void getNomeArquivo(int slot, char* nomeArquivo) {
    sprintf(nomeArquivo, "save_detective_%d.dat", slot);
}

/**
 * @brief Lê os arquivos de save existentes e exibe o status de cada slot.
 */
void mostrarSlots() {
    printf("\n--- SLOTS DE JOGO ---\n");
    for (int i = 1; i <= MAX_SAVES; i++) {
        char nomeArquivo[30];
        getNomeArquivo(i, nomeArquivo);
        FILE* arquivo = fopen(nomeArquivo, "rb"); // "rb" = read binary (ler binário)
        if (arquivo == NULL) {
            printf("%d. [ Vazio ]\n", i);
        } else {
            SaveState save;
            fread(&save, sizeof(SaveState), 1, arquivo); // Lê a estrutura inteira do arquivo.
            fclose(arquivo);
            if(save.slotOcupado) { // Verifica se o save é válido
                printf("%d. %s (%s)\n", i, save.nomeSave, save.dataHora);
            } else {
                printf("%d. [ Vazio ]\n", i);
            }
        }
    }
    printf("-----------------------\n");
}

/**
 * @brief Salva o estado atual do jogo em um arquivo binário.
 */
void salvarJogo(int slot, GameState *estado) {
    SaveState save;
    save.slotOcupado = 1;
    save.estadoDoJogo = *estado;

    printf("Digite um nome para este save (max 49 chars): ");
    fgets(save.nomeSave, 50, stdin);
    save.nomeSave[strcspn(save.nomeSave, "\n")] = '\0'; // Remove o '\n' do final do fgets.

    time_t t = time(NULL);
    strftime(save.dataHora, 30, "%d/%m/%Y %H:%M", localtime(&t)); // Formata a data e hora.

    char nomeArquivo[30];
    getNomeArquivo(slot, nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo, "wb"); // "wb" = write binary (escrever binário)
    if (arquivo) {
        fwrite(&save, sizeof(SaveState), 1, arquivo); // Escreve a estrutura inteira no arquivo.
        fclose(arquivo);
        printf("\nJogo salvo com sucesso no Slot %d!\n", slot);
    } else {
        printf("\nErro ao abrir o arquivo para salvar!\n");
    }
}

// Função auxiliar para encontrar um nó na árvore com base no seu ID. Necessária para o carregamento.
No* encontrarNoPorId(No* noAtual, int id) {
    if (noAtual == NULL) return NULL; // Se chegou ao fim de um galho, não encontrou.
    if (noAtual->comodoId == id) return noAtual; // Encontrou!
    
    // Busca recursivamente na sub-árvore esquerda.
    No* encontrado = encontrarNoPorId(noAtual->esquerda, id);
    if (encontrado != NULL) return encontrado; // Se achou na esquerda, retorna.

    // Se não, busca na sub-árvore direita.
    return encontrarNoPorId(noAtual->direita, id);
}


/**
 * @brief Carrega um jogo salvo a partir de um arquivo binário.
 * @return 1 em caso de sucesso, 0 em caso de falha.
 */
int carregarJogo(int slot, GameState *estado) {
    char nomeArquivo[30];
    getNomeArquivo(slot, nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("\nSlot vazio ou nao encontrado!\n");
        return 0;
    }

    SaveState save;
    fread(&save, sizeof(SaveState), 1, arquivo);
    fclose(arquivo);
    
    if (!save.slotOcupado) {
        printf("\nEste slot de save esta corrompido ou vazio.\n");
        return 0;
    }

    // --- Estratégia de Carregamento Seguro ---
    // NÃO podemos usar os ponteiros salvos diretamente, pois os endereços de memória mudam a cada execução.
    // 1. Recriamos a estrutura do mapa do zero, que terá ponteiros válidos para a memória atual.
    estado->raiz = criarMapa();
    
    // 2. Copiamos os dados que NÃO são ponteiros (mochila, contadores, flags de itens).
    estado->mochila = save.estadoDoJogo.mochila;
    estado->totalItensColetados = save.estadoDoJogo.totalItensColetados;
    memcpy(estado->itensJaColetados, save.estadoDoJogo.itensJaColetados, sizeof(estado->itensJaColetados));

    // 3. Para restaurar a posição do jogador, pegamos o ID do cômodo salvo e o procuramos na
    //    árvore recém-criada para obter o ponteiro correto.
    estado->posicaoAtual = encontrarNoPorId(estado->raiz, save.estadoDoJogo.posicaoAtual->comodoId);
    if (estado->posicaoAtual == NULL) { // Segurança caso a busca falhe.
        estado->posicaoAtual = estado->raiz;
    }

    printf("\nJogo '%s' carregado com sucesso!\n", save.nomeSave);
    return 1;
}

/**
 * @brief Deleta um arquivo de save do disco, ou marca como vazio.
 */
void resetarSlot(int slot) {
    char nomeArquivo[30];
    getNomeArquivo(slot, nomeArquivo);
    
    // A forma mais segura de "resetar" é abrir o arquivo e marcar o slot como não ocupado.
    // Deletar o arquivo (com remove()) também funciona, mas pode gerar erros se o arquivo não existir.
    FILE* arquivo = fopen(nomeArquivo, "rb+"); // "rb+" = ler e escrever binário, sem apagar o conteúdo.
    if(arquivo == NULL) {
        printf("\nSlot %d ja esta vazio.\n", slot);
        return;
    }

    SaveState save;
    // Lê o save para não corromper o resto do arquivo (embora aqui só tenhamos uma struct).
    fread(&save, sizeof(SaveState), 1, arquivo); 
    save.slotOcupado = 0; // Marca o slot como vazio.
    strcpy(save.nomeSave, "[ Vazio ]");

    fseek(arquivo, 0, SEEK_SET); // Volta para o início do arquivo.
    fwrite(&save, sizeof(SaveState), 1, arquivo); // Escreve a struct modificada.
    fclose(arquivo);

    printf("\nSlot %d resetado com sucesso!\n", slot);
}

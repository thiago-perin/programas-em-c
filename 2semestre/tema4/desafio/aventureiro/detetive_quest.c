// ========================================================================================
// JOGO: DETECTIVE QUEST
// DESCRIÇÃO: Um jogo de exploração em uma casa misteriosa usando uma árvore binária
//            para representar o mapa. O jogador navega pelos cômodos, encontra
//            itens e desvenda o mistério de uma das cinco missões disponíveis.
// VERSÃO: 2.1 - Desafio Final (Código 100% Comentado)
// ========================================================================================

// ========================================================================================
// BIBLIOTECAS (INCLUDES)
// ========================================================================================
// NOTA DE APRENDIZADO: As bibliotecas nos dão acesso a funções prontas. Incluí-las
// é o primeiro passo para construir nosso programa.
#include <stdio.h>      // Inclui a biblioteca de Entrada e Saída Padrão, para usar funções como printf e scanf.
#include <stdlib.h>     // Inclui a biblioteca Padrão, essencial para alocação de memória (malloc, free) e números aleatórios (rand, srand).
#include <string.h>     // Inclui a biblioteca de Strings, para manipular texto com funções como strcpy e strcmp.
#include <time.h>       // Inclui a biblioteca de Tempo, usada aqui para inicializar o gerador de números aleatórios de forma única.

// ========================================================================================
// DEFINIÇÕES GLOBAIS (CONSTANTES)
// ========================================================================================
// NOTA DE APRENDIZADO: Usar #define para criar constantes (apelidos para valores) torna o código
// mais legível e muito mais fácil de manter.
#define MAX_SAVES 5             // Define uma constante para o número máximo de slots de salvamento.
#define TOTAL_COMODOS 8         // Define o número total de cômodos que nosso mapa terá.
#define ITENS_POR_COMODO 5      // Define quantos itens únicos podem ser encontrados em cada cômodo.
#define TOTAL_FRASES_VAZIO 40   // Define a quantidade de frases de ambiente genéricas para dar sabor ao jogo.
#define TAMANHO_MOCHILA 50      // Define a capacidade máxima de itens na mochila do jogador.
#define TOTAL_MISSOES 5         // Define o número total de missões que o jogador pode escolher.
#define ID_COMODO_COFRE 6       // Define o ID fixo do cômodo "Escritório Secreto", onde o cofre estará.
#define ID_COMODO_CHAVE 7       // Define o ID fixo do cômodo "Despensa", onde a chave do cofre estará.

// ========================================================================================
// ESTRUTURAS DE DADOS (STRUCTS)
// ========================================================================================
// NOTA DE APRENDIZADO: Structs são como "moldes" para criar nossos próprios tipos de variáveis
// que agrupam diferentes informações. A ordem delas é importante, pois não podemos
// usar um molde que ainda não foi definido.

/**
 * @struct Item
 * @brief Define a estrutura de um item que pode ser guardado na mochila.
 */
typedef struct {                                // Inicia a definição da nossa estrutura de Item.
    char nome[50];                              // Declara um campo 'nome' para o item, com espaço para até 49 caracteres.
    char tipo[50];                              // Declara um campo 'tipo' para categorizar o item (ex: Ferramenta, Documento).
} Item;                                         // Finaliza a definição e dá o nome "Item" à nossa nova estrutura.

/**
 * @struct MochilaEstatica
 * @brief Define a estrutura da mochila do jogador.
 */
typedef struct {                                // Inicia a definição da estrutura da Mochila.
    Item dados[TAMANHO_MOCHILA];                // Declara um array (vetor) de Itens, usando a constante que definimos para o tamanho.
    int quantidade;                             // Declara um contador para sabermos quantos itens temos atualmente na mochila.
} MochilaEstatica;                              // Finaliza a definição e nomeia a estrutura como "MochilaEstatica".

/**
 * @struct No
 * @brief Estrutura de um Nó da árvore, que representa um cômodo ESTRUTURAL no mapa.
 */
typedef struct No {                             // Inicia a definição da estrutura do Nó do mapa.
    int comodoId;                               // Armazena o ID do cômodo, que usaremos para buscar seu nome e conteúdo no nosso "banco de dados".
    struct No* esquerda;                        // Ponteiro para o nó filho da esquerda (o próximo cômodo nesse caminho).
    struct No* direita;                         // Ponteiro para o nó filho da direita.
    struct No* pai;                             // Ponteiro para o nó pai, essencial para a função "voltar".
} No;                                           // Finaliza a definição e nomeia como "No".

/**
 * @struct NoPista
 * @brief Estrutura de um Nó da Árvore de Busca Binária (BST) para as pistas.
 */
typedef struct NoPista {                        // Inicia a definição da estrutura do Nó de Pista.
    char pista[100];                            // Armazena o texto da pista encontrada.
    struct NoPista* esquerda;                   // Ponteiro para a sub-árvore de pistas que vêm antes alfabeticamente.
    struct NoPista* direita;                    // Ponteiro para a sub-árvore de pistas que vêm depois alfabeticamente.
} NoPista;                                      // Finaliza a definição e nomeia como "NoPista".

/**
 * @struct GameState
 * @brief Armazena todo o estado dinâmico do jogo que precisa ser salvo e carregado.
 */
typedef struct {                                // Inicia a definição da estrutura que guardará todo o estado do jogo.
    No* raiz;                                   // Ponteiro para o início do mapa (o primeiro cômodo).
    No* posicaoAtual;                           // Ponteiro para o cômodo onde o jogador está no momento.
    MochilaEstatica mochila;                    // Variável do tipo MochilaEstatica para guardar os itens do jogador.
    int itensJaColetados[TOTAL_COMODOS][ITENS_POR_COMODO]; // Matriz para marcar quais itens já foram coletados e não aparecerem de novo.
    int totalItensColetados;                    // Contador geral de itens para mostrar o progresso.

    // --- Campos de Missão e Pistas ---
    NoPista* raizPistas;                        // Ponteiro para a raiz da nossa árvore de pistas.
    int missaoAtualId;                          // Armazena o ID da missão que o jogador escolheu (0 a 4).
    int chanceDePista;                          // Armazena a chance (em %) de encontrar uma pista, baseada na dificuldade da missão.
    int pistasJaColetadas[TOTAL_COMODOS];       // Vetor para marcar quais pistas já foram coletadas.
    int totalPistasColetadas;                   // Contador de pistas para a condição de vitória.

    // --- Campos para o Quebra-Cabeça do Cofre ---
    int jogadorPossuiChaveDoCofre;              // Flag (0 ou 1) que indica se encontramos a pista que funciona como chave.
    int cofreAberto;                            // Flag (0 ou 1) que indica se o cofre no escritório já foi aberto.
} GameState;                                    // Finaliza a definição e nomeia como "GameState".

/**
 * @struct SaveState
 * @brief Estrutura completa que é escrita e lida dos arquivos de save.
 */
typedef struct {                                // Inicia a definição da estrutura que será salva em arquivo.
    int slotOcupado;                            // Flag para sabermos se um slot de save tem dados válidos ou está vazio.
    char nomeSave[50];                          // Armazena o nome que o jogador deu ao salvamento.
    char dataHora[30];                          // Armazena a data e hora em que o jogo foi salvo.
    GameState estadoDoJogo;                     // Aninha a estrutura GameState aqui, para salvar todo o progresso de uma vez.
} SaveState;                                    // Finaliza a definição e nomeia como "SaveState".

// ========================================================================================
// BANCO DE DADOS GLOBAL DO JOGO
// ========================================================================================
// NOTA DE APRENDIZADO: Manter os dados (textos) separados da lógica (código) é uma prática
// de programação muito importante, chamada de "separação de interesses".

// Estrutura para armazenar o CONTEÚDO de um cômodo.
typedef struct {                                // Define um molde para guardar o conteúdo de cada cômodo.
    const char* nome;                           // Nome do cômodo.
    const char* frases[ITENS_POR_COMODO];       // Frases que revelam os itens.
    const char* itens[ITENS_POR_COMODO];        // Nomes dos itens.
    const char* tiposItens[ITENS_POR_COMODO];   // Tipos dos itens.
} ComodoData;                                   // Nomeia a estrutura como "ComodoData".
ComodoData DADOS_COMODOS[TOTAL_COMODOS];        // Declara o array global que servirá como nosso "banco de dados" para os cômodos.

// Estrutura para armazenar o CONTEÚDO de uma Missão.
typedef struct {                                // Define um molde para guardar as informações de cada missão.
    const char* nome;                           // Nome da missão.
    const char* dificuldade;                    // Texto da dificuldade (ex: "Fácil").
    const char* historia;                       // Texto que conta a história da missão.
    int chance;                                 // Valor numérico da chance de encontrar pistas.
} MissaoData;                                   // Nomeia a estrutura como "MissaoData".
MissaoData DADOS_MISSOES[TOTAL_MISSOES];        // Declara o array global para guardar os dados das missões.

// "Banco de dados" de Pistas. Agora um array 2D: [id_da_missao][id_do_comodo]
const char* DADOS_PISTAS_MISSOES[TOTAL_MISSOES][TOTAL_COMODOS]; // Declara o array 2D para as pistas de cada missão.

// Array global com frases genéricas para quando o jogador explora um local onde o item já foi pego.
const char* FRASES_GLOBAIS_VAZIO[TOTAL_FRASES_VAZIO] = { // Inicia a declaração e inicialização do array de frases de ambiente.
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
}; // Finaliza a inicialização do array.

// ========================================================================================
// PROTÓTIPOS DAS FUNÇÕES
// ========================================================================================
// NOTA DE APRENDIZADO: Protótipos são como um "índice" para o compilador. Declaramos
// aqui todas as funções que vamos usar, para que possamos chamá-las em qualquer
// ordem no código, sem que o compilador reclame que não as conhece.

// --- Funções de Inicialização e Utilitárias ---
void inicializarDadosDoJogo();                   // Protótipo da função que carrega todos os textos e dados do jogo na memória.
void limparBufferEntrada();                     // Protótipo da nossa função utilitária para limpar o buffer de entrada do teclado.

// --- Funções de Menu e Jogo ---
void exibirMenuPrincipal();                     // Protótipo da função que imprime o menu principal na tela.
void selecionarMissao(GameState* estado, int *jogoIniciado); // Protótipo da função para o submenu de seleção de missão.
void entrarNoJogo(GameState *estado);           // Protótipo da função principal do loop de exploração do jogo.
void mostrarCreditos();                         // Protótipo da função que exibe os créditos.

// --- Funções do Mapa (Árvore Binária) ---
No* criarNo(int comodoId);                      // Protótipo da função que cria um novo nó (cômodo) para o mapa.
No* criarMapa();                                // Protótipo da função que monta a estrutura completa do mapa.
void liberarMapa(No* raiz);                     // Protótipo da função que libera a memória alocada para o mapa.
No* encontrarNoPorId(No* noAtual, int id);      // Protótipo da função auxiliar para encontrar um nó pelo seu ID.

// --- Funções da Mochila ---
void iniciarMochila(MochilaEstatica *mochila);   // Protótipo da função que prepara a mochila para um novo jogo.
void adicionarItemNaMochila(MochilaEstatica *mochila, const char* nome, const char* tipo); // Protótipo da função para adicionar um item.
void verMochila(const MochilaEstatica *mochila); // Protótipo da função que exibe os itens da mochila.
void ordenarMochila(MochilaEstatica *mochila);  // Protótipo da função que ordena os itens da mochila.

// --- Funções das Pistas (Árvore Binária de Busca) ---
NoPista* criarNoPista(const char* pista);       // Protótipo da função que cria um novo nó para a árvore de pistas.
NoPista* inserirPista(NoPista* raiz, const char* pista); // Protótipo da função que insere uma pista na árvore, mantendo a ordem.
void mostrarPistas(NoPista* raiz);              // Protótipo da função que exibe todas as pistas em ordem alfabética.
void liberarPistas(NoPista* raiz);              // Protótipo da função que libera a memória alocada para a árvore de pistas.
void finalDoJogo(GameState* estado);            // Protótipo da função que mostra a tela de vitória.

// --- Funções de Arquivo (Save/Load/Reset) ---
void gerenciarCarregamento(int *jogoIniciadoFlag, GameState *estado); // Protótipo do menu de carregamento.
void gerenciarSalvamento(int jogoIniciadoFlag, GameState *estado); // Protótipo do menu de salvamento.
void gerenciarReset();                          // Protótipo do menu de reset de saves.
void getNomeArquivo(int slot, char* nomeArquivo); // Protótipo da função que gera o nome do arquivo de save.
void mostrarSlots();                            // Protótipo da função que exibe o status de todos os slots de save.
void salvarJogo(int slot, GameState *estado);   // Protótipo da função que efetivamente salva o jogo em um arquivo.
int carregarJogo(int slot, GameState *estado);  // Protótipo da função que carrega o jogo de um arquivo.
void resetarSlot(int slot);                     // Protótipo da função que reseta um slot de save.

// ========================================================================================
// FUNÇÃO PRINCIPAL (main)
// ========================================================================================
int main() {                                    // Ponto de entrada principal do programa. A execução sempre começa aqui.
    srand(time(NULL));                          // Semeia o gerador de números aleatórios usando o tempo atual, para que os resultados (rand()) sejam diferentes a cada execução.
    inicializarDadosDoJogo();                   // Chama a função de inicialização para carregar nosso "banco de dados" de textos na memória.

    int opcao;                                  // Declara uma variável para armazenar a escolha numérica do usuário no menu.
    int jogoIniciado = 0;                       // Declara e inicializa uma flag para controlar se um jogo está ativo (novo ou carregado).
    GameState estadoDoJogo;                     // Declara a variável principal que guardará todo o estado do jogo atual.
    estadoDoJogo.raiz = NULL;                   // Inicializa o ponteiro da raiz do mapa com NULL, uma boa prática para evitar lixo de memória.
    estadoDoJogo.raizPistas = NULL;             // Inicializa também o ponteiro da raiz de pistas com NULL.

    do {                                        // Inicia um loop 'do-while' para o menu principal, garantindo que ele execute pelo menos uma vez.
        exibirMenuPrincipal();                  // Chama a função que imprime as opções do menu na tela.
        
        while (scanf("%d", &opcao) != 1) {      // Inicia um loop de validação para garantir que a entrada do usuário seja um número inteiro.
            printf("Entrada invalida. Por favor, digite um numero: "); // Informa o usuário sobre a entrada inválida.
            limparBufferEntrada();              // Chama nossa função utilitária para limpar o buffer de entrada (letras, etc.) e evitar loops infinitos.
        }                                       // Finaliza o loop de validação de entrada.
        limparBufferEntrada();                  // Limpa o caractere '\n' (Enter) que sobrou no buffer após a leitura do número.

        switch (opcao) {                        // Inicia uma estrutura 'switch' para tratar a opção numérica escolhida pelo usuário.
            case 1:                             // Define o bloco de código a ser executado se a opção for 1.
                selecionarMissao(&estadoDoJogo, &jogoIniciado); // Chama a função que gerencia a seleção da missão e a criação de um novo jogo.
                printf("\nMissao selecionada! Escolha 'Continuar Exploracao' para comecar.\n"); // Dá feedback ao jogador.
                break;                          // Impede que a execução continue para o próximo case.
            case 2:                             // Define o bloco de código para a opção 2.
                if (jogoIniciado) {             // Verifica se um jogo foi iniciado ou carregado antes de permitir a continuação.
                    entrarNoJogo(&estadoDoJogo); // Entra no loop principal de exploração do jogo.
                } else {                        // Caso contrário...
                    printf("\nVoce precisa iniciar um 'Novo Jogo' ou 'Carregar um Jogo' primeiro!\n"); // Informa o jogador sobre o que fazer.
                }                               // Finaliza o bloco 'if-else'.
                break;                          // Finaliza o tratamento do case 2.
            case 3:                             // Define o bloco para a opção 3.
                gerenciarCarregamento(&jogoIniciado, &estadoDoJogo); // Chama a função que encapsula a lógica de carregamento.
                break;                          // Finaliza o tratamento do case 3.
            case 4:                             // Define o bloco para a opção 4.
                gerenciarSalvamento(jogoIniciado, &estadoDoJogo); // Chama a função que encapsula a lógica de salvamento.
                break;                          // Finaliza o tratamento do case 4.
            case 5:                             // Define o bloco para a opção 5.
                gerenciarReset();               // Chama a função que encapsula a lógica de resetar um save.
                break;                          // Finaliza o tratamento do case 5.
            case 6:                             // Define o bloco para a opção 6.
                mostrarCreditos();              // Chama a função que exibe os créditos.
                break;                          // Finaliza o tratamento do case 6.
            case 0:                             // Define o bloco para a opção 0.
                printf("\nSaindo de Detective Quest...\n"); // Exibe uma mensagem de despedida.
                break;                          // Finaliza o tratamento do case 0.
            default:                            // Bloco executado se nenhuma das opções acima for escolhida.
                printf("\nOpcao Invalida! Tente novamente.\n"); // Informa o usuário que a opção é inválida.
        }                                       // Finaliza a estrutura 'switch'.

        if (opcao > 2 && opcao < 6) {           // Verifica se a opção foi uma das que não têm pausa própria (Save/Load/Reset).
             printf("\nPressione Enter para voltar ao menu principal..."); // Pede ao usuário para pressionar Enter.
             getchar();                         // Aguarda a entrada de um caractere (o Enter) para pausar a tela.
        }                                       // Finaliza o bloco 'if'.
    } while (opcao != 0);                       // Define a condição de parada do loop 'do-while': ele continuará até que a opção seja 0.

    // --- Limpeza de Memória ---
    if (estadoDoJogo.raiz != NULL) {            // Verifica se o ponteiro do mapa não é nulo para evitar erros.
        liberarMapa(estadoDoJogo.raiz);         // Libera toda a memória alocada para a árvore do mapa.
    }                                           // Finaliza o bloco 'if'.
    if (estadoDoJogo.raizPistas != NULL) {      // Verifica se o ponteiro das pistas não é nulo.
        liberarPistas(estadoDoJogo.raizPistas); // Libera toda a memória alocada para a árvore de pistas.
    }                                           // Finaliza o bloco 'if'.
    return 0;                                   // Retorna 0 ao sistema operacional, indicando que o programa terminou com sucesso.
}                                               // Finaliza a função main.

// ========================================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ========================================================================================

/**
 * @brief Preenche o "banco de dados" global com todo o conteúdo textual do jogo.
 */
void inicializarDadosDoJogo() {                 // Inicia a implementação da função que preenche nossas estruturas de dados globais.
    // --- Conteúdo dos Cômodos ---
    // NOTA DE APRENDIZADO: A inicialização de structs com `.nome = "..."` é uma forma mais segura e legível
    // de atribuir valores, pois não depende da ordem dos campos na definição da struct.
    DADOS_COMODOS[0] = (ComodoData){.nome = "Hall de Entrada", .frases = {"Uma CHAVE DE FENDA repousa sobre uma pequena mesa.", "Voce nota um MAPA DA CASA amassado sob o capacho.", "Um ISQUEIRO de metal esta no chao.", "Uma garrafa de AGUA pela metade foi deixada para tras.", "Um pe de CABRA pequeno esta encostado na parede."}, .itens = {"Chave de Fenda", "Mapa da Casa", "Isqueiro", "Garrafa de Agua", "Pe de Cabra Pequeno"}, .tiposItens = {"Ferramenta", "Documento", "Utilidade", "Consumivel", "Ferramenta"}};
    DADOS_COMODOS[1] = (ComodoData){.nome = "Sala de Estar", .frases = {"Uma LANTERNA robusta esta sobre a lareira.", "Um rolo de FITA ADESIVA prateada esta no braco de uma poltrona.", "Voce encontra um CANIVETE SUICO.", "Uma BARRA DE CEREAL embalada esta na mesa de centro.", "Um par de LUVAS DE COURO está jogado no sofa."}, .itens = {"Lanterna", "Fita Adesiva", "Canivete Suico", "Barra de Cereal", "Luvas de Couro"}, .tiposItens = {"Utilidade", "Utilidade", "Ferramenta", "Consumivel", "Vestimenta"}};
    DADOS_COMODOS[2] = (ComodoData){.nome = "Sala de Jantar", .frases = {"Um abridor de garrafas (SACA-ROLHAS) esta sobre a mesa.", "Uma caixa de FOSFOROS quase cheia foi deixada ao lado de um candelabro.", "Um guardanapo de pano esconde um pequeno BLOCO DE NOTAS.", "Um conjunto de TALHERES DE PRATA parece valioso.", "Uma garrafa de VINHO ANTIGO, ainda lacrada, esta na cristaleira."}, .itens = {"Saca-rolhas", "Fosforos", "Bloco de Notas", "Talheres de Prata", "Vinho Antigo"}, .tiposItens = {"Ferramenta", "Utilidade", "Documento", "Valor", "Consumivel"}};
    DADOS_COMODOS[3] = (ComodoData){.nome = "Biblioteca", .frases = {"Uma LUPA com cabo de marfim esta sobre um livro aberto.", "Escondido atras de livros, voce encontra um DIARIO ANTIGO.", "Um abridor de cartas (FACA DE PAPEL) esta sobre a escrivaninha.", "Uma CANETA-TINTEIRO e um vidro de tinta seca estao em um canto.", "Uma CAIXA DE MUSICA empoeirada toca uma melodia fantasmagorica."}, .itens = {"Lupa", "Diario Antigo", "Faca de Papel", "Caneta-tinteiro", "Caixa de Musica"}, .tiposItens = {"Investigacao", "Documento", "Ferramenta", "Utilidade", "Curiosidade"}};
    DADOS_COMODOS[4] = (ComodoData){.nome = "Varanda", .frases = {"Um par de BINOCULOS foi deixado em uma cadeira de vime.", "Um BARBANTE resistente esta enrolado em um dos pilares.", "Um velho regador de metal (RECIPIENTE) esta em um canto.", "Um pacote de SEMENTES estranhas esta sobre o parapeito.", "Uma CHAVE PEQUENA E ORNAMENTADA esta no chao."}, .itens = {"Binoculos", "Barbante", "Recipiente", "Sementes Estranhas", "Chave Ornamentada"}, .tiposItens = {"Investigacao", "Utilidade", "Utilidade", "Curiosidade", "Chave"}};
    DADOS_COMODOS[5] = (ComodoData){.nome = "Cozinha", .frases = {"Uma FACA DE COZINHA afiada esta em um cepo.", "Um rolo de PAPEL ALUMINIO esta sobre o balcao.", "Um pote de SAL grosso esta ao lado do fogao.", "Uma lata de FEIJAO ENLATADO parece comestivel.", "Um MARTELO DE CARNE esta pendurado em um gancho."}, .itens = {"Faca de Cozinha", "Papel Aluminio", "Sal", "Feijao Enlatado", "Martelo de Carne"}, .tiposItens = {"Ferramenta", "Utilidade", "Consumivel", "Consumivel", "Ferramenta"}};
    DADOS_COMODOS[6] = (ComodoData){.nome = "Escritorio Secreto", .frases = {"Em cima da mesa, um REVOLVER .38 antigo, descarregado.", "Uma caixa de MUNICAO .38 esta dentro de uma gaveta.", "Um maço de CARTAS amareladas, amarradas com uma fita.", "Um COFRE na parede requer uma combinacao.", "Uma garrafa de WHISKY pela metade esta ao lado de um copo sujo."}, .itens = {"Revolver .38", "Municao .38", "Maco de Cartas", "Cofre", "Whisky"}, .tiposItens = {"Defesa", "Municao", "Documento", "Enigma", "Consumivel"}};
    DADOS_COMODOS[7] = (ComodoData){.nome = "Despensa", .frases = {"Um KIT DE PRIMEIROS SOCORROS esta em uma prateleira.", "VELAS grossas estao em uma caixa de madeira.", "Um PE DE CABRA esta na parede.", "SACOS DE ARROZ E FEIJAO, aparentemente ainda bons.", "Uma armadilha de urso (RATOEIRA GRANDE) esta no chao."}, .itens = {"Kit de Primeiros Socorros", "Velas", "Pe de cabra", "Sacos de Arroz e feijao", "Ratoeira Grande"}, .tiposItens = {"Cura", "Utilidade", "Ferramenta", "Consumivel", "Armadilha"}};

    // --- Conteúdo das Missões ---
    DADOS_MISSOES[0] = (MissaoData){
        .nome = "O Caso do Padeiro Desaparecido",
        .dificuldade = "Muito Facil",
        .historia = "O padeiro Sr. Abel sumiu. A ultima vez que foi visto, ia a esta casa buscar uma receita de familia. Encontre a receita para provar que ele esteve aqui.",
        .chance = 50};
    DADOS_MISSOES[1] = (MissaoData){
        .nome = "A Melodia Sileciosa",
        .dificuldade = "Facil",
        .historia = "Uma musicista famosa deixou uma composicao final que revelaria seu verdadeiro herdeiro. Encontre a partitura antes dos outros herdeiros.",
        .chance = 40};
    DADOS_MISSOES[2] = (MissaoData){
        .nome = "O Legado Roubado",
        .dificuldade = "Medio",
        .historia = "Um colecionador alega que um quadro rarissimo, seu por direito, foi escondido nesta casa por um rival. Encontre evidencias do quadro e da posse.",
        .chance = 30};
    DADOS_MISSOES[3] = (MissaoData){
        .nome = "O Pacto do Alquimista",
        .dificuldade = "Dificil",
        .historia = "Um cientista recluso desapareceu, deixando pesquisas perigosas. Uma farmaceutica quer a formula. Descubra a verdade antes deles.",
        .chance = 20};
    DADOS_MISSOES[4] = (MissaoData){
        .nome = "A Heranca do Fantasma",
        .dificuldade = "Super Dificil",
        .historia = "O ultimo dono da casa deixou um testamento complexo. Apenas reunindo todas as pistas voce podera provar que e o herdeiro digno da fortuna.",
        .chance = 10};

    // --- Conteúdo das Pistas por Missão ---
    // Missão 1: O Padeiro Desaparecido
    DADOS_PISTAS_MISSOES[0][0] = "Pegadas de Farinha";              // Pista para o Hall de Entrada (ID 0) na Missão 1 (ID 0).
    DADOS_PISTAS_MISSOES[0][1] = "Jornal com Receita Circulada";    // Pista para a Sala de Estar (ID 1) na Missão 1.
    DADOS_PISTAS_MISSOES[0][2] = "Prato Vazio com Migalhas";         // Pista para a Sala de Jantar (ID 2) na Missão 1.
    DADOS_PISTAS_MISSOES[0][3] = "Livro de Culinaria Antigo";        // Pista para a Biblioteca (ID 3) na Missão 1.
    DADOS_PISTAS_MISSOES[0][4] = "Vaso Quebrado com Ervas";          // Pista para a Varanda (ID 4) na Missão 1.
    DADOS_PISTAS_MISSOES[0][5] = "Forno Ainda Morno";                // Pista para a Cozinha (ID 5) na Missão 1.
    DADOS_PISTAS_MISSOES[0][ID_COMODO_COFRE] = "A Receita Secreta";  // PISTA FINAL, localizada no cômodo do cofre.
    DADOS_PISTAS_MISSOES[0][ID_COMODO_CHAVE] = "Chave Enferrujada";  // PISTA CHAVE, localizada no cômodo da chave.
    // Missão 2: A Melodia Silenciosa
    DADOS_PISTAS_MISSOES[1][0] = "Estojo de Violino Vazio";
    DADOS_PISTAS_MISSOES[1][1] = "Metronomo Travado";
    DADOS_PISTAS_MISSOES[1][2] = "Taca de Vinho Meio Cheia";
    DADOS_PISTAS_MISSOES[1][3] = "Biografia de um Compositor Famoso";
    DADOS_PISTAS_MISSOES[1][4] = "Partitura Inacabada";
    DADOS_PISTAS_MISSOES[1][5] = "Xicara de Cha Fria";
    DADOS_PISTAS_MISSOES[1][ID_COMODO_COFRE] = "A Partitura Final";
    DADOS_PISTAS_MISSOES[1][ID_COMODO_CHAVE] = "Chave com Clave de Sol";
    // Missão 3: O Legado Roubado
    DADOS_PISTAS_MISSOES[2][0] = "Recibo de uma Moldura";
    DADOS_PISTAS_MISSOES[2][1] = "Marca de um Quadro na Parede";
    DADOS_PISTAS_MISSOES[2][2] = "Guardanapo com Anotacoes";
    DADOS_PISTAS_MISSOES[2][3] = "Livro sobre Falsificacao de Arte";
    DADOS_PISTAS_MISSOES[2][4] = "Fragmento de Tinta a Oleo";
    DADOS_PISTAS_MISSOES[2][5] = "Pincel Esquecido";
    DADOS_PISTAS_MISSOES[2][ID_COMODO_COFRE] = "Certificado de Autenticidade";
    DADOS_PISTAS_MISSOES[2][ID_COMODO_CHAVE] = "Chave Pesada";
    // Missão 4: O Pacto do Alquimista
    DADOS_PISTAS_MISSOES[3][0] = "Carta da Farmaceutica";
    DADOS_PISTAS_MISSOES[3][1] = "Diagrama Molecular Rabiscado";
    DADOS_PISTAS_MISSOES[3][2] = "Residuo Quimico num Copo";
    DADOS_PISTAS_MISSOES[3][3] = "Livro de Alquimia com Anotacoes";
    DADOS_PISTAS_MISSOES[3][4] = "Plantas com Mutacao Estranha";
    DADOS_PISTAS_MISSOES[3][5] = "Balanca de Precisao";
    DADOS_PISTAS_MISSOES[3][ID_COMODO_COFRE] = "A Formula Perigosa";
    DADOS_PISTAS_MISSOES[3][ID_COMODO_CHAVE] = "Cartao de Acesso";
    // Missão 5: A Herança do Fantasma
    DADOS_PISTAS_MISSOES[4][0] = "Retrato com Olhos Arranhados";
    DADOS_PISTAS_MISSOES[4][1] = "Caixa de Musica que Toca Sozinha";
    DADOS_PISTAS_MISSOES[4][2] = "Velas Dispostas em Circulo";
    DADOS_PISTAS_MISSOES[4][3] = "Diario com Relatos Sobrenaturais";
    DADOS_PISTAS_MISSOES[4][4] = "Simbolo Arcano Desenhado no Chao";
    DADOS_PISTAS_MISSOES[4][5] = "Adaga Ritualistica";
    DADOS_PISTAS_MISSOES[4][ID_COMODO_COFRE] = "Pagina Final do Testamento";
    DADOS_PISTAS_MISSOES[4][ID_COMODO_CHAVE] = "Chave de Esqueleto";
} // Finaliza a função inicializarDadosDoJogo.

/**
 * @brief Limpa o buffer de entrada do teclado (stdin).
 */
void limparBufferEntrada() {                 // Inicia a implementação da função.
    int c;                                   // Declara uma variável do tipo int para armazenar cada caractere.
    while ((c = getchar()) != '\n' && c != EOF); // Lê e descarta cada caractere do buffer até encontrar uma nova linha ('\n') ou o fim do arquivo (EOF).
}                                            // Finaliza a função.

/**
 * @brief Imprime o menu principal do jogo na tela.
 */
void exibirMenuPrincipal() {                  // Inicia a implementação da função.
    printf("\n\n#########################################\n"); // Imprime o cabeçalho superior do menu.
    printf("        DETECTIVE QUEST - MENU\n");      // Imprime o título do menu.
    printf("#########################################\n"); // Imprime uma linha separadora.
    printf("1 - Novo Jogo\n");                   // Imprime a opção 1.
    printf("2 - Continuar Exploracao\n");        // Imprime a opção 2.
    printf("3 - Carregar Jogo\n");               // Imprime a opção 3.
    printf("4 - Salvar Jogo\n");                // Imprime a opção 4.
    printf("5 - Resetar um Save\n");             // Imprime a opção 5.
    printf("6 - Creditos\n");                   // Imprime a opção 6.
    printf("0 - Sair\n");                       // Imprime a opção 0.
    printf("-----------------------------------------\n"); // Imprime uma linha separadora.
    printf("Escolha uma opcao: ");              // Pede a entrada do usuário.
}                                             // Finaliza a função.

/**
 * @brief Exibe o menu de seleção de missão e inicializa um novo estado de jogo.
 */
void selecionarMissao(GameState* estado, int *jogoIniciado) { // Inicia a implementação da função.
    int opcao;                                  // Variável para a escolha do jogador.
    printf("\n\n#########################################\n"); // Imprime o cabeçalho.
    printf("        SELECIONE SUA MISSAO\n");      // Imprime o título.
    printf("#########################################\n"); // Imprime uma linha separadora.
    for (int i = 0; i < TOTAL_MISSOES; i++) {   // Inicia um loop para exibir todas as missões.
        printf("%d - %s (%s)\n", i + 1, DADOS_MISSOES[i].nome, DADOS_MISSOES[i].dificuldade); // Exibe o número, nome e dificuldade.
        printf("    %s\n\n", DADOS_MISSOES[i].historia); // Exibe a história da missão.
    }                                           // Finaliza o loop 'for'.
    printf("0 - Voltar\n");                     // Exibe a opção de voltar.
    printf("-----------------------------------------\n"); // Imprime uma linha separadora.
    printf("Escolha uma missao: ");             // Pede a entrada do usuário.

    while (scanf("%d", &opcao) != 1 || opcao < 0 || opcao > TOTAL_MISSOES) { // Valida a entrada.
        printf("Entrada invalida. Por favor, digite um numero entre 0 e %d: ", TOTAL_MISSOES); // Pede novamente.
        limparBufferEntrada();                  // Limpa o buffer.
    }                                           // Finaliza a validação.
    limparBufferEntrada();                      // Limpa o '\n' do buffer.
    
    if (opcao == 0) {                           // Verifica se o jogador escolheu cancelar.
        printf("Selecao de missao cancelada.\n"); // Informa o cancelamento.
        return;                                 // Retorna ao menu principal.
    }                                           // Finaliza o 'if'.
    
    int idMissao = opcao - 1;                   // Converte a opção (1-5) para um ID de array (0-4).

    // --- RESET COMPLETO DO ESTADO DO JOGO ---
    if (*jogoIniciado) {                        // Se um jogo já estava em andamento...
        liberarMapa(estado->raiz);              // Libera a memória do mapa antigo.
        liberarPistas(estado->raizPistas);      // Libera a memória das pistas antigas.
    }                                           // Finaliza o 'if'.

    estado->raiz = criarMapa();                 // Cria uma nova estrutura de mapa.
    estado->posicaoAtual = estado->raiz;        // Define a posição inicial como o início do mapa.
    estado->missaoAtualId = idMissao;           // Salva o ID da missão escolhida no estado do jogo.
    estado->chanceDePista = DADOS_MISSOES[idMissao].chance; // Define a chance de pista com base na dificuldade.

    iniciarMochila(&estado->mochila);           // Zera a mochila.
    estado->totalItensColetados = 0;            // Zera o contador de itens.
    estado->raizPistas = NULL;                  // Define a raiz da árvore de pistas como nula.
    estado->totalPistasColetadas = 0;           // Zera o contador de pistas.
    estado->jogadorPossuiChaveDoCofre = 0;      // Zera a flag da chave para a nova missão.
    estado->cofreAberto = 0;                    // Zera a flag do cofre para a nova missão.
    
    for(int i = 0; i < TOTAL_COMODOS; i++) {    // Inicia um loop para percorrer todos os cômodos.
        estado->pistasJaColetadas[i] = 0;       // Zera a flag de pista coletada para este cômodo.
        for (int j = 0; j < ITENS_POR_COMODO; j++) { // Inicia um loop aninhado para os itens.
            estado->itensJaColetados[i][j] = 0; // Zera a flag de item coletado.
        }                                       // Finaliza o loop de itens.
    }                                           // Finaliza o loop de cômodos.
    *jogoIniciado = 1;                          // Ativa a flag principal, indicando que um jogo está pronto para começar.
}                                               // Finaliza a função.

/**
 * @brief Função central que controla a exploração, o menu de ações e a lógica de jogo.
 */
void entrarNoJogo(GameState *estado) {          // Inicia a implementação da função principal do jogo.
    int opcao;                                  // Variável para a escolha de ação do jogador.
    char escolha;                               // Variável para escolhas de 's'/'n' ou direção.
    
    while(1) {                                  // Inicia o loop infinito do jogo, que só é quebrado quando o jogador decide sair.
        int idComodoAtual = estado->posicaoAtual->comodoId; // Obtém o ID do cômodo atual para fácil acesso.
        const char* nomeComodoAtual = DADOS_COMODOS[idComodoAtual].nome; // Busca o nome do cômodo no banco de dados.
        int semSaida = (estado->posicaoAtual->esquerda == NULL && estado->posicaoAtual->direita == NULL); // Verifica se o cômodo é uma "folha" na árvore.

        // --- Interface do Jogo (HUD) ---
        printf("\n\n--------------------------------------------\n"); // Imprime uma linha separadora.
        printf("Voce esta em: %s %s\n", nomeComodoAtual, semSaida ? "(Sem Saida)" : ""); // Mostra a localização atual.
        printf("Itens Coletados: %d/%d | Pistas Encontradas: %d/%d\n", estado->totalItensColetados, TOTAL_COMODOS * ITENS_POR_COMODO, estado->totalPistasColetadas, TOTAL_COMODOS); // Mostra o progresso.
        printf("--------------------------------------------\n"); // Imprime uma linha separadora.
        printf("O que voce deseja fazer?\n");             // Pergunta ao jogador o que fazer.
        printf("1. Andar pelo Comodo (Procurar Itens/Pistas)\n"); // Opção 1.
        printf("2. Ver Mochila\n");                      // Opção 2.
        printf("3. Ver Pistas\n");                       // Opção 3.
        printf("4. Mover-se para outro comodo\n");       // Opção 4.
        printf("0. Voltar ao Menu Principal\n");          // Opção 0.
        printf("Escolha: ");                              // Pede a entrada.
        
        while (scanf("%d", &opcao) != 1) {      // Valida a entrada para aceitar apenas números.
            printf("Entrada invalida. Por favor, digite um numero: "); // Informa o erro.
            limparBufferEntrada();              // Limpa o buffer.
        }                                       // Finaliza a validação.
        limparBufferEntrada();                  // Limpa o '\n'.

        switch(opcao) {                         // Inicia o switch para tratar a ação do jogador.
           case 1: {                            // Inicia o bloco para a opção 1 (Procurar no cômodo).
                    // --- Lógica de Encontrar Itens ---
                    int itemIndex = rand() % ITENS_POR_COMODO; // Sorteia um "ponto de interesse" no cômodo.
                    if (estado->itensJaColetados[idComodoAtual][itemIndex] == 0) { // Verifica se o item correspondente já foi coletado.
                        printf("\n%s\n", DADOS_COMODOS[idComodoAtual].frases[itemIndex]); // Se não, exibe a frase que revela o item.
                        printf("Pegar o item? (s/n): "); // Pergunta se o jogador quer pegar.
                        scanf(" %c", &escolha);         // Lê a resposta.
                        limparBufferEntrada();          // Limpa o buffer.
                        if (escolha == 's' || escolha == 'S') { // Se a resposta for sim...
                            adicionarItemNaMochila(&estado->mochila, DADOS_COMODOS[idComodoAtual].itens[itemIndex], DADOS_COMODOS[idComodoAtual].tiposItens[itemIndex]); // Adiciona o item.
                            estado->itensJaColetados[idComodoAtual][itemIndex] = 1; // Marca o item como coletado.
                            estado->totalItensColetados++; // Incrementa o contador.
                        } else {                        // Se a resposta for não...
                            printf("Voce decide deixar o item por enquanto.\n"); // Exibe uma mensagem.
                        }                               // Finaliza o if/else de pegar o item.
                    } else {                            // Se o item já foi pego...
                        int fraseVazioIndex = rand() % TOTAL_FRASES_VAZIO; // Sorteia uma frase de ambiente genérica.
                        printf("\n%s\n", FRASES_GLOBAIS_VAZIO[fraseVazioIndex]); // Exibe a frase.
                    }                                   // Finaliza o if/else de item já coletado.

                    // --- NOVA LÓGICA DE PISTAS E COFRE ---
                    if (estado->pistasJaColetadas[idComodoAtual] == 0) { // Verifica se a pista deste cômodo já foi encontrada.
                        int idMissao = estado->missaoAtualId; // Obtém o ID da missão atual para buscar a pista correta.
                        
                        // CASO 1: ESTAMOS NO ESCRITÓRIO SECRETO (ONDE ESTÁ O COFRE)
                        if (idComodoAtual == ID_COMODO_COFRE) { // Verifica se estamos no cômodo do cofre.
                            if (estado->jogadorPossuiChaveDoCofre) { // Se estivermos, verifica se já temos a chave.
                                printf("\nVoce usa a chave que encontrou e abre o cofre!\n"); // Mensagem de sucesso.
                                const char* pistaEncontrada = DADOS_PISTAS_MISSOES[idMissao][idComodoAtual]; // Pega a pista final no banco de dados.
                                printf("!!! Dentro, voce encontra a pista crucial: \"%s\" !!!\n", pistaEncontrada); // Exibe a pista encontrada.
                                estado->raizPistas = inserirPista(estado->raizPistas, pistaEncontrada); // Insere a pista na árvore.
                                estado->pistasJaColetadas[idComodoAtual] = 1; // Marca a pista como coletada.
                                estado->totalPistasColetadas++; // Incrementa o contador.
                                estado->cofreAberto = 1;    // Marca o cofre como aberto.
                            } else {                        // Se não tivermos a chave...
                                printf("\nVoce encontrou um cofre de aco na parede, mas esta trancado. Voce precisa de uma chave.\n"); // Informa o jogador.
                            }                               // Finaliza o if/else da chave.
                        } 
                        // CASO 2: NÃO ESTAMOS NO ESCRITÓRIO. LÓGICA NORMAL DE ENCONTRAR PISTAS.
                        else {                          // Se não for o cômodo do cofre...
                            if ((rand() % 100) < estado->chanceDePista) { // Rola o dado para ver se encontra a pista, com base na chance da missão.
                                const char* pistaEncontrada = DADOS_PISTAS_MISSOES[idMissao][idComodoAtual]; // Pega a pista correta no banco de dados.
                                printf("\n!!! Voce encontrou uma pista: \"%s\" !!!\n", pistaEncontrada); // Exibe a pista.
                                estado->raizPistas = inserirPista(estado->raizPistas, pistaEncontrada); // Insere na árvore.
                                estado->pistasJaColetadas[idComodoAtual] = 1; // Marca como coletada.
                                estado->totalPistasColetadas++; // Incrementa o contador.

                                if (idComodoAtual == ID_COMODO_CHAVE) { // Após encontrar, verifica se esta era a pista da chave.
                                    printf("Isso parece ser o que voce precisa para abrir o cofre!\n"); // Dá uma dica ao jogador.
                                    estado->jogadorPossuiChaveDoCofre = 1; // Ativa a flag que indica que temos a chave.
                                }                           // Finaliza o 'if' da chave.
                            }                               // Finaliza o 'if' da chance.
                        }                                   // Finaliza o if/else do cômodo.

                        if (estado->totalPistasColetadas == TOTAL_COMODOS) { // Verifica se todas as pistas foram encontradas.
                            finalDoJogo(estado);            // Se sim, chama a tela de vitória.
                            printf("\nPressione Enter para voltar ao menu principal..."); // Pede para o jogador continuar.
                            getchar();                      // Pausa a tela.
                            return;                         // Retorna ao menu principal, encerrando a exploração.
                        }                                   // Finaliza o 'if' de vitória.
                    }                                       // Finaliza o 'if' de pista já coletada.
                }                                           // Finaliza o escopo do case 1.
                break;                                      // Finaliza o case 1.
            case 2:                                         // Inicia o bloco para a opção 2.
                ordenarMochila(&estado->mochila);           // Primeiro, ordena silenciosamente a mochila.
                verMochila(&estado->mochila);               // Depois, exibe a mochila já ordenada.
                break;                                      // Finaliza o case 2.
            case 3:                                         // Inicia o bloco para a opção 3.
                printf("\n--- PISTAS COLETADAS ---\n");      // Imprime um cabeçalho.
                if (estado->raizPistas == NULL) {           // Verifica se a árvore de pistas está vazia.
                    printf("Nenhuma pista encontrada ainda.\n"); // Se sim, informa o jogador.
                } else {                                    // Se não...
                    mostrarPistas(estado->raizPistas);      // Chama a função que exibe as pistas em ordem.
                }                                           // Finaliza o if/else.
                printf("--------------------------\n");      // Imprime um rodapé.
                break;                                      // Finaliza o case 3.
            case 4:                                         // Inicia o bloco para a opção 4.
                printf("\n--- Mover-se ---\n");              // Imprime um cabeçalho.
                if (estado->posicaoAtual->esquerda != NULL) printf(" (e) Esquerda -> %s\n", DADOS_COMODOS[estado->posicaoAtual->esquerda->comodoId].nome); // Mostra a opção de ir para esquerda, se existir.
                if (estado->posicaoAtual->direita != NULL) printf(" (d) Direita  -> %s\n", DADOS_COMODOS[estado->posicaoAtual->direita->comodoId].nome);  // Mostra a opção de ir para direita, se existir.
                if (estado->posicaoAtual->pai != NULL) printf(" (v) Voltar   -> %s\n", DADOS_COMODOS[estado->posicaoAtual->pai->comodoId].nome);    // Mostra a opção de voltar, se existir.
                printf(" (c) Cancelar\n");                  // Mostra a opção de cancelar.
                printf("Escolha: ");                          // Pede a entrada.
                scanf(" %c", &escolha);                     // Lê a direção.
                limparBufferEntrada();                      // Limpa o buffer.
                
                No* proximaPosicao = estado->posicaoAtual;  // Cria um ponteiro temporário, inicialmente na mesma posição.
                switch(escolha) {                           // Inicia o switch para a direção.
                    case 'e': case 'E': if(estado->posicaoAtual->esquerda) proximaPosicao = estado->posicaoAtual->esquerda; else printf("Caminho invalido.\n"); break; // Move para esquerda se o caminho existir.
                    case 'd': case 'D': if(estado->posicaoAtual->direita) proximaPosicao = estado->posicaoAtual->direita; else printf("Caminho invalido.\n"); break;  // Move para direita se o caminho existir.
                    case 'v': case 'V': if(estado->posicaoAtual->pai) proximaPosicao = estado->posicaoAtual->pai; else printf("Caminho invalido.\n"); break;      // Volta para o cômodo pai se existir.
                    default: printf("Movimento cancelado.\n"); // Cancela para qualquer outra tecla.
                }                                           // Finaliza o switch de direção.
                estado->posicaoAtual = proximaPosicao;      // Atualiza a posição real do jogador.
                break;                                      // Finaliza o case 4.
            case 0:                                         // Inicia o bloco para a opção 0.
                printf("\nVoltando ao menu principal...\n"); // Exibe uma mensagem.
                return;                                     // Sai da função e do loop de exploração, voltando ao menu.
            default:                                        // Bloco para qualquer outra opção numérica.
                printf("\nOpcao invalida.\n");             // Informa o erro.
        }                                                   // Finaliza o switch de ação.
        
        if (opcao != 0) {                                   // Se a opção não for sair...
            printf("\nPressione Enter para continuar..."); // Pede ao jogador para pressionar Enter.
            getchar();                                      // Pausa a execução, aguardando o Enter.
        }                                                   // Finaliza o 'if'.
    }                                                       // Finaliza o loop 'while(1)'.
}                                                           // Finaliza a função.

/**
 * @brief Mostra os créditos do jogo.
 */
void mostrarCreditos() {                                    // Inicia a implementação da função que exibe os créditos.
    printf("\n\n#####################################\n"); // Imprime o cabeçalho superior dos créditos.
    printf("            DETECTIVE QUEST\n");                 // Imprime o título do jogo.
    printf("#########################################\n"); // Imprime uma linha separadora.
    printf("Jogo de exemplo usando Arvores Binarias\n");     // Imprime a descrição do projeto.
    printf("Produtor: Thiago Marcel\n");                   // Imprime o nome do produtor.
    printf("Orientador: Fabio Henrique Silva\n");           // Imprime o nome do orientador.
    printf("#########################################\n"); // Imprime o rodapé dos créditos.
}                                                           // Finaliza a função mostrarCreditos.

/**
 * @brief "Fábrica" de nós. Aloca memória para um novo cômodo e o inicializa.
 * @param comodoId O ID do cômodo a ser criado.
 * @return Um ponteiro para o novo nó (cômodo) alocado na memória.
 */
No* criarNo(int comodoId) {                     // Inicia a implementação da função que cria um novo nó do mapa.
    // NOTA DE APRENDIZADO: malloc (memory allocation) é a função que usamos para pedir memória ao sistema operacional.
    No* novo = (No*) malloc(sizeof(No));        // Aloca um bloco de memória do tamanho exato da nossa struct 'No' e guarda o endereço no ponteiro 'novo'.
    if (novo == NULL) {                         // Verifica se a alocação de memória falhou (se o sistema não tinha memória para nos dar).
        printf("Erro fatal: Falha ao alocar memoria para um novo comodo!\n"); // Informa um erro crítico.
        exit(1);                                // Encerra o programa imediatamente, pois não é possível continuar sem memória.
    }                                           // Finaliza o bloco 'if'.
    novo->comodoId = comodoId;                  // Atribui o ID recebido como parâmetro ao campo 'comodoId' do novo nó.
    novo->esquerda = NULL;                      // Inicializa o ponteiro da esquerda com NULL, indicando que ainda não há um caminho para a esquerda.
    novo->direita = NULL;                       // Inicializa o ponteiro da direita com NULL.
    novo->pai = NULL;                           // Inicializa o ponteiro do pai com NULL.
    return novo;                                // Retorna o endereço de memória do nó recém-criado e inicializado.
}                                               // Finaliza a função criarNo.

/**
 * @brief Cria a estrutura ESTRUTURAL do mapa da casa, conectando os nós em uma árvore binária.
 * @return O ponteiro para a raiz da árvore (o Hall de Entrada).
 */
No* criarMapa() {                               // Inicia a implementação da função que monta nosso mapa.
    No* raiz = criarNo(0);                      // Cria o nó raiz da árvore (Hall de Entrada, ID 0).
    raiz->esquerda = criarNo(1); raiz->esquerda->pai = raiz; // Cria o nó da Sala de Estar (ID 1), o conecta à esquerda da raiz e aponta seu pai de volta para a raiz.
    raiz->direita = criarNo(2); raiz->direita->pai = raiz;   // Cria o nó da Sala de Jantar (ID 2), o conecta à direita da raiz e aponta seu pai de volta para a raiz.
    raiz->esquerda->esquerda = criarNo(3); raiz->esquerda->esquerda->pai = raiz->esquerda; // Cria e conecta a Biblioteca à esquerda da Sala de Estar.
    raiz->esquerda->direita = criarNo(4); raiz->esquerda->direita->pai = raiz->esquerda;   // Cria e conecta a Varanda à direita da Sala de Estar.
    raiz->direita->esquerda = criarNo(5); raiz->direita->esquerda->pai = raiz->direita;    // Cria e conecta a Cozinha à esquerda da Sala de Jantar.
    raiz->esquerda->esquerda->direita = criarNo(6); raiz->esquerda->esquerda->direita->pai = raiz->esquerda->esquerda; // Cria e conecta o Escritório Secreto à direita da Biblioteca.
    raiz->direita->esquerda->esquerda = criarNo(7); raiz->direita->esquerda->esquerda->pai = raiz->direita->esquerda; // Cria e conecta a Despensa à esquerda da Cozinha.
    return raiz;                                // Retorna o ponteiro para a raiz, que é o ponto de partida para navegar no mapa.
}                                               // Finaliza a função criarMapa.

/**
 * @brief Libera (desaloca) toda a memória usada pela estrutura do mapa de forma recursiva.
 */
void liberarMapa(No* raiz) {                    // Inicia a implementação da função para liberar a memória do mapa.
    // NOTA DE APRENDIZADO: Esta função usa recursão e percurso em pós-ordem. Ela primeiro vai até o
    // final de um galho (esquerda, depois direita) antes de liberar o nó atual. Isso evita que
    // a gente perca o "caminho" para os nós filhos antes de liberá-los.
    if (raiz != NULL) {                         // Condição de parada da recursão: se o nó é NULL, não faz nada.
        liberarMapa(raiz->esquerda);            // Chama a si mesma para a sub-árvore esquerda.
        liberarMapa(raiz->direita);             // Chama a si mesma para a sub-árvore direita.
        free(raiz);                             // Libera a memória do nó atual APÓS ter liberado seus filhos.
    }                                           // Finaliza o bloco 'if'.
}                                               // Finaliza a função liberarMapa.

/**
 * @brief Inicia uma mochila, zerando sua contagem de itens.
 */
void iniciarMochila(MochilaEstatica *mochila) {  // Inicia a implementação da função.
    mochila->quantidade = 0;                    // Define a quantidade de itens na mochila como 0.
}                                               // Finaliza a função.

/**
 * @brief Adiciona um item à mochila do jogador.
 */
void adicionarItemNaMochila(MochilaEstatica *mochila, const char* nome, const char* tipo) { // Inicia a implementação da função.
    if (mochila->quantidade >= TAMANHO_MOCHILA) { // Verifica se a mochila já está cheia.
        printf("\nSua mochila esta cheia!\n");  // Informa o jogador.
        return;                                 // Sai da função sem adicionar o item.
    }                                           // Finaliza o 'if'.
    strcpy(mochila->dados[mochila->quantidade].nome, nome); // Copia o nome do item para o próximo espaço livre na mochila.
    strcpy(mochila->dados[mochila->quantidade].tipo, tipo); // Copia o tipo do item.
    mochila->quantidade++;                      // Incrementa o contador de itens na mochila.
    printf("\nItem \"%s\" adicionado a mochila.\n", nome); // Dá feedback ao jogador.
}                                               // Finaliza a função.

/**
 * @brief Exibe o conteúdo da mochila do jogador em formato de tabela.
 */
void verMochila(const MochilaEstatica *mochila) { // Inicia a implementação da função.
    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n", mochila->quantidade, TAMANHO_MOCHILA); // Imprime o cabeçalho com a contagem atual.
    printf("----------------------------------------\n"); // Imprime uma linha separadora.
    printf("%-25s | %-15s\n", "NOME", "TIPO");   // Imprime os títulos das colunas. O `%-25s` alinha o texto à esquerda em um espaço de 25 caracteres.
    printf("----------------------------------------\n"); // Imprime uma linha separadora.
    if (mochila->quantidade == 0) {             // Verifica se a mochila está vazia.
        printf("Mochila vazia.\n");             // Se sim, informa o jogador.
    } else {                                    // Se não...
        for (int i = 0; i < mochila->quantidade; i++) { // Inicia um loop para percorrer todos os itens.
            printf("%-25s | %-15s\n", mochila->dados[i].nome, mochila->dados[i].tipo); // Imprime o nome e tipo de cada item, formatado.
        }                                       // Finaliza o loop.
    }                                           // Finaliza o if/else.
    printf("----------------------------------------\n"); // Imprime o rodapé da tabela.
}                                               // Finaliza a função.

/**
 * @brief Ordena os itens da mochila por nome em ordem alfabética (versão silenciosa).
 */
void ordenarMochila(MochilaEstatica *mochila) {  // Inicia a implementação da função de ordenação.
    if (mochila->quantidade < 2) {              // Verifica se há pelo menos dois itens para ordenar.
        return;                                 // Se não, sai da função, pois não há o que fazer.
    }                                           // Finaliza o 'if'.
    Item temp;                                  // Declara uma variável temporária do tipo Item para ajudar na troca.
    for (int i = 0; i < mochila->quantidade - 1; i++) { // Inicia o loop externo do Bubble Sort.
        for (int j = 0; j < mochila->quantidade - i - 1; j++) { // Inicia o loop interno para comparações.
            if (strcmp(mochila->dados[j].nome, mochila->dados[j + 1].nome) > 0) { // Compara os nomes de dois itens adjacentes.
                // Se o primeiro nome vem depois do segundo alfabeticamente, eles são trocados.
                temp = mochila->dados[j];           // Guarda o item j na variável temporária.
                mochila->dados[j] = mochila->dados[j+1]; // Coloca o item j+1 na posição j.
                mochila->dados[j+1] = temp;         // Coloca o item temporário (antigo j) na posição j+1.
            }                                   // Finaliza o 'if' da comparação.
        }                                       // Finaliza o loop interno.
    }                                           // Finaliza o loop externo.
}                                               // Finaliza a função.

/**
 * @brief Exibe a tela de conclusão do jogo.
 */
void finalDoJogo(GameState* estado) {           // Inicia a implementação da função de vitória.
    int idMissao = estado->missaoAtualId;       // Pega o ID da missão atual.
    printf("\n\n############################################################\n"); // Imprime o cabeçalho.
    printf("                  MISSAO CUMPRIDA!\n"); // Imprime a mensagem de vitória.
    printf("############################################################\n\n"); // Imprime uma linha.
    printf("Voce desvendou o misterio de: %s\n", DADOS_MISSOES[idMissao].nome); // Mostra qual missão foi completada.
    printf("Com sua astucia de detetive, voce reuniu todas as pistas necessarias.\n\n"); // Mensagem de congratulação.
    printf("--- PISTAS COLETADAS ---\n");        // Cabeçalho para a lista de pistas.
    mostrarPistas(estado->raizPistas);          // Chama a função para exibir as pistas em ordem.
    printf("\n--------------------------\n");      // Imprime um separador.
    printf("O caso esta encerrado. Parabens!\n"); // Mensagem final.
    printf("############################################################\n"); // Imprime o rodapé.
}                                               // Finaliza a função.

/**
 * @brief Aloca memória e cria um novo nó para a BST de pistas.
 */
NoPista* criarNoPista(const char* pista) {      // Inicia a implementação da "fábrica" de nós de pista.
    NoPista* novo = (NoPista*) malloc(sizeof(NoPista)); // Aloca memória do tamanho da struct NoPista.
    if (novo == NULL) {                         // Verifica se a alocação de memória falhou.
        printf("Erro fatal: Falha ao alocar memoria para uma nova pista!\n"); // Informa o erro.
        exit(1);                                // Encerra o programa.
    }                                           // Finaliza o 'if'.
    strcpy(novo->pista, pista);                 // Copia o texto da pista para o campo 'pista' do novo nó.
    novo->esquerda = NULL;                      // Inicializa o ponteiro da esquerda com NULL.
    novo->direita = NULL;                       // Inicializa o ponteiro da direita com NULL.
    return novo;                                // Retorna o endereço do novo nó.
}                                               // Finaliza a função.

/**
 * @brief Insere uma nova pista na BST, mantendo a ordem alfabética.
 */
NoPista* inserirPista(NoPista* raiz, const char* pista) { // Inicia a implementação da função de inserção na BST.
    if (raiz == NULL) {                         // Caso base da recursão: se a árvore (ou sub-árvore) está vazia...
        return criarNoPista(pista);             // Cria um novo nó e o retorna como a nova raiz.
    }                                           // Finaliza o 'if'.
    if (strcmp(pista, raiz->pista) < 0) {       // Compara a nova pista com a pista da raiz atual. Se a nova vem antes alfabeticamente...
        raiz->esquerda = inserirPista(raiz->esquerda, pista); // Chama a função recursivamente para a sub-árvore esquerda.
    } else if (strcmp(pista, raiz->pista) > 0) { // Se a nova pista vem depois alfabeticamente...
        raiz->direita = inserirPista(raiz->direita, pista); // Chama a função recursivamente para a sub-árvore direita.
    }                                           // Se for igual, não faz nada para evitar duplicatas.
    return raiz;                                // Retorna a raiz da árvore (ou sub-árvore) modificada.
}                                               // Finaliza a função.

/**
 * @brief Exibe as pistas em ordem alfabética (percurso in-order).
 */
void mostrarPistas(NoPista* raiz) {             // Inicia a implementação da função que mostra as pistas.
    // NOTA DE APRENDIZADO: Este é um percurso "in-order" (em-ordem). A recursão vai para a
    // esquerda, depois processa o nó atual (printf), e depois vai para a direita. Em uma BST,
    // isso garante que os elementos sejam processados em ordem crescente (alfabética).
    if (raiz != NULL) {                         // Condição de parada da recursão.
        mostrarPistas(raiz->esquerda);          // Visita toda a sub-árvore esquerda primeiro.
        printf("- %s\n", raiz->pista);          // Imprime o conteúdo do nó atual.
        mostrarPistas(raiz->direita);           // Visita toda a sub-árvore direita por último.
    }                                           // Finaliza o 'if'.
}                                               // Finaliza a função.

/**
 * @brief Libera toda a memória alocada para a BST de pistas (percurso post-order).
 */
void liberarPistas(NoPista* raiz) {             // Inicia a implementação da função que libera a memória das pistas.
    if (raiz != NULL) {                         // Condição de parada da recursão.
        liberarPistas(raiz->esquerda);          // Libera primeiro a sub-árvore esquerda.
        liberarPistas(raiz->direita);           // Libera depois a sub-árvore direita.
        free(raiz);                             // Por último, libera o nó atual.
    }                                           // Finaliza o 'if'.
}                                               // Finaliza a função.

/**
 * @brief Gerencia a interface para o usuário carregar um jogo.
 */
void gerenciarCarregamento(int *jogoIniciadoFlag, GameState *estado) { // Inicia a implementação do menu de carregamento.
    mostrarSlots();                             // Chama a função para exibir os slots disponíveis.
    printf("Escolha o slot para carregar (0 para cancelar): "); // Pede ao jogador para escolher um slot.
    int slot;                                   // Declara a variável para armazenar a escolha.
    while (scanf("%d", &slot) != 1) {           // Valida a entrada.
        printf("Entrada invalida. Digite um numero: "); // Informa o erro.
        limparBufferEntrada();                  // Limpa o buffer.
    }                                           // Finaliza o loop de validação.
    limparBufferEntrada();                      // Limpa o '\n'.
    if (slot > 0 && slot <= MAX_SAVES) {        // Verifica se o slot escolhido é válido.
        if (carregarJogo(slot, estado)) {       // Tenta carregar o jogo e verifica se a operação foi bem-sucedida.
            *jogoIniciadoFlag = 1;              // Se sim, atualiza a flag principal do jogo.
        }                                       // Finaliza o 'if' do carregamento.
    } else {                                    // Se o slot for inválido ou 0...
        printf("Carregamento cancelado.\n");    // Informa o cancelamento.
    }                                           // Finaliza o if/else.
}                                               // Finaliza a função.

/**
 * @brief Gerencia a interface para o usuário salvar o jogo.
 */
void gerenciarSalvamento(int jogoIniciadoFlag, GameState *estado) { // Inicia a implementação do menu de salvamento.
    if (!jogoIniciadoFlag) {                    // Verifica se há um jogo em andamento para salvar.
        printf("\nVoce precisa iniciar ou carregar um jogo para poder salvar!\n"); // Se não, informa o jogador.
        return;                                 // Sai da função.
    }                                           // Finaliza o 'if'.
    mostrarSlots();                             // Mostra os slots disponíveis.
    printf("Escolha o slot para salvar (0 para cancelar): "); // Pede ao jogador para escolher um slot.
    int slot;                                   // Variável para a escolha.
    while (scanf("%d", &slot) != 1) {           // Valida a entrada.
        printf("Entrada invalida. Digite um numero: "); // Informa o erro.
        limparBufferEntrada();                  // Limpa o buffer.
    }                                           // Finaliza a validação.
    limparBufferEntrada();                      // Limpa o '\n'.
    if (slot > 0 && slot <= MAX_SAVES) {        // Se o slot for válido...
        salvarJogo(slot, estado);               // Chama a função que efetivamente salva o jogo.
    } else {                                    // Se não...
        printf("Salvamento cancelado.\n");      // Informa o cancelamento.
    }                                           // Finaliza o if/else.
}                                               // Finaliza a função.

/**
 * @brief Gerencia a interface para o usuário resetar um arquivo de save.
 */
void gerenciarReset() {                         // Inicia a implementação do menu de reset.
    mostrarSlots();                             // Mostra os slots disponíveis.
    printf("Escolha o slot para resetar (0 para cancelar): "); // Pede ao jogador para escolher.
    int slot;                                   // Variável para a escolha.
    while (scanf("%d", &slot) != 1) {           // Valida a entrada.
        printf("Entrada invalida. Digite um numero: "); // Informa o erro.
        limparBufferEntrada();                  // Limpa o buffer.
    }                                           // Finaliza a validação.
    limparBufferEntrada();                      // Limpa o '\n'.
    if (slot > 0 && slot <= MAX_SAVES) {        // Se o slot for válido...
        resetarSlot(slot);                      // Chama a função que efetivamente reseta o slot.
    } else {                                    // Se não...
        printf("Reset cancelado.\n");           // Informa o cancelamento.
    }                                           // Finaliza o if/else.
}                                               // Finaliza a função.

/**
 * @brief Gera o nome do arquivo de save com base no número do slot.
 */
void getNomeArquivo(int slot, char* nomeArquivo) { // Inicia a implementação da função.
    sprintf(nomeArquivo, "save_detective_%d.dat", slot); // Usa sprintf para formatar a string do nome do arquivo.
}                                               // Finaliza a função.

/**
 * @brief Lê os arquivos de save existentes e exibe o status de cada slot.
 */
void mostrarSlots() {                           // Inicia a implementação da função.
    printf("\n--- SLOTS DE JOGO ---\n");         // Imprime o cabeçalho.
    for (int i = 1; i <= MAX_SAVES; i++) {      // Inicia um loop para verificar cada slot.
        char nomeArquivo[30];                   // Variável para guardar o nome do arquivo.
        getNomeArquivo(i, nomeArquivo);         // Gera o nome do arquivo para o slot atual.
        FILE* arquivo = fopen(nomeArquivo, "rb"); // Tenta abrir o arquivo em modo de leitura binária ("read binary").
        if (arquivo == NULL) {                  // Se fopen retornar NULL, o arquivo não existe.
            printf("%d. [ Vazio ]\n", i);       // Então, o slot está vazio.
        } else {                                // Se o arquivo existir...
            SaveState save;                     // Declara uma variável para armazenar os dados lidos.
            fread(&save, sizeof(SaveState), 1, arquivo); // Lê a estrutura inteira do arquivo para a variável.
            fclose(arquivo);                    // Fecha o arquivo.
            if(save.slotOcupado) {              // Verifica a flag dentro do arquivo.
                printf("%d. %s (%s)\n", i, save.nomeSave, save.dataHora); // Se ocupado, mostra o nome e a data.
            } else {                            // Se a flag for 0...
                printf("%d. [ Vazio ]\n", i);   // O slot está corrompido ou foi resetado, então é considerado vazio.
            }                                   // Finaliza o if/else da flag.
        }                                       // Finaliza o if/else da existência do arquivo.
    }                                           // Finaliza o loop.
    printf("-----------------------\n");         // Imprime o rodapé.
}                                               // Finaliza a função.

/**
 * @brief Salva o estado atual do jogo em um arquivo binário.
 */
void salvarJogo(int slot, GameState *estado) {  // Inicia a implementação da função de salvar.
    SaveState save;                             // Cria uma instância da estrutura SaveState.
    save.slotOcupado = 1;                       // Define a flag como 1, indicando que este é um save válido.
    save.estadoDoJogo = *estado;                // Copia todo o conteúdo do estado atual do jogo para a estrutura de save.
    printf("Digite um nome para este save (max 49 chars): "); // Pede um nome para o save.
    fgets(save.nomeSave, 50, stdin);            // Lê o nome do save de forma segura, evitando buffer overflow.
    save.nomeSave[strcspn(save.nomeSave, "\n")] = '\0'; // Remove o caractere '\n' que o fgets captura.
    time_t t = time(NULL);                      // Obtém o tempo atual do sistema.
    strftime(save.dataHora, 30, "%d/%m/%Y %H:%M", localtime(&t)); // Formata o tempo em uma string legível.
    char nomeArquivo[30];                       // Variável para o nome do arquivo.
    getNomeArquivo(slot, nomeArquivo);          // Gera o nome do arquivo.
    FILE* arquivo = fopen(nomeArquivo, "wb");   // Abre o arquivo em modo de escrita binária ("write binary").
    if (arquivo) {                              // Se o arquivo foi aberto com sucesso...
        fwrite(&save, sizeof(SaveState), 1, arquivo); // Escreve a estrutura 'save' inteira no arquivo.
        fclose(arquivo);                        // Fecha o arquivo.
        printf("\nJogo salvo com sucesso no Slot %d!\n", slot); // Informa o sucesso.
    } else {                                    // Se houve um erro ao abrir o arquivo...
        printf("\nErro ao abrir o arquivo para salvar!\n"); // Informa o erro.
    }                                           // Finaliza o if/else.
}                                               // Finaliza a função.

/**
 * @brief Função auxiliar para encontrar um nó na árvore com base no seu ID.
 */
No* encontrarNoPorId(No* noAtual, int id) {     // Inicia a implementação da função de busca.
    if (noAtual == NULL) return NULL;           // Caso base: se o nó é nulo, não encontrou, retorna NULL.
    if (noAtual->comodoId == id) return noAtual; // Se o ID do nó atual é o que procuramos, retorna o ponteiro para este nó.
    No* encontrado = encontrarNoPorId(noAtual->esquerda, id); // Procura recursivamente em toda a sub-árvore esquerda.
    if (encontrado != NULL) return encontrado;  // Se encontrou na esquerda, já retorna o resultado.
    return encontrarNoPorId(noAtual->direita, id); // Se não, procura e retorna o resultado da sub-árvore direita.
}                                               // Finaliza a função.

/**
 * @brief Carrega um jogo salvo a partir de um arquivo binário.
 */
int carregarJogo(int slot, GameState *estado) { // Inicia a implementação da função de carregar.
    char nomeArquivo[30];                       // Variável para o nome do arquivo.
    getNomeArquivo(slot, nomeArquivo);          // Gera o nome do arquivo.
    FILE* arquivo = fopen(nomeArquivo, "rb");   // Tenta abrir o arquivo para leitura binária.
    if (arquivo == NULL) {                      // Se não conseguir abrir...
        printf("\nSlot vazio ou nao encontrado!\n"); // Informa o jogador.
        return 0;                               // Retorna 0 para indicar falha.
    }                                           // Finaliza o 'if'.

    SaveState save;                             // Cria uma instância para receber os dados do arquivo.
    fread(&save, sizeof(SaveState), 1, arquivo); // Lê os dados do arquivo para a estrutura 'save'.
    fclose(arquivo);                            // Fecha o arquivo.
    
    if (!save.slotOcupado) {                    // Verifica a flag de validade.
        printf("\nEste slot de save esta corrompido ou vazio.\n"); // Se inválido, informa.
        return 0;                               // Retorna 0 para indicar falha.
    }                                           // Finaliza o 'if'.

    // --- Estratégia de Carregamento Seguro ---
    if(estado->raiz) liberarMapa(estado->raiz); // Libera o mapa de qualquer jogo anterior que estivesse na memória.
    if(estado->raizPistas) liberarPistas(estado->raizPistas); // Libera as pistas de qualquer jogo anterior.

    estado->raiz = criarMapa();                 // Reconstrói a estrutura do mapa do zero.
    
    // Copia os dados simples e os arrays do save para o estado atual do jogo.
    estado->mochila = save.estadoDoJogo.mochila;
    estado->totalItensColetados = save.estadoDoJogo.totalItensColetados;
    memcpy(estado->itensJaColetados, save.estadoDoJogo.itensJaColetados, sizeof(estado->itensJaColetados));
    estado->missaoAtualId = save.estadoDoJogo.missaoAtualId;
    estado->chanceDePista = save.estadoDoJogo.chanceDePista;
    estado->totalPistasColetadas = save.estadoDoJogo.totalPistasColetadas;
    memcpy(estado->pistasJaColetadas, save.estadoDoJogo.pistasJaColetadas, sizeof(estado->pistasJaColetadas));
    estado->jogadorPossuiChaveDoCofre = save.estadoDoJogo.jogadorPossuiChaveDoCofre;
    estado->cofreAberto = save.estadoDoJogo.cofreAberto;
    
    // Restaura a posição do jogador buscando na árvore recém-criada pelo ID que foi salvo.
    estado->posicaoAtual = encontrarNoPorId(estado->raiz, save.estadoDoJogo.posicaoAtual->comodoId);
    if (estado->posicaoAtual == NULL) {         // Medida de segurança caso o ID não seja encontrado.
        estado->posicaoAtual = estado->raiz;    // Volta para o início.
    }                                           // Finaliza o 'if'.

    // RECONSTRÓI a BST de pistas a partir do array de flags `pistasJaColetadas`.
    estado->raizPistas = NULL;                  // Começa com uma árvore vazia.
    for (int i = 0; i < TOTAL_COMODOS; i++) {   // Percorre o array de flags.
        if (estado->pistasJaColetadas[i] == 1) { // Se a pista 'i' havia sido coletada...
            estado->raizPistas = inserirPista(estado->raizPistas, DADOS_PISTAS_MISSOES[estado->missaoAtualId][i]); // ...a inserimos na árvore recém-criada.
        }                                       // Finaliza o 'if'.
    }                                           // Finaliza o loop.

    printf("\nJogo '%s' carregado com sucesso!\n", save.nomeSave); // Informa o sucesso.
    return 1;                                   // Retorna 1 para indicar sucesso.
}                                               // Finaliza a função.

/**
 * @brief Reseta um arquivo de save marcando-o como vazio.
 */
void resetarSlot(int slot) {                    // Inicia a implementação da função de reset.
    char nomeArquivo[30];                       // Variável para o nome do arquivo.
    getNomeArquivo(slot, nomeArquivo);          // Gera o nome do arquivo.
    FILE* arquivo = fopen(nomeArquivo, "rb+");  // Abre o arquivo em modo "leitura e escrita binária".
    if(arquivo == NULL) {                       // Se o arquivo não existir...
        printf("\nSlot %d ja esta vazio.\n", slot); // Informa que já está vazio.
        return;                                 // Sai da função.
    }                                           // Finaliza o 'if'.

    SaveState save;                             // Cria uma instância de save.
    fread(&save, sizeof(SaveState), 1, arquivo); // Lê o conteúdo atual (necessário para não corromper o arquivo).
    save.slotOcupado = 0;                       // Define a flag de ocupado como 0.
    strcpy(save.nomeSave, "[ Vazio ]");         // Altera o nome para "[ Vazio ]".
    fseek(arquivo, 0, SEEK_SET);                // Volta o cursor de escrita para o início do arquivo.
    fwrite(&save, sizeof(SaveState), 1, arquivo); // Escreve a estrutura modificada por cima da antiga.
    fclose(arquivo);                            // Fecha o arquivo.

    printf("\nSlot %d resetado com sucesso!\n", slot); // Informa o sucesso.
}                                               // Finaliza a função.

// Inclui a biblioteca de entrada e saída padrão (Standard Input/Output),
// essencial para usar funções como printf() para exibir texto e scanf() para ler dados do usuário.
#include <stdio.h>

// Inclui a biblioteca padrão (Standard Library), que contém funções
// de propósito geral. Neste código, ela é usada especificamente para a função system(),
// que permite executar comandos do sistema operacional (como limpar a tela).
#include <stdlib.h> 

// ######################################################
// DEFINIÇÕES GLOBAIS (CONSTANTES)
// ######################################################
// Usar #define cria constantes que tornam o código mais legível e fácil de modificar.
// O compilador substitui o nome da constante pelo valor definido antes de compilar.

#define linha 10   // Define o número de linhas do mapa do jogo.
#define coluna 10  // Define o número de colunas do mapa do jogo.
#define conek 5    // Define o valor numérico para o ataque "Cone".
#define cruzk 6    // Define o valor numérico para o ataque "Cruz".
#define octak 7    // Define o valor numérico para o ataque "Octaedro".
#define navio 3    // Define o valor numérico que representa uma parte de um navio.

// --- DEFINIÇÕES DE CORES (CÓDIGOS DE ESCAPE ANSI) ---
// NOTA: Para as cores funcionarem, o programa deve ser executado em um terminal que
// suporte códigos de escape ANSI, como o terminal do VS Code, Windows Terminal, Linux ou macOS.
// O Prompt de Comando antigo do Windows pode não exibir as cores corretamente.
#define RESET_COLOR "\x1b[0m"  // Reseta a cor do texto para o padrão do terminal.
#define BLUE "\x1b[34m"        // Define a cor do texto para azul.
#define WHITE "\x1b[37m"       // Define a cor do texto para branco.
#define RED "\x1b[31m"         // Define a cor do texto para vermelho.
#define YELLOW "\x1b[33m"      // Define a cor do texto para amarelo.

// Função para limpar o buffer de entrada do teclado.
// Após ler um número com scanf(), o caractere de nova linha ('\n', gerado ao pressionar Enter)
// permanece no buffer. Esta função consome todos os caracteres restantes até encontrar
// um '\n' ou o fim do arquivo (EOF), evitando que leituras futuras (como getchar())
// capturem esse caractere indesejado e pulem etapas importantes.
void limparBufferEntrada() {
    int c; // Variável para armazenar cada caractere lido.
    while ((c = getchar()) != '\n' && c != EOF);
}

// A função main() é o ponto de entrada de qualquer programa em C.
int main() {
    // Declaração de variáveis que serão usadas ao longo do programa.
    int opcao = 0; // Armazena a escolha do usuário no menu principal.
    int sentido, habilidade; // Armazenam as escolhas nos sub-menus.
    int lin, col; // Armazenam as coordenadas (linha e coluna) fornecidas pelo usuário.
    int campo[linha][coluna]; // Matriz (array 2D) que representa o campo de batalha.

    // Inicializa o campo com 0 (representando água).
    // Dois laços 'for' aninhados percorrem cada célula da matriz.
    for (int i = 0; i < linha; i++) {       // O laço externo percorre as linhas.
        for (int j = 0; j < coluna; j++) {  // O laço interno percorre as colunas.
            campo[i][j] = 0; // Atribui 0 a cada posição, "limpando" o mapa.
        }
    }

    // O laço 'do-while' garante que o menu seja exibido pelo menos uma vez
    // e continue a ser exibido até que o usuário escolha a opção de sair (5).
    do {
        // system("cls || clear") é um comando para limpar a tela do terminal.
        // "cls" é o comando para Windows.
        // "clear" é o comando para Linux e macOS.
        // O "||" faz com que o segundo comando seja executado apenas se o primeiro falhar.
        // Isso torna o código compatível com diferentes sistemas operacionais.
        system("cls || clear"); 
        
        // Exibe as opções do menu principal.
        printf("### Jogo de Batalha Naval ###\n");
        printf("1. Colocar um novo navio\n");
        printf("2. Usar uma habilidade de ataque\n");
        printf("3. Ver o mapa\n");
        printf("4. Ver regras\n");
        printf("5. Sair do jogo\n");
        printf("-----------------------------------\n");
        printf("Digite sua opcao: ");

        // Lê a entrada do usuário. scanf() retorna o número de itens lidos com sucesso.
        // Se o usuário digitar algo que não é um número (ex: uma letra), scanf() retornará 0.
        // Esta verificação protege o programa contra entradas inválidas.
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; // Atribui um valor inválido para que caia no 'default' do switch.
        }
        limparBufferEntrada(); // Limpa qualquer caractere restante, como o '\n' ou letras.

        // A estrutura 'switch' executa um bloco de código diferente dependendo do valor de 'opcao'.
        switch (opcao) {
            case 1: // Inserir Navio
                // O código para inserir o navio (lógica de coordenadas, verificação de limites e de ocupação)
                // é o mesmo da versão anterior. Ele pede a orientação e a posição inicial,
                // e então preenche o mapa se a posição for válida e estiver livre.
                printf("\n--- Colocar Navio de 3 Partes ---\n");
                printf("1. Horizontal\n");
                printf("2. Vertical\n");
                printf("3. Diagonal (crescente /)\n");
                printf("4. Diagonal (decrescente \\)\n");
                printf("Escolha a orientacao: ");
                scanf("%d", &sentido);
                limparBufferEntrada();

                switch (sentido) {
                    case 1: // Horizontal
                        printf("Digite a linha inicial (0 a %d): ", linha - 1);
                        scanf("%d", &lin);
                        printf("Digite a coluna inicial (0 a %d): ", coluna - navio);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        if ((lin >= 0 && lin < linha) && (col >= 0 && col <= coluna - navio)) {
                            if (campo[lin][col] == 0 && campo[lin][col + 1] == 0 && campo[lin][col + 2] == 0) {
                                campo[lin][col] = campo[lin][col + 1] = campo[lin][col + 2] = navio;
                                printf("\nNavio inserido com sucesso!\n");
                            } else {
                                printf("\nERRO: Posicao ja ocupada!\n");
                            }
                        } else {
                            printf("\nERRO: Coordenadas fora do mapa!\n");
                        }
                        break;
                    // Os outros casos (Vertical, Diagonais) seguem a mesma lógica de verificação e inserção.
                    case 2: // Vertical
                        printf("Digite a linha inicial (0 a %d): ", linha - navio);
                        scanf("%d", &lin);
                        printf("Digite a coluna inicial (0 a %d): ", coluna - 1);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        if ((lin >= 0 && lin <= linha - navio) && (col >= 0 && col < coluna)) {
                            if (campo[lin][col] == 0 && campo[lin + 1][col] == 0 && campo[lin + 2][col] == 0) {
                                campo[lin][col] = campo[lin + 1][col] = campo[lin + 2][col] = navio;
                                printf("\nNavio inserido com sucesso!\n");
                            } else {
                                printf("\nERRO: Posicao ja ocupada!\n");
                            }
                        } else {
                            printf("\nERRO: Coordenadas fora do mapa!\n");
                        }
                        break;
                    case 3: // Diagonal /
                        printf("Digite a linha inicial (%d a %d): ", navio - 1, linha - 1);
                        scanf("%d", &lin);
                        printf("Digite a coluna inicial (0 a %d): ", coluna - navio);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        if ((lin >= navio - 1 && lin < linha) && (col >= 0 && col <= coluna - navio)) {
                            if (campo[lin][col] == 0 && campo[lin - 1][col + 1] == 0 && campo[lin - 2][col + 2] == 0) {
                                campo[lin][col] = navio;
                                campo[lin - 1][col + 1] = navio;
                                campo[lin - 2][col + 2] = navio;
                                printf("\nNavio inserido com sucesso!\n");
                            } else {
                                printf("\nERRO: Posicao ja ocupada!\n");
                            }
                        } else {
                            printf("\nERRO: Coordenadas fora do mapa!\n");
                        }
                        break;
                    case 4: // Diagonal \
                        printf("Digite a linha inicial (0 a %d): ", linha - navio);
                        scanf("%d", &lin);
                        printf("Digite a coluna inicial (0 a %d): ", coluna - navio);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        if ((lin >= 0 && lin <= linha - navio) && (col >= 0 && col <= coluna - navio)) {
                            if (campo[lin][col] == 0 && campo[lin + 1][col + 1] == 0 && campo[lin + 2][col + 2] == 0) {
                                campo[lin][col] = navio;
                                campo[lin + 1][col + 1] = navio;
                                campo[lin + 2][col + 2] = navio;
                                printf("\nNavio inserido com sucesso!\n");
                            } else {
                                printf("\nERRO: Posicao ja ocupada!\n");
                            }
                        } else {
                            printf("\nERRO: Coordenadas fora do mapa!\n");
                        }
                        break;
                    default:
                        printf("\nOpcao de sentido invalida!\n");
                        break;
                }
                break;
            case 2: // Usar Habilidade
                // A lógica para usar habilidades também é a mesma: pede o tipo de ataque e a coordenada
                // de origem, verifica se o padrão de ataque cabe no mapa e, se couber, soma
                // o valor da habilidade nas células correspondentes.
                printf("\n--- Usar Habilidade de Ataque ---\n");
                printf("1. Cone\n");
                printf("2. Cruz\n");
                printf("3. Octaedro\n");
                printf("Escolha a habilidade: ");
                scanf("%d", &habilidade);
                limparBufferEntrada();
                switch (habilidade) {
                    case 1: // Cone
                        printf("Digite a linha do vertice (0 a %d): ", linha - 3);
                        scanf("%d", &lin);
                        printf("Digite a coluna do vertice (2 a %d): ", coluna - 3);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        if ((lin >= 0 && lin <= linha - 3) && (col >= 2 && col <= coluna - 3)) {
                            campo[lin][col] += conek;
                            campo[lin + 1][col - 1] += conek; campo[lin + 1][col] += conek; campo[lin + 1][col + 1] += conek;
                            campo[lin + 2][col - 2] += conek; campo[lin + 2][col - 1] += conek; campo[lin + 2][col] += conek; campo[lin + 2][col + 1] += conek; campo[lin + 2][col + 2] += conek;
                            printf("\nHabilidade Cone utilizada!\n");
                        } else {
                            printf("\nERRO: Coordenadas fora do mapa!\n");
                        }
                        break;
                    // Outros casos de habilidades seguem a mesma lógica.
                    case 2: // Cruz
                        printf("Digite a linha do centro (1 a %d): ", linha - 2);
                        scanf("%d", &lin);
                        printf("Digite a coluna do centro (2 a %d): ", coluna - 3);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        if ((lin >= 1 && lin <= linha - 2) && (col >= 2 && col <= coluna - 3)) {
                           campo[lin - 1][col] += cruzk; campo[lin + 1][col] += cruzk;
                           campo[lin][col-2] += cruzk; campo[lin][col-1] += cruzk; campo[lin][col] += cruzk; campo[lin][col+1] += cruzk; campo[lin][col+2] += cruzk;
                           printf("\nHabilidade Cruz utilizada!\n");
                        } else {
                           printf("\nERRO: Coordenadas fora do mapa!\n");
                        }
                        break;
                    case 3: // Octaedro
                        printf("Digite a linha do centro (1 a %d): ", linha - 2);
                        scanf("%d", &lin);
                        printf("Digite a coluna do centro (1 a %d): ", coluna - 2);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        if ((lin >= 1 && lin <= linha - 2) && (col >= 1 && col <= coluna - 2)) {
                           campo[lin - 1][col] += octak; campo[lin + 1][col] += octak;
                           campo[lin][col - 1] += octak; campo[lin][col] += octak; campo[lin][col + 1] += octak;
                           printf("\nHabilidade Octaedro utilizada!\n");
                        } else {
                           printf("\nERRO: Coordenadas fora do mapa!\n");
                        }
                        break;
                    default:
                        printf("\nOpcao de habilidade invalida!\n");
                        break;
                }
                break;
            case 3: // Ver o Mapa (MODIFICADO com símbolos e cores)
                printf("\n- - - - - MAPA ATUAL - - - - - \n");
                printf("     0  1  2  3  4  5  6  7  8  9\n");
                printf("----------------------------------\n");
                for (int i = 0; i < linha; i++) {
                    printf("%d |", i);
                    for (int j = 0; j < coluna; j++) {
                        int valor = campo[i][j];
                        
                        // Agora, em vez de números, exibimos símbolos coloridos.
                        // A lógica de verificação foi aprimorada para exibir diferentes status.
                        
                        // Se o valor for a soma de um navio + qualquer ataque, é um ACERTO.
                        if (valor >= navio + conek) { 
                            printf("  " RED "X" RESET_COLOR); // Imprime um X em vermelho
                        } else if (valor == navio) { // Se for apenas o valor de um navio, é um NAVIO INTACTO.
                            printf("  " WHITE "O" RESET_COLOR); // Imprime um O em branco
                        } else if (valor == 0) { // Se for 0, é ÁGUA.
                            printf("  " BLUE "~" RESET_COLOR); // Imprime um ~ em azul
                        } else { // Qualquer outro valor significa que uma habilidade atingiu a água.
                            printf("  " YELLOW "*" RESET_COLOR); // Imprime um * em amarelo
                        }
                    }
                    printf("\n");
                }
                printf("----------------------------------\n");
                // Exibe uma legenda para que o jogador entenda os símbolos.
                printf(BLUE "~" RESET_COLOR " = Agua | " WHITE "O" RESET_COLOR " = Navio | " RED "X" RESET_COLOR " = Atingido | " YELLOW "*" RESET_COLOR " = Habilidade na Agua\n");
                break;
            case 4: // Ver Regras
                // As regras agora usam os mesmos símbolos coloridos do mapa para facilitar a compreensão.
                printf("\n--- Regras do Jogo ---\n");
                printf("1. Navios (" WHITE "O" RESET_COLOR ") sao as partes de sua embarcacao.\n");
                printf("2. Zonas de habilidade (" YELLOW "*" RESET_COLOR ") sao os locais onde o ataque especial foi usado na agua.\n");
                printf("3. Um navio atingido (" RED "X" RESET_COLOR ") eh onde uma habilidade acertou um navio.\n");
                printf("4. Para posicionar navios, voce sempre informa a coordenada inicial.\n");
                break;
            case 5: // Sair
                printf("\nSaindo do jogo...\n");
                break;
            default: // Caso de opção inválida
                printf("\nOpcao invalida. Digite um numero entre 1 e 5.\n");
                break;
        }
        
        // Pausa o programa para o usuário ler o resultado, exceto se a opção for sair.
        // A condição foi melhorada para pausar apenas em opções válidas que não sejam a de sair.
        if (opcao > 0 && opcao < 5) {
             printf("\nPressione Enter para voltar ao menu...");
             getchar(); // Captura o Enter pressionado pelo usuário para continuar.
        }

    } while (opcao != 5); // A condição do laço: continuar enquanto 'opcao' for diferente de 5.

    return 0; // Retorna 0, indicando que o programa terminou com sucesso.
}

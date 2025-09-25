// Inclui a biblioteca de entrada e saída padrão (Standard Input/Output),
// essencial para usar funções como printf() para exibir texto e scanf() para ler dados do usuário.
#include <stdio.h>

// ######################################################
// DEFINIÇÕES GLOBAIS (CONSTANTES)
// ######################################################
// Usar #define é uma forma de criar constantes. O compilador substitui
// o nome da constante pelo valor definido antes de compilar o código.
// Isso torna o código mais legível e fácil de manter.

#define linha 10   // Define o número de linhas do mapa do jogo.
#define coluna 10  // Define o número de colunas do mapa do jogo.
#define conek 5    // Define o valor numérico que representa um ataque do tipo "Cone".
#define cruzk 6    // Define o valor numérico que representa um ataque do tipo "Cruz".
#define octak 7    // Define o valor numérico que representa um ataque do tipo "Octaedro".
#define navio 3    // Define o valor numérico que representa uma parte de um navio no mapa.
                   // Também é usado para definir o tamanho (comprimento) do navio.

// Função para limpar o buffer de entrada do teclado.
// Após ler um número com scanf(), o caractere de nova linha ('\n', gerado ao pressionar Enter)
// permanece no buffer. Esta função consome todos os caracteres restantes até encontrar
// um '\n' ou o fim do arquivo (EOF), evitando que leituras futuras (como getchar() ou outro scanf())
// capturem esse caractere indesejado e causem comportamento inesperado no programa.
void limparBufferEntrada() {
    int c; // Variável para armazenar cada caractere lido.
    // Loop que continua enquanto o caractere lido (c) não for uma nova linha ('\n')
    // e também não for o indicador de fim de arquivo (EOF).
    while ((c = getchar()) != '\n' && c != EOF);
}

// A função main() é o ponto de entrada de qualquer programa em C.
int main() {
    // Declaração de variáveis que serão usadas ao longo do programa.
    int opcao = 0; // Armazena a escolha do usuário no menu principal.
    int sentido, habilidade; // Armazenam as escolhas nos sub-menus de navio e ataque.
    int lin, col; // Armazenam as coordenadas (linha e coluna) fornecidas pelo usuário.
    
    // Declaração da matriz (array bidimensional) que representa o campo de batalha.
    // O tamanho é definido pelas constantes 'linha' e 'coluna' (10x10).
    int campo[linha][coluna];

    /*#######################################*/
    // Inicializa o campo com 0 (representando água)
    /*#######################################*/
    // Dois laços 'for' aninhados percorrem cada célula da matriz.
    for (int i = 0; i < linha; i++) {       // O laço externo percorre as linhas.
        for (int j = 0; j < coluna; j++) {  // O laço interno percorre as colunas para cada linha.
            campo[i][j] = 0; // Atribui 0 a cada posição, "limpando" o mapa.
        }
    }

    /*#######################################*/
    // Menu principal do programa
    /*#######################################*/
    // O laço 'do-while' garante que o menu seja exibido pelo menos uma vez
    // e continue a ser exibido até que o usuário escolha a opção de sair (5).
    do {
        // Exibe as opções do menu para o usuário.
        printf("\n\n");
        printf("### Jogo de Batalha Naval ###\n");
        printf("1. Colocar um novo navio\n");
        printf("2. Usar uma habilidade de ataque\n");
        printf("3. Ver o mapa\n");
        printf("4. Ver regras\n");
        printf("5. Sair do jogo\n");
        printf("-----------------------------------\n");
        printf("Digite sua opcao: ");

        // Lê o número digitado pelo usuário e o armazena na variável 'opcao'.
        scanf("%d", &opcao);
        limparBufferEntrada(); // Chama a função para limpar o buffer após a leitura do número.

        // A estrutura 'switch' executa um bloco de código diferente dependendo do valor de 'opcao'.
        switch (opcao) {
            /*#######################################*/
            // CASE 1: Inserir Navio
            /*#######################################*/
            case 1:
                printf("\n--- Colocar Navio de 3 Partes ---\n");
                printf("1. Horizontal\n");
                printf("2. Vertical\n");
                printf("3. Diagonal (crescente /)\n");
                printf("4. Diagonal (decrescente \\)\n");
                printf("Escolha a orientacao: ");
                scanf("%d", &sentido); // Lê a orientação desejada.
                limparBufferEntrada();

                // Um novo 'switch' para tratar as diferentes orientações do navio.
                switch (sentido) {
                    case 1: // Horizontal
                        // Pede as coordenadas iniciais. A coluna máxima é limitada para que o navio caiba no mapa.
                        printf("Digite a linha inicial (0 a %d): ", linha - 1);
                        scanf("%d", &lin);
                        printf("Digite a coluna inicial (0 a %d): ", coluna - navio);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        
                        // 1ª Verificação: Garante que as coordenadas estão dentro dos limites válidos do mapa.
                        if ((lin >= 0 && lin < linha) && (col >= 0 && col <= coluna - navio)) {
                            // 2ª Verificação: Checa se todas as 3 posições necessárias estão livres (iguais a 0).
                            if (campo[lin][col] == 0 && campo[lin][col + 1] == 0 && campo[lin][col + 2] == 0) {
                                // Se estiverem livres, posiciona o navio mudando os valores para o valor da constante 'navio'.
                                campo[lin][col] = campo[lin][col + 1] = campo[lin][col + 2] = navio;
                                printf("\nNavio inserido com sucesso!\n");
                            } else {
                                printf("\nERRO: Posicao ja ocupada!\n");
                            }
                        } else {
                            printf("\nERRO: Coordenadas fora do mapa!\n");
                        }
                        break;

                    case 2: // Vertical
                        printf("Digite a linha inicial (0 a %d): ", linha - navio);
                        scanf("%d", &lin);
                        printf("Digite a coluna inicial (0 a %d): ", coluna - 1);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        // 1ª Verificação: Limites do mapa.
                        if ((lin >= 0 && lin <= linha - navio) && (col >= 0 && col < coluna)) {
                            // 2ª Verificação: Espaço livre.
                            if (campo[lin][col] == 0 && campo[lin + 1][col] == 0 && campo[lin + 2][col] == 0) {
                                // Posiciona o navio.
                                campo[lin][col] = campo[lin + 1][col] = campo[lin + 2][col] = navio;
                                printf("\nNavio inserido com sucesso!\n");
                            } else {
                                printf("\nERRO: Posicao ja ocupada!\n");
                            }
                        } else {
                            printf("\nERRO: Coordenadas fora do mapa!\n");
                        }
                        break;

                    case 3: // Diagonal crescente (/)
                        printf("Digite a linha inicial (%d a %d): ", navio - 1, linha - 1);
                        scanf("%d", &lin);
                        printf("Digite a coluna inicial (0 a %d): ", coluna - navio);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        // 1ª Verificação: Limites do mapa.
                        if ((lin >= navio - 1 && lin < linha) && (col >= 0 && col <= coluna - navio)) {
                            // 2ª Verificação: Espaço livre (lin-1, lin-2, etc., para subir na diagonal).
                            if (campo[lin][col] == 0 && campo[lin - 1][col + 1] == 0 && campo[lin - 2][col + 2] == 0) {
                                // Posiciona o navio.
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

                    case 4: // Diagonal decrescente (\)
                        printf("Digite a linha inicial (0 a %d): ", linha - navio);
                        scanf("%d", &lin);
                        printf("Digite a coluna inicial (0 a %d): ", coluna - navio);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        // 1ª Verificação: Limites do mapa.
                        if ((lin >= 0 && lin <= linha - navio) && (col >= 0 && col <= coluna - navio)) {
                            // 2ª Verificação: Espaço livre (lin+1, lin+2, etc., para descer na diagonal).
                            if (campo[lin][col] == 0 && campo[lin + 1][col + 1] == 0 && campo[lin + 2][col + 2] == 0) {
                                // Posiciona o navio.
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
                        
                    default: // Caso o usuário digite uma opção de sentido inválida.
                        printf("\nOpcao de sentido invalida!\n");
                        break;
                }
                break;

            /*#######################################*/
            // CASE 2: Usar Habilidade
            /*#######################################*/
            case 2:
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
                        // Verifica se as coordenadas do vértice permitem que o desenho do cone caiba no mapa.
                        if ((lin >= 0 && lin <= linha - 3) && (col >= 2 && col <= coluna - 3)) {
                            // Adiciona o valor do ataque 'conek' às células do mapa no formato de um cone.
                            // Usar "+=" permite acumular ataques na mesma célula.
                            campo[lin][col] += conek;
                            campo[lin + 1][col - 1] += conek; campo[lin + 1][col] += conek; campo[lin + 1][col + 1] += conek;
                            campo[lin + 2][col - 2] += conek; campo[lin + 2][col - 1] += conek; campo[lin + 2][col] += conek; campo[lin + 2][col + 1] += conek; campo[lin + 2][col + 2] += conek;
                            printf("\nHabilidade Cone utilizada!\n");
                        } else {
                            printf("\nERRO: Coordenadas fora do mapa!\n");
                        }
                        break;
                    case 2: // Cruz
                        printf("Digite a linha do centro (1 a %d): ", linha - 2);
                        scanf("%d", &lin);
                        printf("Digite a coluna do centro (2 a %d): ", coluna - 3);
                        scanf("%d", &col);
                        limparBufferEntrada();
                        // Verifica se as coordenadas do centro permitem que o desenho da cruz caiba no mapa.
                        if ((lin >= 1 && lin <= linha - 2) && (col >= 2 && col <= coluna - 3)) {
                           // Adiciona o valor do ataque 'cruzk' às células no formato de uma cruz.
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
                        // Verifica se as coordenadas do centro permitem que o desenho do octaedro caiba no mapa.
                        if ((lin >= 1 && lin <= linha - 2) && (col >= 1 && col <= coluna - 2)) {
                           // Adiciona o valor do ataque 'octak' às células no formato de um octaedro.
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

            /*#######################################*/
            // CASE 3: Ver o Mapa
            /*#######################################*/
            case 3:
                printf("\n- - - - - MAPA ATUAL - - - - - \n");
                printf("     0  1  2  3  4  5  6  7  8  9\n"); // Cabeçalho das colunas
                printf("----------------------------------\n");
                // Laços para percorrer a matriz e exibir seu conteúdo.
                for (int i = 0; i < linha; i++) {
                    printf("%d |", i); // Cabeçalho da linha atual.
                    for (int j = 0; j < coluna; j++) {
                        int valorFinal = campo[i][j]; // Pega o valor da célula.
                        
                        // Lógica para detectar acertos: se o valor da célula for maior ou igual
                        // à soma de um navio (3) e o menor valor de ataque (5), significa que
                        // um navio naquela posição foi atingido.
                        if (valorFinal >= navio + conek) {
                            valorFinal = 9; // Atribui 9 para representar um acerto no mapa.
                        }
                        // Imprime o valor da célula formatado para ocupar 3 espaços, alinhando o mapa.
                        printf("%3d", valorFinal);
                    }
                    printf("\n"); // Pula para a próxima linha após imprimir todas as colunas.
                }
                printf("----------------------------------\n");
                break;

            /*#######################################*/
            // CASE 4: Ver Regras
            /*#######################################*/
            case 4:
                // Simplesmente exibe as regras do jogo.
                printf("\n--- Regras do Jogo ---\n");
                printf("1. Navios sao representados pelo numero %d.\n", navio);
                printf("2. Habilidades (5, 6, 7) somam pontos. Se um local com navio for atingido,\n");
                printf("   seu valor sera %d + [valor da habilidade].\n", navio);
                printf("3. No mapa, qualquer valor >= %d representa um navio atingido e sera mostrado como 9!\n", navio + conek);
                printf("4. Para posicionar navios, voce sempre informa a coordenada inicial\n");
                printf("   (a mais a esquerda, a de cima ou a de baixo, dependendo da orientacao).\n");
                break;

            /*#######################################*/
            // CASE 5: Sair
            /*#######################################*/
            case 5:
                printf("\nSaindo do jogo...\n");
                break;

            // O 'default' é executado se o valor de 'opcao' não corresponder a nenhum 'case'.
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
        
        // Se a opção não for sair, o programa pausa e espera que o usuário pressione Enter.
        // Isso dá tempo para o usuário ler a saída de sua ação antes de o menu ser exibido novamente.
        if (opcao != 5) {
             printf("\nPressione Enter para voltar ao menu...");
             getchar(); // Captura o Enter pressionado pelo usuário.
        }

    } while (opcao != 5); // A condição do laço: continuar enquanto 'opcao' for diferente de 5.

    // Retorna 0 para o sistema operacional, indicando que o programa terminou com sucesso.
    return 0;
}

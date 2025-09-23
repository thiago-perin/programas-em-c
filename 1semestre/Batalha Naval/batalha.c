#include <stdio.h>

#define linha 10   // quantidade de linhas do mapa
#define coluna 10  // quantidade de colunas do mapa
#define conek 5    // valor adicionado ao mapa ao usar a habilidade cone
#define cruzk 6    // valor adicionado ao mapa ao usar a habilidade cruz
#define octak 7    // valor adicionado ao mapa ao usar a habilidade octaedro
#define navio 3    // valor/tamanho do navio a ser inserido no mapa

// Função para limpar o buffer de entrada (evita bugs de leitura)
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int opcao = 0;
    int sentido, habilidade;
    int lin, col;
    int campo[linha][coluna];

    /*#######################################*/
    // Inicializa o campo com 0 (água)
    /*#######################################*/
    for (int i = 0; i < linha; i++) {
        for (int j = 0; j < coluna; j++) {
            campo[i][j] = 0;
        }
    }

    /*#######################################*/
    // Menu principal do programa
    /*#######################################*/
    do {
        printf("\n\n");
        printf("### Jogo de Batalha Naval ###\n");
        printf("1. Colocar um novo navio\n");
        printf("2. Usar uma habilidade de ataque\n");
        printf("3. Ver o mapa\n");
        printf("4. Ver regras\n");
        printf("5. Sair do jogo\n");
        printf("-----------------------------------\n");
        printf("Digite sua opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada(); // Limpa o buffer após a leitura do número

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
                        if ((lin >= 0 && lin <= linha - 3) && (col >= 2 && col <= coluna - 3)) {
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

            /*#######################################*/
            // CASE 3: Ver o Mapa
            /*#######################################*/
            case 3:
                printf("\n- - - - - MAPA ATUAL - - - - - \n");
                printf("    0  1  2  3  4  5  6  7  8  9\n");
                printf("----------------------------------\n");
                for (int i = 0; i < linha; i++) {
                    printf("%d |", i);
                    for (int j = 0; j < coluna; j++) {
                        int valorFinal = campo[i][j];
                        if (valorFinal >= navio + conek) { // Verifica se um navio foi atingido
                            valorFinal = 9;
                        }
                        printf("%3d", valorFinal);
                    }
                    printf("\n");
                }
                printf("----------------------------------\n");
                break;

            /*#######################################*/
            // CASE 4: Ver Regras
            /*#######################################*/
            case 4:
                printf("\n--- Regras do Jogo ---\n");
                printf("1. Navios sao representados pelo numero %d.\n", navio);
                printf("2. Habilidades (5, 6, 7) somam pontos. Se um local com navio for atingido,\n");
                printf("   seu valor sera %d + [valor da habilidade].\n", navio);
                printf("3. No mapa, qualquer valor 9 representa um navio atingido!\n");
                printf("4. Para posicionar navios, voce sempre informa a coordenada inicial\n");
                printf("   (a mais a esquerda, a de cima ou a de baixo, dependendo da orientacao).\n");
                break;

            /*#######################################*/
            // CASE 5: Sair
            /*#######################################*/
            case 5:
                printf("\nSaindo do jogo...\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
        
        if (opcao != 5) {
             printf("\nPressione Enter para voltar ao menu...");
             getchar();
        }

    } while (opcao != 5);

    return 0;
}

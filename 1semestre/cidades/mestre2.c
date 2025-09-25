// Inclui a biblioteca de entrada e saída padrão (Standard Input/Output).
// Essencial para usar funções como printf() (para mostrar texto na tela)
// e scanf() (para ler dados digitados pelo usuário).
#include <stdio.h>


// A função main() é o ponto de partida de todo programa em C.
// A execução do código começa aqui.
int main() {
 
    // ######################################################
    // DECLARAÇÃO DE VARIÁVEIS
    // ######################################################

    // Variáveis do tipo 'int' (inteiro) para armazenar números inteiros.
    int pontos_turisticos1, pontos_turisticos2;
    
    // 'unsigned int' é um tipo de inteiro que armazena apenas valores positivos (sem sinal).
    // É uma ótima escolha para 'população', pois este valor nunca será negativo.
    unsigned int populacao1, populacao2;
    
    // Variáveis do tipo 'float' para armazenar números com casas decimais (ponto flutuante).
    float pib1, area1, densidade_pop1, pib_capita1, super_poder1, inv_denpop1;
    float pib2, area2, densidade_pop2, pib_capita2, super_poder2, inv_denpop2;
    
    // Arrays de caracteres (strings) para armazenar textos.
    char cidade1[20], cidade2[20]; // Armazena até 19 caracteres + 1 caractere nulo final ('\0').
    char estado1[2], estado2[2];   // Armazena 1 letra + o terminador '\0'.
    char codigo1[3], codigo2[3];   // Armazena 2 caracteres numéricos + o terminador '\0'.


    // ######################################################
    // COLETA DE DADOS DAS CARTAS
    // ######################################################
    
    // Exibe o título do jogo.
    printf("Desafio Super Trunfo - Países - nivel mestre \n\n");
 
    // --- Dados da Carta 1 ---
    printf("--- Cadastro da Carta 1 ---\n");
    printf("Digite o nome da primeira cidade usando até 20 caracteres: \n");
    // scanf com "%s" lê uma string (texto) até encontrar um espaço.
    scanf("%s", &cidade1);

    printf("Digite uma letra para representar o primeiro estado usando um caracter (A até H): \n");
    scanf("%s", &estado1);

    printf("Digite um número(01...04) para representar a primeira cidade: \n");
    scanf("%s", &codigo1);
     
    printf("Digite o tamanho da população da primeira cidade em milhares de habitantes: \n");
    // O especificador de formato '%u' é usado para ler um 'unsigned int' (inteiro sem sinal).
    scanf("%u", &populacao1);

    printf("Digite o valor da área da primeira cidade em quilômetros quadrados: \n");
    // O especificador de formato '%f' é usado para ler um 'float'.
    scanf("%f", &area1);
     
    printf("Digite o pib da primeira cidade em reais: \n");
    scanf("%f", &pib1);
 
    printf("Digite quantos pontos turisticos tem na primeira cidade: \n");
    // O especificador de formato '%d' é usado para ler um 'int' padrão.
    scanf("%d", &pontos_turisticos1);
     
    // --- Dados da Carta 2 ---
    printf("\n--- Cadastro da Carta 2 ---\n");
    printf("Digite o nome da segunda cidade usando até 20 caracteres: \n");
    scanf("%s", &cidade2);

    printf("Digite uma letra para representar o segundo estado usando um caracter (A até H): \n");
    scanf("%s", &estado2); // Havia um scanf faltando aqui no código original para ler o estado 2

    printf("Digite um número(01...04) para representar a segunda cidade: \n");
    scanf("%s", &codigo2);
     
    printf("Digite o tamanho da população da segunda cidade em milhares de habitantes: \n");
    scanf("%u", &populacao2);

    printf("Digite o valor da área da segunda cidade em quilômetros quadrados: \n");
    scanf("%f", &area2);
     
    printf("Digite o pib da segunda cidade em reais: \n");
    scanf("%f", &pib2);
 
    printf("Digite quantos pontos turisticos tem na segunda cidade: \n");
    scanf("%d", &pontos_turisticos2);
     
    // ######################################################
    // CÁLCULOS DOS ATRIBUTOS
    // ######################################################

    // Calcula a densidade populacional (habitantes por área).
    densidade_pop1 = populacao1 / area1;
    inv_denpop1 = area1 / populacao1; // Inverso da densidade.
    densidade_pop2 = populacao2 / area2;
    inv_denpop2 = area2 / populacao2;
     
    // Calcula o PIB per capita (riqueza média por habitante).
    pib_capita1 = pib1 * 1000 / populacao1;
    pib_capita2 = pib2 * 1000 / populacao2;

    // Calcula o "Super Poder", uma pontuação geral fictícia para cada carta.
    super_poder1 = populacao1 + area1 + pib1 + pontos_turisticos1 + pib_capita1 + inv_denpop1;
    super_poder2 = populacao2 + area2 + pib2 + pontos_turisticos2 + pib_capita2 + inv_denpop2;

    // ######################################################
    // EXIBIÇÃO DAS CARTAS COMPLETAS
    // ######################################################
    
    // Imprime um resumo formatado de todos os dados da Carta 1.
    printf("\n--- Carta 1 ---\n");
    printf("Estado: %s \n", estado1);
    printf("Código: %s %s \n", estado1, codigo1);
    printf("Nome da cidade: %s \n", cidade1);
    // Para imprimir um 'unsigned int', usamos '%u'. '%.2u' não é padrão, mas o correto é só '%u'.
    printf("População: %u mil habitantes \n", populacao1);
    // O código \u00B2 é uma representação Unicode para o caractere '²' (ao quadrado).
    printf("Area: %.2f Km\u00B2 \n", area1); 
    printf("PIB: %.2f bilhões de reais \n", pib1);
    printf("Número de Pontos Turisticos: %d \n", pontos_turisticos1);
    printf("Densidade Populacional: %.2f mil hab / Km\u00B2 \n", densidade_pop1);
    printf("Pib per Capita: %.2f reais por habitante\n", pib_capita1);

    // Repete o processo para a Carta 2.
    printf("\n--- Carta 2 ---\n");
    printf("Estado: %s \n", estado2);
    printf("Código: %s %s \n", estado2, codigo2);
    printf("Nome da cidade: %s \n", cidade2);
    printf("População: %u mil habitantes \n", populacao2);
    printf("Area: %.2f Km\u00B2 \n", area2);
    printf("PIB: %.2f bilhões de reais \n", pib2);
    printf("Número de Pontos Turisticos: %d \n", pontos_turisticos2);
    printf("Densidade Populacional: %.2f mil hab / Km\u00B2 \n", densidade_pop2);
    printf("Pib per Capita: %.2f reais por habitante\n", pib_capita2);

    // ######################################################
    // PRÉ-CÁLCULO DOS VENCEDORES DE CADA CATEGORIA
    // ######################################################
    // Uma lógica para determinar o vencedor de cada atributo antes mesmo de o usuário escolher.

    // A expressão 'populacao1 < populacao2' resulta em 1 (verdadeiro) se a pop2 for maior, ou 0 (falso) caso contrário.
    // 'populacao++' soma 1. Se o resultado era 1 (carta 2 venceu), vira 2. Se era 0 (carta 1 venceu/empatou), vira 1.
    // O resultado final é o número da carta vencedora.
    int populacao = populacao1 < populacao2;
    populacao++;
     
    // O mesmo processo se repete para as outras categorias. Regra: menor é melhor.
    int area = area1 < area2;
    area++;
     
    int pib = pib1 < pib2;
    pib++;
     
    int pontos_turisticos = pontos_turisticos1 < pontos_turisticos2;
    pontos_turisticos++;
     
    // ATENÇÃO: Para densidade populacional, a lógica é invertida. Maior é melhor.
    int densidade_pop = densidade_pop1 > densidade_pop2;
    densidade_pop++;
     
    int pib_capita = pib_capita1 < pib_capita2;
    pib_capita++;
     
    int super_poder = super_poder1 < super_poder2;
    super_poder++;

    // ######################################################
    // INTERAÇÃO COM O USUÁRIO PARA A DISPUTA
    // ######################################################

    // Variáveis para guardar as escolhas do usuário.
    int escolha1, escolha2;

    // Exibe um menu para o usuário escolher quais atributos comparar.
    printf("\nQuais propriedades você quer comparar?:\n");
    printf("1. População \n2. Área \n3. PIB \n4. Pontos turísticos \n");
    printf("5. Densidade populacional \n6. PIB per capita\n7. Super poder \n");
    
    // Lê as duas escolhas do usuário.
    printf("\nEscolha a primeira propriedade: \n");
    scanf("%d", &escolha1);
    printf("Escolha a segunda propriedade, devendo ser diferente da primeira: \n");
    scanf("%d", &escolha2);
     
    // Validação simples para garantir que as escolhas são diferentes.
    if (escolha1 == escolha2) { 
        printf("Você escolheu a mesma propriedade que a primeira, escolha outra propriedade. \n");
        scanf("%d", &escolha2);
    } else {} // O 'else' vazio não faz nada, mas a estrutura é válida.
     
    printf("\n--- Comparação da primeira propriedade escolhida ---\n");
     
    // Variáveis para guardar os "pontos" de cada carta com base na escolha do usuário.
    float pontuacao1carta1, pontuacao1carta2;
     
    // A estrutura 'switch' executa um bloco de código diferente baseado no valor de 'escolha1'.
    switch (escolha1) {
        // Se o usuário digitou 1...
        case 1:
            // Compara as populações.
            if (populacao1 == populacao2) {
                printf("População: Houve empate \n");
            } else {
                printf("Carta 1 - %s, População: %u mil hab.\n", cidade1, populacao1);
                printf("Carta 2 - %s, População: %u mil hab.\n", cidade2, populacao2);
                // Usa o resultado pré-calculado para anunciar o vencedor.
                printf("População: Carta %d venceu \n", populacao);
                // Guarda os valores da população para a soma final.
                pontuacao1carta1 = populacao1;
                pontuacao1carta2 = populacao2;
            }
            break; // O 'break' é essencial para sair do switch após um caso ser executado.
        
        // Os outros casos seguem a mesma lógica para cada atributo.
        case 2:
            if (area1 == area2) { 
                printf("Área: Houve empate \n");
            } else {
                printf("Carta 1 - %s, Área: %.2f km\u00B2 \n", cidade1, area1);
                printf("Carta 2 - %s, Área: %.2f km\u00B2 \n", cidade2, area2);
                printf("Área: Carta %d venceu \n", area);
                pontuacao1carta1 = area1;
                pontuacao1carta2 = area2;
            }
            break;
        case 3:
            if (pib1 == pib2) {
                printf("PIB: Houve empate \n");
            } else {
                printf("Carta 1 - %s, PIB: %.2f bilhões de reais\n", cidade1, pib1);
                printf("Carta 2 - %s, PIB: %.2f bilhões de reais \n", cidade2, pib2);
                printf("PIB: Carta %d venceu \n", pib);
                pontuacao1carta1 = pib1;
                pontuacao1carta2 = pib2;
            }
            break;
        case 4:
             if (pontos_turisticos1 == pontos_turisticos2) {
                printf("Pontos turísticos: Houve empate \n");
            } else {
                printf("Carta 1 - %s, Pontos turísticos: %d\n", cidade1, pontos_turisticos1);
                printf("Carta 2 - %s, Pontos turísticos: %d\n", cidade2, pontos_turisticos2);
                printf("Pontos turísticos: Carta %d venceu \n", pontos_turisticos);
                pontuacao1carta1 = pontos_turisticos1;
                pontuacao1carta2 = pontos_turisticos2;
            }
            break;
        case 5:
            if (densidade_pop1 == densidade_pop2) {
                printf("Densidade populacional: Houve empate \n");
            } else {
                printf("Carta 1 - %s, Densidade: %.2f mil hab./km\u00B2 \n", cidade1, densidade_pop1);
                printf("Carta 2 - %s, Densidade: %.2f mil hab./km\u00B2 \n", cidade2, densidade_pop2);
                printf("Densidade populacional: Carta %d venceu \n", densidade_pop);
                pontuacao1carta1 = densidade_pop1;
                pontuacao1carta2 = densidade_pop2;
            }
            break;
        case 6:
            if (pib_capita1 == pib_capita2) {
                printf("PIB per capita: Houve empate \n");
            } else {
                printf("Carta 1 - %s, PIB per capita: %.2f reais/hab.\n", cidade1, pib_capita1);
                printf("Carta 2 - %s, PIB per capita: %.2f reais/hab.\n", cidade2, pib_capita2);
                printf("PIB per capita: Carta %d venceu \n", pib_capita);
                pontuacao1carta1 = pib_capita1;
                pontuacao1carta2 = pib_capita2;
            }
            break;
        case 7:
            if (super_poder1 == super_poder2) {
                printf("Super poder: Houve empate \n");
            } else {
                printf("Carta 1 - %s, Super poder: %.2f\n", cidade1, super_poder1);
                printf("Carta 2 - %s, Super poder: %.2f\n", cidade2, super_poder2);
                printf("Super poder: Carta %d venceu \n", super_poder);
                pontuacao1carta1 = super_poder1;
                pontuacao1carta2 = super_poder2;
            }
            break;
        // O 'default' é executado se o usuário digitar um número que não corresponde a nenhum 'case'.
        default:
            printf("Opção inválida. \n"); 
            break;
    }

    printf("\n--- Comparação da segunda propriedade escolhida ---\n");
     
    float pontuacao2carta1, pontuacao2carta2;
     
    // Um segundo 'switch' idêntico para a segunda escolha do usuário.
    // Ele guarda os resultados nas variáveis 'pontuacao2...'.
    switch (escolha2) {
        // (O conteúdo dos cases é o mesmo do switch anterior, apenas armazenando
        // o resultado nas variáveis 'pontuacao2carta1' e 'pontuacao2carta2')
        case 1:
            if (populacao1 == populacao2) { printf("População: Houve empate \n"); } 
            else {
                printf("População: Carta %d venceu \n", populacao);
                pontuacao2carta1 = populacao1; pontuacao2carta2 = populacao2;
            } break;
        case 2:
            if (area1 == area2) { printf("Área: Houve empate \n"); }
            else {
                printf("Área: Carta %d venceu \n", area);
                pontuacao2carta1 = area1; pontuacao2carta2 = area2;
            } break;
        // ... (e assim por diante para os outros casos) ...
        case 3: if (pib1 == pib2){printf("PIB: Houve empate \n");} else {printf("PIB: Carta %d venceu \n", pib); pontuacao2carta1 = pib1; pontuacao2carta2 = pib2;} break;
        case 4: if (pontos_turisticos1 == pontos_turisticos2){printf("Pontos turísticos: Houve empate \n");} else {printf("Pontos turísticos: Carta %d venceu \n", pontos_turisticos); pontuacao2carta1 = pontos_turisticos1; pontuacao2carta2 = pontos_turisticos2;} break;
        case 5: if (densidade_pop1 == densidade_pop2){printf("Densidade populacional: Houve empate \n");} else {printf("Densidade populacional: Carta %d venceu \n", densidade_pop); pontuacao2carta1 = densidade_pop1; pontuacao2carta2 = densidade_pop2;} break;
        case 6: if (pib_capita1 == pib_capita2){printf("PIB per capita: Houve empate \n");} else {printf("PIB per capita: Carta %d venceu \n", pib_capita); pontuacao2carta1 = pib_capita1; pontuacao2carta2 = pib_capita2;} break;
        case 7: if (super_poder1 == super_poder2){printf("Super poder: Houve empate \n");} else {printf("Super poder: Carta %d venceu \n", super_poder); pontuacao2carta1 = super_poder1; pontuacao2carta2 = super_poder2;} break;
        default: printf("Opção inválida. \n"); break;
    }
     
    // ######################################################
    // CÁLCULO FINAL E DECLARAÇÃO DO VENCEDOR GERAL
    // ######################################################

    // Soma os valores das duas propriedades escolhidas para cada carta.
    float totalcarta1= pontuacao1carta1+ pontuacao2carta1;
    float totalcarta2= pontuacao1carta2+ pontuacao2carta2;
     
    // Exibe a pontuação total de cada carta.
    printf("\n--- Resultado Final ---\n");
    printf("O total de pontos da carta 1 (soma das propriedades) é %.2f. \n", totalcarta1);
    printf("O total de pontos da carta 2 (soma das propriedades) é %.2f. \n", totalcarta2);
    
    // Usa o 'operador ternário' para declarar o vencedor. É uma forma compacta de um 'if-else'.
    // Lê-se: (CONDIÇÃO) ? (faça isso se for verdade) : (faça isso se for falso);
    totalcarta1 > totalcarta2 ? printf("Carta 1 é a grande vencedora!\n") : printf("Carta 2 é a grande vencedora!\n");
     
    // 'return 0' sinaliza ao sistema operacional que o programa terminou com sucesso.
    return 0;
}

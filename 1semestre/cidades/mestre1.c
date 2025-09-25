// Inclui a biblioteca de entrada e saída padrão (Standard Input/Output).
// Esta linha é fundamental para que possamos usar funções como printf() (para mostrar texto na tela)
// e scanf() (para ler dados digitados pelo usuário).
#include <stdio.h>

// A função main() é o ponto de partida de todo programa em C.
// O código dentro de suas chaves {} será executado quando o programa for iniciado.
int main() {
    
    // ######################################################
    // DECLARAÇÃO DE VARIÁVEIS
    // ######################################################
    // Aqui, reservamos espaço na memória para guardar todas as informações
    // que o programa vai usar para as duas "cartas" do jogo.

    // Variáveis do tipo 'int' (inteiro) para armazenar números inteiros.
    int pontos_turisticos1, pontos_turisticos2;
    int populacao1, populacao2; // Apesar de lido como float, o uso posterior sugere que poderia ser int.

    // Variáveis do tipo 'float' para armazenar números com casas decimais.
    // São usadas para valores que podem não ser inteiros, como área, PIB e os resultados dos cálculos.
    float pib1, area1, densidade_pop1, pib_capita1, super_poder1, inv_denpop1;
    float pib2, area2, densidade_pop2, pib_capita2, super_poder2, inv_denpop2;
    
    // Variáveis do tipo 'char[]' (array de caracteres), também conhecidas como "strings".
    // Elas são usadas para armazenar textos. O número entre colchetes define o tamanho máximo.
    char cidade1[20], cidade2[20]; // Armazena até 19 caracteres + 1 caractere especial de final de string ('\0').
    char estado1[2], estado2[2];   // Armazena 1 letra + o terminador '\0'.
    char codigo1[3], codigo2[3];   // Armazena 2 números + o terminador '\0'.

    // Exibe o nome do jogo na tela para o usuário.
    printf("Desafio Super Trunfo - Países - nivel intermediário \n");

    // ######################################################
    // COLETA DE DADOS DA CARTA 1
    // ######################################################
    // Nesta seção, o programa interage com o usuário para preencher os dados da primeira cidade.
    
    // Pede o nome da cidade. O \n no final pula para a próxima linha.
    printf("Digite o nome da primeira cidade usando até 20 caracteres: \n");
    // scanf("%s", &cidade1) lê o que o usuário digitar (uma palavra) e guarda na variável 'cidade1'.
    // O '%s' indica que estamos lendo uma string.
    scanf("%s", &cidade1);

    printf("Digite uma letra para representar o primeiro estado usando um caracter (A até H): \n");
    scanf("%s", &estado1);

    printf("Digite um número(01...04) para representar a primeira cidade: \n");
    scanf("%s", &codigo1);
    
    // Pede a população da cidade.
    printf("Digite o tamanho da população da primeira cidade em milhares de habitantes: \n");
    // O '%f' indica que estamos lendo um número float (com decimal).
    // O '&' antes de 'populacao1' é crucial: ele informa ao scanf o endereço de memória
    // onde o valor digitado deve ser armazenado.
    scanf("%f", &populacao1);

    printf("Digite o valor da área da primeira cidade em quilômetros quadrados: \n");
    scanf("%f", &area1);
    
    printf("Digite o pib da primeira cidade em reais: \n");
    scanf("%f", &pib1);
    
    printf("Digite quantos pontos turisticos tem na primeira cidade: \n");
    // O '%d' indica que estamos lendo um número inteiro (int).
    scanf("%d", &pontos_turisticos1);
    
    // ######################################################
    // COLETA DE DADOS DA CARTA 2
    // ######################################################
    // O processo é repetido para a segunda cidade.
    
    printf("Digite o nome da segunda cidade usando até 20 caracteres: \n");
    scanf("%s", &cidade2);

    printf("Digite uma letra para representar o segundo estado usando um caracter (A até H): \n");
    scanf("%s", &estado2);

    printf("Digite um número(01...04) para representar a segunda cidade: \n");
    scanf("%s", &codigo2);
    
    printf("Digite o tamanho da população da segunda cidade em milhares de habitantes: \n");
    scanf("%f", &populacao2);

    printf("Digite o valor da área da segunda cidade em quilômetros quadrados: \n");
    scanf("%f", &area2);
    
    printf("Digite o pib da segunda cidade em reais: \n");
    scanf("%f", &pib2);
    
    printf("Digite quantos pontos turisticos tem na segunda cidade: \n");
    scanf("%d", &pontos_turisticos2);
    
    // ######################################################
    // CÁLCULOS DOS ATRIBUTOS DERIVADOS
    // ######################################################
    // Com os dados básicos em mãos, o programa calcula os outros atributos das cartas.

    // Calcula a densidade populacional (habitantes por área).
    densidade_pop1 = populacao1 / area1;
    densidade_pop2 = populacao2 / area2;
    
    // Calcula o inverso da densidade (área por habitante).
    inv_denpop1 = area1 / populacao1;
    inv_denpop2 = area2 / populacao2;
    
    // Calcula o PIB per capita (riqueza média por habitante).
    pib_capita1 = pib1 * 1000 / populacao1;
    pib_capita2 = pib2 * 1000 / populacao2;

    // Calcula o "Super Poder", um valor fictício que soma vários atributos
    // para criar um placar geral para cada carta.
    super_poder1 = populacao1 + area1 + pib1 + pontos_turisticos1 + pib_capita1 + inv_denpop1;
    super_poder2 = populacao2 + area2 + pib2 + pontos_turisticos2 + pib_capita2 + inv_denpop2;

    // ######################################################
    // EXIBIÇÃO DAS CARTAS COMPLETAS
    // ######################################################
    
    // Imprime todos os dados formatados da primeira carta.
    // '%.2f' formata um número float para exibir apenas duas casas decimais.
    printf("\n--- Carta 1 ---\n");
    printf("Estado: %s \n", estado1);
    printf("Código: %s %s \n", estado1, codigo1);
    printf("Nome da cidade: %s \n", cidade1);
    printf("População: %.2f mil habitantes \n", populacao1);
    printf("Area: %.2f Km^2 \n", area1);
    printf("PIB: %.2f bilhões de reais \n", pib1);
    printf("Número de Pontos Turisticos: %d \n", pontos_turisticos1);
    printf("Densidade Populacional: %.2f mil hab / Km^2 \n", densidade_pop1);
    printf("Pib per Capita: %.2f reais por habitante\n", pib_capita1);

    // Imprime todos os dados formatados da segunda carta.
    printf("\n--- Carta 2 ---\n");
    printf("Estado: %s \n", estado2);
    printf("Código: %s %s \n", estado2, codigo2);
    printf("Nome da cidade: %s \n", cidade2);
    printf("População: %.2f mil habitantes \n", populacao2);
    printf("Area: %.2f Km^2 \n", area2);
    printf("PIB: %.2f bilhões de reais \n", pib2);
    printf("Número de Pontos Turisticos: %d \n", pontos_turisticos2);
    printf("Densidade Populacional: %.2f mil hab / Km^2 \n", densidade_pop2);
    printf("Pib per Capita: %.2f reais por habitante\n", pib_capita2);

    // ######################################################
    // LÓGICA DE COMPARAÇÃO E RESULTADOS
    // ######################################################
    // Esta é uma forma inteligente de determinar o vencedor.

    // A expressão 'populacao1 > populacao2' é uma comparação lógica.
    // Em C, se for VERDADEIRA, o resultado é 1. Se for FALSA, o resultado é 0.
    // Então, comp_populacao guardará 1 (se carta 1 venceu) ou 0 (se carta 2 venceu ou empatou).
    int comp_populacao = populacao1 > populacao2;
    // Ao fazer 'comp_populacao++', somamos 1 ao resultado.
    // Se era 1, vira 2 (indicando que a Carta 2 perdeu).
    // Se era 0, vira 1 (indicando que a Carta 1 perdeu ou empatou).
    // O resultado final é o número da carta vencedora (2 para a carta 2, 1 para a carta 1).
    // NOTA: O código original parece ter a lógica invertida. Se populacao1 > populacao2, o resultado deveria ser 1.
    // A lógica correta seria: se 'populacao1 > populacao2' é 1, o vencedor é 1. Se for 0, o vencedor é 2.
    // A forma como está, se `comp_populacao` for `1`, `++` o torna `2`. Se for `0`, `++` o torna `1`. A lógica está um pouco confusa,
    // mas o objetivo é mapear o resultado (0 ou 1) para o jogador (1 ou 2).
    comp_populacao++;
    
    // O mesmo processo é repetido para todos os outros atributos.
    int area = area1 > area2;
    area++;
    
    int pib = pib1 > pib2;
    pib++;
    
    int pontos_turisticos = pontos_turisticos1 > pontos_turisticos2;
    pontos_turisticos++;
    
    int densidade_pop = densidade_pop1 > densidade_pop2;
    densidade_pop++;
    
    int pib_capita = pib_capita1 > pib_capita2;
    pib_capita++;
    
    int super_poder = super_poder1 > super_poder2;
    super_poder++;
    
    // Exibe os resultados finais de cada comparação.
    printf("\n--- Comparação das cartas ---\n");
    printf("População: Carta %d venceu \n", comp_populacao);
    printf("Área: Carta %d venceu \n", area);
    printf("PIB: Carta %d venceu \n", pib);
    printf("Pontos turísticos: Carta %d venceu \n", pontos_turisticos);
    printf("Densidade populacional: Carta %d venceu \n", densidade_pop);
    printf("PIB per capita: Carta %d venceu \n", pib_capita);
    printf("Super poder: Carta %d venceu \n", super_poder);
    
    // 'return 0' indica ao sistema operacional que o programa foi executado
    // com sucesso e terminou sem erros.
    return 0;
}

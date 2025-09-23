#include <stdio.h>


int main() {
  
       int pontos_turisticos1, pontos_turisticos2;
       unsigned int populacao1, populacao2;   
       float pib1, area1, densidade_pop1, pib_capita1, super_poder1, inv_denpop1;
       float pib2, area2, densidade_pop2, pib_capita2, super_poder2, inv_denpop2;
       char cidade1[20], cidade2[20];
       char estado1[2], estado2[2];
       char codigo1[3], codigo2[3];


       // nome do aplicativo
       printf("Desafio Super Trunfo - Países - nivel mestre \n");
  
       // Cadastrando a primeira cidade
       printf("Digite o nome da primeira cidade usando até 20 caracteres: \n");
       scanf("%s", &cidade1);


       // Cadastrando o estado da primeira cidade
       printf("Digite uma letra para representar o primeiro estado usando um caracter (A até H): \n");
       scanf("%s", &estado1);


       // Cadastrando o código da primeira cidade
       printf("Digite um número(01...04) para representar a primeira cidade: \n");
       scanf("%s", &codigo1);
      
      // Colocando o valor da população da primeira cidade
       printf("Digite o tamanho da população da primeira cidade em milhares de habitantes: \n");
       scanf("%u", &populacao1);


    // Colocando a área da primeira cidade
       printf("Digite o valor da área da primeira cidade em quilômetros quadrados: \n");
       scanf("%f", &area1);
      
      // Colocando o pib da primeira cidade
     printf("Digite o pib da primeira cidade em reais: \n");
       scanf("%f", &pib1);
  
       // Colocando o pib da primeira cidade
     printf("Digite quantos pontos turisticos tem na primeira cidade: \n");
       scanf("%d", &pontos_turisticos1);
      
       // Cadastrando a segunda cidade
       printf("Digite o nome da segunda cidade usando até 20 caracteres: \n");
       scanf("%s", &cidade2);


       // Cadastrando o estado da segunda cidade
       printf("Digite uma letra para representar o segundo estado usando um caracter (A até H): \n");


       // Cadastrando o código da segunda cidade
       printf("Digite um número(01...04) para representar a segunda cidade: \n");
       scanf("%s", &codigo2);
      
      // Colocando o valor da população da segunda cidade
       printf("Digite o tamanho da população da segunda cidade em milhares de habitantes: \n");
       scanf("%u", &populacao2);


    // Colocando a área da segunda cidade
       printf("Digite o valor da área da segunda cidade em quilômetros quadrados: \n");
       scanf("%f", &area2);
      
      // Colocando o pib da segunda cidade
       printf("Digite o pib da segunda cidade em reais: \n");
       scanf("%f", &pib2);
  
       // Colocando o pib da segunda cidade
       printf("Digite quantos pontos turisticos tem na segunda cidade: \n");
       scanf("%d", &pontos_turisticos2);
      
    // Calculando a densidade populacional das duas cidades
       densidade_pop1 = populacao1 / area1;
       inv_denpop1 = area1 / populacao1;
       densidade_pop2 = populacao2 / area2;
       inv_denpop2 = area2 / populacao2;
      
    // Calculando o PIB por habitante das duas cidades
       pib_capita1 = pib1 * 1000 / populacao1;
       pib_capita2 = pib2 * 1000 / populacao2;


       // Calculando o Super Poder
       super_poder1 = populacao1 + area1 + pib1 + pontos_turisticos1 + pib_capita1 + inv_denpop1;
       super_poder2 = populacao2 + area2 + pib2 + pontos_turisticos2 + pib_capita2 + inv_denpop2;


       // A impressão da primeira carta       
       printf("Carta 1: \n");
       printf("Estado: %s \n", estado1);
       printf("Código: %s %s \n", estado1, codigo1);
       printf("Nome da cidade: %s \n", cidade1);
       printf("População: %.2u mil habitantes \n", populacao1);
       printf("Area: %.2f Km\u00B2 \n", area1);
       printf("PIB: %.2f bilhões de reais \n", pib1);
       printf("Número de Pontos Turisticos: %d \n", pontos_turisticos1);
       printf("Densidade Populacional: %.2f mil hab / Km\u00B2 \n", densidade_pop1);
       printf("Pib per Capita: %.2f reais por habitante\n", pib_capita1);


       // A impressão da segunda carta
       printf("Carta 2: \n");
       printf("Estado: %s \n", estado2);
       printf("Código: %s %s \n", estado2, codigo2);
       printf("Nome da cidade: %s \n", cidade2);
       printf("População: %.2u mil habitantes \n", populacao2);
       printf("Area: %.2f Km\u00B2 \n", area2);
       printf("PIB: %.2f bilhões de reais \n", pib2);
       printf("Número de Pontos Turisticos: %d \n", pontos_turisticos2);
       printf("Densidade Populacional: %.2f mil hab / Km\u00B2 \n", densidade_pop2);
       printf("Pib per Capita: %.2f reais por habitante\n", pib_capita2);


       // 0 = carta 1, 1 = carta 2
       int populacao = populacao1 < populacao2;
       populacao++;
      
       int area = area1 < area2;
       area++;
      
       int pib = pib1 < pib2;
       pib++;
      
       int pontos_turisticos = pontos_turisticos1 < pontos_turisticos2;
       pontos_turisticos++;
      
       int densidade_pop = densidade_pop1 > densidade_pop2;
       densidade_pop++;
      
       int pib_capita = pib_capita1 < pib_capita2;
       pib_capita++;
      
       int super_poder = super_poder1 < super_poder2;
       super_poder++;


       int escolha1, escolha2;


       // Escolhendo a propriedade para fazer comparação das cidades
       printf("Quais propriedades você quer comparar?: \n");
       printf("1. População \n");
       printf("2. Área \n");
       printf("3. PIB \n");
       printf("4. Pontos turísticos \n");
       printf("5. Densidade populacional \n");
       printf("6. PIB per capita\n");
       printf("7. Super poder \n");
       // o usuário fará as suas escolhas
       printf("Escolha a primeira propriedade: \n");
       scanf("%d", &escolha1);
       printf("Escolha a segunda propriedade, devendo ser diferente da primeira: \n");
       scanf("%d", &escolha2);
      
       if (escolha1 == escolha2)
       { printf("Você escolheu a mesma propriedade que a primeira, escolha outra propriedade. \n");
       scanf("%d", &escolha2);
       } else {}
      
       // Comparação das cartas
       printf("Comparação da primeira propriedade escolhida: \n");
      
       float pontuacao1carta1, pontuacao1carta2;
      
       switch (escolha1)
       {           case 1:
                               // comparação das populações
                               if (populacao1 == populacao2)
                               {           printf("Carta1 - %s, População: %u mil hab.\n", cidade1, populacao1);
                                            printf("Carta2 - %s, População: %u mil hab.\n", cidade2, populacao2);
                                            printf("População: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, População: %u mil hab.\n", cidade1, populacao1);
                                           printf("Carta2 - %s, População: %u mil hab.\n", cidade2, populacao2);
                                           printf("População: Carta %d venceu \n", populacao);
                                           pontuacao1carta1 = populacao1;
                                           pontuacao1carta2 = populacao2;
                                           }
                               break;
                   case 2:
                               // comparação das areas
                               if (area1 == area2)
                               {           printf("Carta1 - %s, Área: %.2f km\u00B2 \n", cidade1, area1);
                                           printf("Carta2 - %s, Área: %.2f km\u00B2 \n", cidade2, area2);
                                            printf("Área: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, Área: %.2f km\u00B2 \n", cidade1, area1);
                                           printf("Carta2 - %s, Área: %.2f km\u00B2 \n", cidade2, area2);
                                           printf("Área: Carta %d venceu \n", area);
                                           pontuacao1carta1 = area1;
                                           pontuacao1carta2 = area2;
                                           }
                               break;
                   case 3:
                               // comparação dos PIBs
                               if (pib1 == pib2)
                               {          printf("Carta1 - %s, PIB: %.2f bilhões de reais\n", cidade1, pib1);
                                           printf("Carta2 - %s, PIB: %.2f bilhões de reais \n", cidade2, pib2);
                                           printf("PIB: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, PIB: %.2f bilhões de reais\n", cidade1, pib1);
                                           printf("Carta2 - %s, PIB: %.2f bilhões de reais \n", cidade2, pib2);
                                           printf("PIB: Carta %d venceu \n", pib);
                                           pontuacao1carta1 = pib1;
                                           pontuacao1carta2 = pib2;
                                           }
                               break;
                   case 4:
                               // comparação dos pontos turisticos
                               if (pontos_turisticos1 == pontos_turisticos2)
                               {          printf("Carta1 - %s, Pontos turísticos: %d\n", cidade1, pontos_turisticos1);
                                           printf("Carta2 - %s, Pontos turísticos: %d\n", cidade2, pontos_turisticos2);
                                           printf("Pontos turísticos: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, Pontos turísticos: %d\n", cidade1, pontos_turisticos1);
                                           printf("Carta2 - %s, Pontos turísticos: %d\n", cidade2, pontos_turisticos2);
                                           printf("Pontos turísticos: Carta %d venceu \n", pontos_turisticos);
                                           pontuacao1carta1 = pontos_turisticos1;
                                           pontuacao1carta2 = pontos_turisticos2;
                                           }
                               break;
                   case 5:
                               // comparação das densidade populacionais
                               if (densidade_pop1 == densidade_pop2)
                               {          printf("Carta1 - %s, Densidade populacional: %.2f mil hab. / km\u00B2 \n", cidade1, densidade_pop1);
                                           printf("Carta2 - %s, Densidade populacional: %.2f mil hab. / km\u00B2 \n", cidade2, densidade_pop2);
                                           printf("Densidade populacional: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, Densidade populacional: %.2f mil hab. / km\u00B2 \n", cidade1, densidade_pop1);
                                           printf("Carta2 - %s, Densidade populacional: %.2f mil hab. / km\u00B2 \n", cidade2, densidade_pop2);
                                           printf("Densidade populacional: Carta %d venceu \n", densidade_pop);
                                           pontuacao1carta1 = densidade_pop1;
                                           pontuacao1carta2 = densidade_pop2;
                                           }
                               break;
                   case 6:
                               // comparação dos pib per capitas
                               if (pib_capita1 == pib_capita2)
                               {          printf("Carta1 - %s, Pib per capita: %.2f reais / hab. \n", cidade1, pib_capita1);
                                           printf("Carta2 - %s, Pib per capita: %.2f reais / hab. \n", cidade2, pib_capita2);
                                           printf("PIB per capita: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, Pib per capita: %.2f reais / hab. \n", cidade1, pib_capita1);
                                           printf("Carta2 - %s, Pib per capita: %.2f reais / hab. \n", cidade2, pib_capita2);
                                           printf("PIB per capita: Carta %d venceu \n", pib_capita);
                                           pontuacao1carta1 = pib_capita1;
                                           pontuacao1carta2 = pib_capita2;
                                           }
                               break;
                   case 7:
                               // comparação dos super_poders
                               if (super_poder1 == super_poder2)
                               {          printf("Carta1 - %s, Super poder: %.2f\n", cidade1, super_poder1);
                                           printf("Carta2 - %s, Super poder: %.2f\n", cidade2, super_poder2);
                                           printf("Super poder: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, Super poder: %.2f\n", cidade1, super_poder1);
                                           printf("Carta2 - %s, Super poder: %.2f\n", cidade2, super_poder2);
                                           printf("Super poder: Carta %d venceu \n", super_poder);
                                           pontuacao1carta1 = super_poder1;
                                           pontuacao1carta2 = super_poder2;
                                           }
                               break;
                   default:
                               printf("Opção inválida. \n"); break;
       }
       // Comparação das cartas
       printf("Comparação da segunda propriedade escolhida: \n");
      
       float pontuacao2carta1, pontuacao2carta2;
      
       switch (escolha2)
       {           case 1:
                               // comparação das populações
                               if (populacao1 == populacao2)
                               {           printf("Carta1 - %s, População: %u mil hab.\n", cidade1, populacao1);
                                            printf("Carta2 - %s, População: %u mil hab.\n", cidade2, populacao2);
                                            printf("População: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, População: %u mil hab.\n", cidade1, populacao1);
                                           printf("Carta2 - %s, População: %u mil hab.\n", cidade2, populacao2);
                                           printf("População: Carta %d venceu \n", populacao);
                                           pontuacao2carta1 = populacao1;
                                           pontuacao2carta2 = populacao2;
                                           }
                               break;
                   case 2:
                               // comparação das areas
                               if (area1 == area2)
                               {           printf("Carta1 - %s, Área: %.2f km\u00B2 \n", cidade1, area1);
                                           printf("Carta2 - %s, Área: %.2f km\u00B2 \n", cidade2, area2);
                                            printf("Área: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, Área: %.2f km\u00B2 \n", cidade1, area1);
                                           printf("Carta2 - %s, Área: %.2f km\u00B2 \n", cidade2, area2);
                                           printf("Área: Carta %d venceu \n", area);
                                           pontuacao2carta1 = area1;
                                           pontuacao2carta2 = area2;
                                           }
                               break;
                   case 3:
                               // comparação dos PIBs
                               if (pib1 == pib2)
                               {          printf("Carta1 - %s, PIB: %.2f bilhões de reais\n", cidade1, pib1);
                                           printf("Carta2 - %s, PIB: %.2f bilhões de reais \n", cidade2, pib2);
                                           printf("PIB: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, PIB: %.2f bilhões de reais\n", cidade1, pib1);
                                           printf("Carta2 - %s, PIB: %.2f bilhões de reais \n", cidade2, pib2);
                                           printf("PIB: Carta %d venceu \n", pib);
                                           pontuacao2carta1 = pib1;
                                           pontuacao2carta2 = pib2;
                                           }
                               break;
                   case 4:
                               // comparação dos pontos turisticos
                               if (pontos_turisticos1 == pontos_turisticos2)
                               {          printf("Carta1 - %s, Pontos turísticos: %d\n", cidade1, pontos_turisticos1);
                                           printf("Carta2 - %s, Pontos turísticos: %d\n", cidade2, pontos_turisticos2);
                                           printf("Pontos turísticos: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, Pontos turísticos: %d\n", cidade1, pontos_turisticos1);
                                           printf("Carta2 - %s, Pontos turísticos: %d\n", cidade2, pontos_turisticos2);
                                           printf("Pontos turísticos: Carta %d venceu \n", pontos_turisticos);
                                           pontuacao2carta1 = pontos_turisticos1;
                                           pontuacao2carta2 = pontos_turisticos2;
                                           }
                               break;
                   case 5:
                               // comparação das densidade populacionais
                               if (densidade_pop1 == densidade_pop2)
                               {          printf("Carta1 - %s, Densidade populacional: %.2f mil hab. / km\u00B2 \n", cidade1, densidade_pop1);
                                           printf("Carta2 - %s, Densidade populacional: %.2f mil hab. / km\u00B2 \n", cidade2, densidade_pop2);
                                           printf("Densidade populacional: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, Densidade populacional: %.2f mil hab. / km\u00B2 \n", cidade1, densidade_pop1);
                                           printf("Carta2 - %s, Densidade populacional: %.2f mil hab. / km\u00B2 \n", cidade2, densidade_pop2);
                                           printf("Densidade populacional: Carta %d venceu \n", densidade_pop);
                                           pontuacao2carta1 = densidade_pop1;
                                           pontuacao2carta2 = densidade_pop2;
                                           }
                               break;
                   case 6:
                               // comparação dos pib per capitas
                               if (pib_capita1 == pib_capita2)
                               {          printf("Carta1 - %s, Pib per capita: %.2f reais / hab. \n", cidade1, pib_capita1);
                                           printf("Carta2 - %s, Pib per capita: %.2f reais / hab. \n", cidade2, pib_capita2);
                                           printf("PIB per capita: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, Pib per capita: %.2f reais / hab. \n", cidade1, pib_capita1);
                                           printf("Carta2 - %s, Pib per capita: %.2f reais / hab. \n", cidade2, pib_capita2);
                                           printf("PIB per capita: Carta %d venceu \n", pib_capita);
                                           pontuacao2carta1 = pib_capita1;
                                           pontuacao2carta2 = pib_capita2;
                                           }
                               break;
                   case 7:
                               // comparação dos super_poders
                               if (super_poder1 == super_poder2)
                               {          printf("Carta1 - %s, Super poder: %.2f\n", cidade1, super_poder1);
                                           printf("Carta2 - %s, Super poder: %.2f\n", cidade2, super_poder2);
                                           printf("Super poder: Houve empate \n");
                               } else {
                                           printf("Carta1 - %s, Super poder: %.2f\n", cidade1, super_poder1);
                                           printf("Carta2 - %s, Super poder: %.2f\n", cidade2, super_poder2);
                                           printf("Super poder: Carta %d venceu \n", super_poder);
                                           pontuacao2carta1 = super_poder1;
                                           pontuacao2carta2 = super_poder2;
                                            }
                               break;
                   default:
                               printf("Opção inválida. \n"); break;
                  
       }
      
       float totalcarta1= pontuacao1carta1+ pontuacao2carta1;
       float totalcarta2= pontuacao1carta2+ pontuacao2carta2;
      
               // Comparação das duas propriedades das duas cartas
               printf("Comparação da soma dos valores das propriedades escolhidas: \n");
               printf("O total de pontos da carta 1 é %f. \n", totalcarta1);
               printf("O total de pontos da carta 2 é %f. \n", totalcarta2);
               totalcarta1 > totalcarta2 ? printf("Carta 1 é vencedora") : printf("Carta 2 é vencedora");
      
       return 0;
   }


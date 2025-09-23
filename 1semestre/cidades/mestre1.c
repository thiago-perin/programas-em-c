#include <stdio.h>

int main() {
	
		int pontos_turisticos1, pontos_turisticos2;
		int populacao1, populacao2;    
		float pib1, area1, densidade_pop1, pib_capita1, super_poder1, inv_denpop1;
		float pib2, area2, densidade_pop2, pib_capita2, super_poder2, inv_denpop2;
		char cidade1[20], cidade2[20];
    	char estado1[2], estado2[2];
		char codigo1[3], codigo2[3];

		// nome do aplicativo
    	printf("Desafio Super Trunfo - Países - nivel intermediário \n");
    
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
		scanf("%f", &populacao1);

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
		scanf("%s", &estado2);

		// Cadastrando o código da segunda cidade
		printf("Digite um número(01...04) para representar a segunda cidade: \n");
		scanf("%s", &codigo2);
		
 	   // Colocando o valor da população da segunda cidade
		printf("Digite o tamanho da população da segunda cidade em milhares de habitantes: \n");
		scanf("%f", &populacao2);

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
		printf("População: %.2f mil habitantes \n", populacao1);
		printf("Area: %.2f Kmˆ2 \n", area1);
		printf("PIB: %.2f bilhões de reais \n", pib1);
		printf("Número de Pontos Turisticos: %d \n", pontos_turisticos1);
		printf("Densidade Populacional: %.2f mil hab / Kmˆ2 \n", densidade_pop1);
		printf("Pib per Capita: %.2f reais por habitante\n", pib_capita1);

		// A impressão da segunda carta
		printf("Carta 2: \n");
		printf("Estado: %s \n", estado2);
		printf("Código: %s %s \n", estado2, codigo2);
		printf("Nome da cidade: %s \n", cidade2);
		printf("População: %.2f mil habitantes \n", populacao2);
		printf("Area: %.2f Kmˆ2 \n", area2);
		printf("PIB: %.2f bilhões de reais \n", pib2);
		printf("Número de Pontos Turisticos: %d \n", pontos_turisticos2);
		printf("Densidade Populacional: %.2f mil hab / Kmˆ2 \n", densidade_pop2);
		printf("Pib per Capita: %.2f reais por habitante\n", pib_capita2);

		int comp_populacao = populacao1 > populacao2;
		comp_populacao++;
		
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
		
		printf("Comparação das cartas \n");
		// comparação das populações
		printf("População: Carta %d venceu \n", comp_populacao);
		// comparação das areas
		printf("Área: Carta %d venceu \n", area);
    	// comparação dos PIBs
		printf("PIB: Carta %d venceu \n", pib);
		// comparação dos pontos turisticos
		printf("Pontos turísticos: Carta %d venceu \n", pontos_turisticos);
		// comparação dos densidade_pops
		printf("Densidade populacional: Carta %d venceu \n", densidade_pop);
		// comparação dos pib per capitas
		printf("PIB per capita: Carta %d venceu \n", pib_capita);
		// comparação dos super_poders
		printf("Super poder: Carta %d venceu \n", super_poder);
		
        return 0;
    }

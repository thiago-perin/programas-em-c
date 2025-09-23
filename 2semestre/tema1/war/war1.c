// ##################
// BIBLIOTECA PARTE 1
// ##################

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // biblioteca que tem strings 

// Adicione as duas linhas abaixo
#define TAM_STRING 20
#define TAM_INT 3

// ######################
// Definição da estrutura
// ######################

struct Territorio {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropa;
};

// ######################################
// Função para limpar o buffer de entrada
// ######################################

void limparBufferEntrada() {
    int c;
    while (
        (c = getchar()
        ) != '\n' && c != EOF);
}

// ######################################
// Função Principal (main)
// ######################################
int main () {
    int MAX_TER = 5;
    struct Territorio biblioteca[MAX_TER];
    int totalTerritorios = 0;
    int opcao;

    // --- Laço Principal do Menu ---
    do {

        // Exibe o menu de opções
        printf("#########################################\n");
        printf("     WAR - TERRITORIOS \n");
        printf("#########################################\n");
        printf("1 - Cadastrar novo Territorio\n");
        printf("2 - Listar todos os Territorios\n");
        printf("0 - Sair do jogo\n");
        printf("-----------------------------------------\n");
        printf("Escolha uma opcao: ");
        
        // Lê a opção do usuário.
        scanf("%d", &opcao);
        limparBufferEntrada(); // Limpar o '\n' deixado pelo scanf.

        // --- Processamento da Opção ---
        switch (opcao) {
            case 1: // CADASTRO DE TERRITORIO

            	// 1. Verifica se o cadastro inicial ja foi feito

				if (totalTerritorios > 0) {
					printf("\nAlguns territorios iniciais ja foram cadastrados!\n");
				} else {
					// 2. Inicia o cadastro em looping
                printf("Vamos cadastrar os 5 territorios iniciais do nosso mundo \n\n");
				
				for (int i = 0; i < MAX_TER; i++) {
				    printf("- - - Cadastrando o %dº Territorio - - - \n", i + 1);

                    printf("Nome do Territorio: ");
                    fgets (biblioteca[i].nome, TAM_STRING, stdin);

                    printf("Cor do Exercito (ex: Azul, verde, etc):");
                    fgets (biblioteca[i].cor, TAM_STRING, stdin);

                    printf("Numero de tropas: ");
                    scanf("%d", &biblioteca[i].tropa);
					limparBufferEntrada(); // Limpar o buffer apos o scanf

					// Remove o ˋ\nˋ que o fgets deixa no final
                    biblioteca[i].nome[strcspn(biblioteca[i].nome, "\n")] = '\0';
                    biblioteca[i].cor[strcspn(biblioteca[i].cor, "\n")] = '\0';

                    printf(" --> Territorio %d cadastrado com sucesso!\n\n", i + 1);
                } 
                
                // 3. Atualiza o contador de uma vez so no final
                totalTerritorios = MAX_TER;
                printf("##############################\n");
                printf("Cadastro inicial concluido com sucesso\n");
                printf("##############################\n");                
			}
   
            printf("\nPressione Enter para continuar...");
            getchar(); // Pausa para o usuario ler a mensagem antes de voltar para o menu.
                break;
            
            case 2: // LISTAGEM DE TERRITORIOS
                printf("##############################\n");            
                printf("- - - MAPA DO MUNDO - ESTADO ATUAL - - -\n");
                printf("##############################\n\n");
                if(totalTerritorios == 0) {
                    printf("Nenhum Territorio cadastrado ainda. \n");
                } else {
                    for (int i = 0; i < totalTerritorios; i++) {

                        printf("Territorio %d ;\n", i + 1);
                        printf("    - Nome: %s\n", biblioteca[i].nome);
                        printf("    - Dominado por: Exercito %s\n", biblioteca[i].cor);
                        printf("    - Tropas: %d\n", biblioteca[i].tropa);
                    } // fecha o for
                    printf("---------------------------------------\n");
                } // fecha o if {}else {}
                    // A pausa é crucial para que o usuário veja a lista antes
                    // do próximo loop limpar a tela.
                    printf("\n Pressione Enter para continuar...");
                    getchar();
                    break;

            case 0: // SAIR
                printf("\n Saindo do Sistema...\n");
                break;
            
            default: // OPÇÃO INVÁLIDA
                printf("\n Opção Invalida! Tente novamente. \n");
                printf("\n Pressione Enter para continuar...");
                getchar();
                break;
        }
    } while (opcao != 0); // Adicione esta linha para fechar o "do"
return 0;
} // fecha o main

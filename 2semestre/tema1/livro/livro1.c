// ##################
// BIBLIOTECA PARTE 1
// ##################

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // biblioteca que tem strings 

// Adicione as duas linhas abaixo
#define TAM_STRING 100 
#define MAX_LIVROS 50

// ######################
// Definição da estrutura
// ######################

struct Livro {
    char nome[TAM_STRING];
    char autor[TAM_STRING];
    char editora[TAM_STRING];
    int edicao;
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
    struct Livro biblioteca[MAX_LIVROS];
    int totalLivros = 0;
    int opcao;

    // --- Laço Principal do Menu ---
    do {

        // Exibe o menu de opções
        printf("#########################################\n");
        printf("     BIBLIOTECA - PARTE Q \n");
        printf("#########################################\n");
        printf("1 - Cadastrar novo livro\n");
        printf("2 - Listar todos os livro\n");
        printf("0 - Sair  livro\n");
        printf("-----------------------------------------\n");
        printf("Escolha uma opção: ");
        
        // Lê a opção do usuário.
        scanf("%d", &opcao);
        limparBufferEntrada(); // Limpar o '\n' deixado pelo scanf.

        // --- Processamento da Opção ---
        switch (opcao) {
            case 1: // CADASTRO DE LIVRO
                printf("-- Cadastro de novo Livro --- \n\n");

                if (totalLivros < MAX_LIVROS) {
                    printf("Digite o nome do livro: ");
                    fgets (biblioteca[totalLivros].nome, TAM_STRING, stdin);

                    printf("Digite o autor do livro: ");
                    fgets (biblioteca[totalLivros].autor, TAM_STRING, stdin);

                    printf("Digite a editora do livro: ");
                    fgets (biblioteca[totalLivros].editora, TAM_STRING, stdin);

                    biblioteca[totalLivros].nome[strcspn(biblioteca[totalLivros].nome, "\n")] = '\0';
                    biblioteca[totalLivros].autor[strcspn(biblioteca[totalLivros].autor, "\n")] = '\0';
                    biblioteca[totalLivros].editora[strcspn(biblioteca[totalLivros].editora, "\n")] = '\0';

                    printf("Digite a edicao: ");
                    scanf("%d", &biblioteca[totalLivros].edicao);
                    limparBufferEntrada();

                    totalLivros++;

                    printf("\n Livro cadastrado com sucesso!\n");
                } else {
                    printf("Biblioteca cheia! Não é possível cadastrar mais livros. \n");
                }

                printf("Pressione Enter para continuar...");
                getchar(); // Pausa para o usuario ler a mensagem antes de voltar para o menu.
                break;
            
            case 2: // LISTAGEM DE LIVROS
                printf("--- Lista de Livros Cadastrados ---\n\n");

                if(totalLivros == 0) {
                    printf("Nenhum Livro cadastrado ainda. \n");
                } else {
                    for (int i = 0; i < totalLivros; i++) {
                        printf("----------------------------------------\n");
                        printf("Livro: %d\n", i + 1);
                        printf("Nome: %s\n", biblioteca[i].nome);
                        printf("Autor: %s\n", biblioteca[i].autor);
                        printf("Editora: %s\n", biblioteca[i].editora);
                        printf("Edicao: %d\n", biblioteca[i].edicao);
                    }
                    printf("---------------------------------------\n");
                }
                    // A pausa é crucial para que o usuário veja a lista antes
                    // do próximo loop limpar a tela.
                    printf("\n Pressione Enter para continuar...");
                    getchar();
                    break;

            caso 0: // SAIR
                printf("\n Saindo do Sistema...\n");
                break;
            
            default: // OPÇÃO INVÁLIDA
                printf("\n Opção Inválida! Tente novamente. \n");
                printf("\n Pressione Enter para continuar...");
                getchar();
                break;
        }
    } while (opcao != 0); // Adicione esta linha para fechar o "do"
}

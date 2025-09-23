// ######################################################
// SISTEMA DE BIBLIOTECA - PARTE 3
//
// OBJETIVOS:
// Modularizar o código com funções especializadas, aplicando boas práticas.
// Passagem por valor (para exibição) e por referência (para modificação).
// Funções para cada funcionalidade principal do sistema.
// A função main() se torna um 'orquestrador', chamando outras
// ######################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ######################
// Constantes globais
// ######################

#define MAX_LIVROS 50
#define MAX_EMPRESTIMOS 100 // CAPACIDADE MÁXIMA DE EMPRÉSTIMOS
#define TAM_STRING 100

// ######################
// Definição da estrutura
// Struct Livro agora tem um campo 'disponível' para controlar o status.
// ######################

struct Livro {
    char nome[TAM_STRING];
    char autor[TAM_STRING];
    char editora[TAM_STRING];
    int edicao;
    int disponivel; // 1 para sim e 0 para não
};

// ######################################
// Definição de empréstimo
// Nova struct para armazenar informações de empréstimo.
// ######################################

struct Emprestimo {
    int indiceLivro; // para saber qual livro do array 'biblioteca' foi emprestado.
    char nomeUsuario[TAM_STRING];
};

// ######################################################################################
// - - - Prototipos das funcoes - - -
// Declarar as funções aqui permite que a 'main' as chame antes das suas definições.
// ######################################################################################

void limparBufferEntrada();
void exibirMenu();
void cadastrarLivro(struct Livro *biblioteca, int *totalLivros);
void listarLivros(const struct Livro *biblioteca, int totalLivros);
void realizarEmprestimo(struct Livro *biblioteca, int totalLivros, struct Emprestimo *emprestimos, int *totalEmprestimos);
void listarEmprestimos(const struct Livro *biblioteca, const struct Emprestimo *emprestimos, int totalEmprestimos);
void liberarMemoria(struct Livro *biblioteca, struct Emprestimo *emprestimos);

// ################################################################
// Função Principal (main)
// Agora o main é mais limpa. Ela aloca memoria, controla o menu
// e delega as tarefas para as funções especializadas
// ################################################################

int main () {
    struct Livro *biblioteca = (struct Livro *) calloc (MAX_LIVROS, sizeof(struct Livro));
    struct Emprestimo *emprestimos = (struct Emprestimo *) malloc (MAX_EMPRESTIMOS * sizeof(struct Emprestimo));

    if (biblioteca == NULL || emprestimos == NULL) {
        printf("Erro: Falha ao alocar memória. \n");
        return 1; // Retorna 1 para indicar um erro.
    }

    int totalLivros = 0;
    int totalEmprestimos = 0;
    int opcao;

    // --- Laço Principal do Menu ---
    do {

        exibirMenu();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {

            case 1:
                cadastrarLivro(biblioteca, &totalLivros);
                break;
            
            case 2:
                listarLivros(biblioteca, totalLivros);
                break;

            case 3:
                realizarEmprestimo(biblioteca, totalLivros, emprestimos, &totalEmprestimos);
                break;
            
            case 4:
                listarEmprestimos(biblioteca, emprestimos, totalEmprestimos);
                break;

            case 0: // SAIR
                printf("\n Saindo do Sistema...\n");
                break;
            
            default: // OPÇÃO INVÁLIDA
                printf("\n Opcao Invalida! Pressione Enter para tentar novamente.");
                getchar();
                break;
        }
    } while (opcao != 0);

    // LIBERAÇÃO DA MEMÓRIA
    liberarMemoria(biblioteca, emprestimos);

    return 0;
}

// - - - Implementação das funções - - -

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirMenu() {
    printf("\n#########################################\n");
    printf("     BIBLIOTECA - PARTE 3 \n");
    printf("#########################################\n");
    printf("1 - Cadastrar novo livro\n");
    printf("2 - Listar todos os livros\n");
    printf("3 - Realizar emprestimo\n");
    printf("4 - Listar emprestimos\n");
    printf("0 - Sair da biblioteca\n");
    printf("-----------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/**
 * @brief Cadastra um novo livro
 * @param biblioteca O array de Livros
 * @param totalLivros Ponteiro para o contador de Livros para que ele seja atualizado
 */
void cadastrarLivro (struct Livro *biblioteca, int *totalLivros) {
    printf("-- Cadastro de novo Livro ---\n\n");

    if (*totalLivros < MAX_LIVROS) {
        int indice = *totalLivros;

        printf("Digite o nome do livro: ");
        fgets (biblioteca[indice].nome, TAM_STRING, stdin);

        printf("Digite o autor do livro: ");
        fgets (biblioteca[indice].autor, TAM_STRING, stdin);

        printf("Digite a editora do livro: ");
        fgets (biblioteca[indice].editora, TAM_STRING, stdin);

        biblioteca[indice].nome[strcspn(biblioteca[indice].nome, "\n")] = '\0';
        biblioteca[indice].autor[strcspn(biblioteca[indice].autor, "\n")] = '\0';
        biblioteca[indice].editora[strcspn(biblioteca[indice].editora, "\n")] = '\0';

        printf("Digite a edicao: ");
        scanf("%d", &biblioteca[indice].edicao);
        limparBufferEntrada();

        biblioteca[indice].disponivel = 1; // Define o livro como disponível
        
        (*totalLivros)++; // Incrementa o valor para o qual o ponteiro aponta

        printf("\nLivro cadastrado com sucesso!\n");
    } else {
        printf("Biblioteca cheia! Nao e possivel cadastrar mais livros.\n");
    }
    printf("\nPressione Enter para continuar...");
    getchar();
}

/**
 * @brief Lista todos os livros cadastrados
 * @param biblioteca O array (ponteiro constante, apenas para leitura).
 * @param totalLivros O número de Livros (passagem por valor)
 */
void listarLivros (const struct Livro *biblioteca, int totalLivros) {
    printf("\n--- Lista de Livros Cadastrados ---\n\n");

    if(totalLivros == 0) {
        printf("Nenhum livro cadastrado ainda.\n");
    } else {
        for (int i = 0; i < totalLivros; i++) {
            printf("----------------------------------------\n");
            printf("Livro: %d\n", i + 1);
            printf("Nome: %s\n", biblioteca[i].nome);
            printf("Autor: %s\n", biblioteca[i].autor);
            printf("Editora: %s\n", biblioteca[i].editora);
            printf("Edicao: %d\n", biblioteca[i].edicao);
            printf("Status: %s\n", biblioteca[i].disponivel ? "Disponivel" : "Emprestado");
        }
        printf("---------------------------------------\n");
    }
    printf("\nPressione Enter para continuar...");
    getchar();
}

/**
 * @brief Realiza o emprestimo de um livro.
 * @param biblioteca O array de livros (altera a disponibilidade do livro).
 * @param totalLivros O número de Livros (passagem por valor)
 * @param emprestimos Array de emprestimos (modificavel)
 * @param totalEmprestimos Ponteiro para o contador de emprestimos (para ser atualizado)
 */
void realizarEmprestimo(struct Livro *biblioteca, int totalLivros, struct Emprestimo *emprestimos, int *totalEmprestimos) {
    printf("\n--- Realizar Emprestimo ---\n\n");

    if (*totalEmprestimos >= MAX_EMPRESTIMOS) {
        printf("Limite de emprestimos atingido!\n");
    } else {
        printf("Livros disponiveis para emprestimo:\n");
        int disponiveis = 0;
        for (int i = 0; i < totalLivros; i++) {
            if (biblioteca[i].disponivel) {
                printf("%d - %s\n", i + 1, biblioteca[i].nome);
                disponiveis++;
            }
        }

        if (disponiveis == 0) {
            printf("\nNenhum livro disponivel para emprestimo no momento.\n");
        } else {
            printf("\nDigite o numero do livro que deseja emprestar: ");
            int numLivro;
            scanf("%d", &numLivro);
            limparBufferEntrada();

            int indice = numLivro - 1;

            if (indice >= 0 && indice < totalLivros && biblioteca[indice].disponivel) {
                printf("Digite o nome do usuario que esta pegando o livro: ");
                fgets(emprestimos[*totalEmprestimos].nomeUsuario, TAM_STRING, stdin);
                emprestimos[*totalEmprestimos].nomeUsuario[strcspn(emprestimos[*totalEmprestimos].nomeUsuario, "\n")] = '\0';
                
                emprestimos[*totalEmprestimos].indiceLivro = indice;
                biblioteca[indice].disponivel = 0;

                (*totalEmprestimos)++; // Incrementa o valor para o qual o ponteiro aponta
                printf("\nEmprestimo realizado com sucesso!\n");
            } else {
                printf("\nNumero de livro invalido ou livro indisponivel.\n");
            }
        }
    }
    printf("\nPressione Enter para continuar...");
    getchar();
}

/**
 * @brief Lista todos os emprestimos realizados.
 * @param biblioteca O array de livros (leitura).
 * @param emprestimos Array de emprestimos (leitura).
 * @param totalEmprestimos Total de emprestimos (valor).
 */
void listarEmprestimos(const struct Livro *biblioteca, const struct Emprestimo *emprestimos, int totalEmprestimos) {
    printf("\n--- Lista de Emprestimos Realizados ---\n\n");
    if (totalEmprestimos == 0 ) {
        printf("Nenhum emprestimo realizado ainda.\n");
    } else {
        for (int i = 0; i < totalEmprestimos; i++) {
            int indiceLivro = emprestimos[i].indiceLivro;
            printf("---------------------------------\n");
            printf("EMPRESTIMO %d\n", i + 1);
            printf("Livro: %s\n", biblioteca[indiceLivro].nome);
            printf("Usuario: %s\n", emprestimos[i].nomeUsuario);
        }
        printf("-----------------------------------\n");
    }
    printf("\nPressione Enter para continuar...");
    getchar();
}

/**
 * @brief Libera a memória alocada dinamicamente para os arrays.
 */
void liberarMemoria(struct Livro *biblioteca, struct Emprestimo *emprestimos) {
    free(biblioteca);
    free(emprestimos);
    printf("Memoria liberada com sucesso.\n");
}

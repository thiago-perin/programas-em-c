// ######################################################
// BIBLIOTECA PARTE 2
//
// OBJETIVOS:
// ADD FUNCIONALIDADE DE EMPRÉSTIMO
// OS ARRAYS DE LIVROS E EMPRÉSTIMOS SÃO ALOCADOS COM MALLOC/CALLOC.
// NOVA STRUCT PARA EMPRÉSTIMO
// LIBERAÇÃO DA MEMÓRIA COM FREE
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

struct emprestimo {
    int indiceLivro; // para saber qual livro do array 'biblioteca' foi emprestado.
    char nomeUsuario[TAM_STRING];
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
    // 1. ALOCAÇÃO DINÂMICA DE MEMÓRIA
    // Agora, em vez de array estáticos, usamos ponteiros.
    struct Livro *biblioteca;
    struct emprestimo *emprestimos;

    // Usamos calloc para o array de livros, calloc(num_elementos, tamanho_de_cada_elemento)
    // Vantagem: inicializa toda a memoria com zeros. Isso significa que 'disponível' já começa com
    biblioteca = (struct Livro *) calloc(MAX_LIVROS, sizeof(struct Livro));

    // Usamos malloc para o array de empréstimos, malloc(tamanho_total_em_bytes)
    emprestimos = (struct emprestimo *) malloc(MAX_EMPRESTIMOS * sizeof(struct emprestimo));

    // VERIFICAÇÃO: É crucial verificar se a locação de memória deu certo.
    if (biblioteca == NULL || emprestimos == NULL) {
        printf("Erro: Falha ao alocar memória. \n");
        return 1; // Retorna 1 para indicar um erro.
    }

    int totalLivros = 0;
    int totalEmprestimos = 0;
    int opcao;

    // --- Laço Principal do Menu ---
    do {

        // Exibe o menu de opções
        printf("#########################################\n");
        printf("     BIBLIOTECA - PARTE 2 \n");
        printf("#########################################\n");
        printf("1 - Cadastrar novo livro\n");
        printf("2 - Listar todos os livro\n");
        printf("3 - Realizar emprestimos\n");
        printf("4 - Listar emprestimos\n");
        printf("0 - Sair  livro\n");
        printf("-----------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao); // Lê a opção do usuário.
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
                    biblioteca[totalLivros].disponivel = 1; // Define o livro como disponível

                    printf("Digite a edicao: ");
                    scanf("%d", &biblioteca[totalLivros].edicao);
                    limparBufferEntrada();

                    totalLivros++;

                    printf("\n Livro cadastrado com sucesso!\n");
                } else {
                    printf("Biblioteca cheia! Não e possivel cadastrar mais livros. \n");
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
            case 3: // REALIZAR EMPRÉSTIMO
                printf("--- Realizar emprestimo ---\n\n");

                if (totalEmprestimos >= MAX_EMPRESTIMOS) {
                    printf("limite de emprestimos atingido!\n");
                } else {
                    printf("Livros disponivel: \n");
                    int disponiveis = 0;
                    for (int i = 0; i < totalLivros; i++) {
                        if (biblioteca[i].disponivel) {
                            printf("%d - %s\n", i + 1, biblioteca[i].nome);
                            disponiveis++;
                        }
                    }

                    if (disponiveis == 0) {
                        printf("Nenhum livro disponivel para emprestimo \n");
                    } else {
                        printf("\nDigite o nome do livro que deseja emprestar: \n");
                        int numLivro;
                        scanf("%d", &numLivro);
                        limparBufferEntrada();

                        int indice = numLivro - 1; // Converte para o indice do array (0 a N-1).

                        //Validação da escolha do usuário.
                        if (indice >= 0 && indice < totalLivros && biblioteca[indice].disponivel) {
                            printf("Digite o nome do usuario que esta pegando o livro: ");
                            fgets(emprestimos[totalEmprestimos].nomeUsuario, TAM_STRING, stdin);
                            emprestimos[totalEmprestimos].nomeUsuario[strcspn(emprestimos[totalEmprestimos].nomeUsuario, "\n")] = '\0';
                            
                            // Registra o emprestimo
                            emprestimos[totalEmprestimos].indiceLivro = indice;

                            // Atualiza o status do livro
                            biblioteca[indice].disponivel = 0;

                            totalEmprestimos++;
                            printf("\nEmprestimo realizado com sucesso!\n");
                        } else {
                            printf("\nNumero de livro invalido ou livro indisponivel.\n");
                        }
                    }
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            
            case 4: // LISTAR EMPRÉSTIMOS
                printf("--- Lista de emprestimos ---\n\n");
                if (totalEmprestimos == 0 ) {
                    printf("Nenhum emprestimo realizado");
                } else {
                    for (int i =0; i < totalEmprestimos; i++) {
                        //Usa o indice armazenado emprestimo para buscar o nome do livro.
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
                break;

            case 0: // SAIR
                printf("\n Saindo do Sistema...\n");
                break;
            
            default: // OPÇÃO INVÁLIDA
                printf("\n Opcao Invalida! Tente novamente. \n");
                printf("\n Pressione Enter para continuar...");
                getchar();
                break;
        }
    } while (opcao != 0); // Adicione esta linha para fechar o "do"

    // 2. LIBERAÇÃO DA MEMÓRIA
    // Antes DE TERMINAR, é essencial foi alocado dinamimicamente.
    // Isso evita "memoria leaks" (va)
    free (biblioteca);
    free (emprestimos);

    printf("Memoria liberada com sucesso \n");

    return 0;
}

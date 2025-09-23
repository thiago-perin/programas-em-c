// ######################################################
// SISTEMA DE BIBLIOTECA - mod 1
// ######################################################

#include <stdio.h>
#include <string.h> // Essencial para comparar strings
#include <stdlib.h> // Para a função exit(), caso a lista não esteja ordenada para a busca binária

// ######################################################
// - - - DECLARAÇÃO DAS FUNÇÕES (PROTÓTIPOS) - - - 
// Isso informa ao compilador que essas funções existem e como elas são,
// mesmo que suas implementações estejam abaixo do código.
// ######################################################

// ######################################################
// 1. BUSCA SEQUENCIAL
// ######################################################
int buscaSequencialIterativa(const char* lista[], int tamanho, const char* item);
int buscaSequencialRecursiva(const char* lista[], int tamanho, const char* item);
int buscaSequencialRecursiva_aux(const char* lista[], int tamanho, const char* item, int indice);

// ######################################################
// 1. BUSCA BINÁRIA
// ######################################################
int buscaBinariaIterativa(const char* lista[], int tamanho, const char* item);
int buscaBinariaRecursiva(const char* lista[], int tamanho, const char* item);
int buscaBinariaRecursiva_aux(const char* lista[], const char* item, int indice, int fim);

// Função auxiliar para imprimir os resultados de forma organizada
void imprimirResultado(const char* nomeDaBusca, const char* item, int resultado);

// - - - FUNÇÃO PRINCIPAL (main) - - -
// O programa começa a ser executado aqui.
int main () {
    // IMPORTANTE: Para a Busca binária funcionar, a lista DEVE estar ordenada alfabeticamente.
    // nossa lista de compras já está ordenada para o exemplo.
    const char* listaDeCompras[] = {
        "arroz",
        "batata",
        "cebola",
        "detergente",
        "feijao",
        "leite",
        "macarrao",
        "oleo",
        "sal",
        "tomate"
    };

    // Calcula o numero de itens na lista.
    // sizeof(listaDeCompras) -> tamanho total do array em bytes.
    // sizeof(listaDeCompras[0]) -> tamanho de um elemento (um ponteiro char*) em bytes.
    // A divisão nos dá o número de elementos.
    int tamanhoDaLista = sizeof(listaDeCompras) / sizeof(listaDeCompras[0]);

    // Itens que vamos procurar para demonstrar os algoritmos.
    const char* itemExistente = "leite";
    const char* itemInexistente = "queijo";

    printf(" - - - DEMONSTRACAO DE ALGORITMO DE BUSCA EM LISTA DE TEXTO - - - \n");
    printf("Lista Utilizada: [arroz, batata, cebola, detergente, feijao, leite, macarrao, oleo, sal, tomate]\n");
    printf(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - \n\n");

    // - - - Testando a Busca Sequencial - - -
    printf("- - - 1. BUSCA SEQUENCIAL - - - \n");
    // a) Versao Iterativa
    int res1 = buscaSequencialIterativa(listaDeCompras, tamanhoDaLista, itemExistente);
    imprimirResultado("Sequencial Iterativa", itemExistente, res1);

    int res2 = buscaSequencialIterativa(listaDeCompras, tamanhoDaLista, itemInexistente);
    imprimirResultado("Sequencial Iterativa", itemInexistente, res2);

    // b) Versão Recursiva
    int res3 = buscaSequencialRecursiva(listaDeCompras, tamanhoDaLista, itemExistente);
    imprimirResultado("Sequencial Recursiva", itemExistente, res3);

    int res4 = buscaSequencialRecursiva(listaDeCompras, tamanhoDaLista, itemInexistente);
    imprimirResultado("Sequencial Recursiva", itemInexistente, res4);
    printf(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - \n\n");

    // - - - Testando a Busca Binária - - - 
    printf(" - - - 2. BUSCA BINARIA (requer lista ordenada) - - - \n\n");
    // a) Versão Iterativa
    int res5 = buscaBinariaIterativa(listaDeCompras, tamanhoDaLista, itemExistente);
    imprimirResultado("Binaria Iterativa", itemExistente, res5);

    int res6 = buscaBinariaIterativa(listaDeCompras, tamanhoDaLista, itemInexistente);
    imprimirResultado("Binaria Iterativa", itemInexistente, res6);

    // b) Versão Recursiva
    int res7 = buscaBinariaRecursiva(listaDeCompras, tamanhoDaLista, itemExistente);
    imprimirResultado("Binaria Recursiva", itemExistente, res7);

    int res8 = buscaBinariaRecursiva(listaDeCompras, tamanhoDaLista, itemInexistente);
    imprimirResultado("Binaria Recursiva", itemInexistente, res8);
    printf(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - \n\n");

    return 0; // Indica que o programa terminou com sucesso.
}

// - - - IMPLEMENTAÇÃO DAS FUNÇÕES - - -

// ===================================================================================
// 1. BUSCA SEQUENCIAL
// ===================================================================================
// Percorre a lista elemento por elemento, do inicio ao fim,
// até encontrar o item ou chegar ao final da lista.
// Não requer que a lista esteja ordenada.
// ===================================================================================

/**
 * @brief Busca sequencial usando um laço (iteração).
 * @param lista O array de strings onde a busca será feita.
 * @param tamanho O número de elementos na lista.
 * @param item A string que está sendo procurada.
 * @return O indice do item se encontrado, ou -1 caso contrário.
 */
int buscaSequencialIterativa(const char* lista[], int tamanho, const char* item) {
    // Um laço 'for' percorre cada indice da lista, de 0 até 'tamanho - 1'.
    for (int i = 0; i < tamanho; i++) {
        // strcmp(str1, str2) compara duas strings.
        // Retorna 0 se elas forem idênticas.
        if (strcmp(lista[i], item) == 0) {
            // Se as strings forem iguais, encontramos o item!
            return i; // Retorna o indice atual.
        }
    }
    // Se o laço terminar e não tivermos retornado nada,
    // significa que o item não está na lista.
    return -1;
}

/**
 * @brief Função "invólucro" (wrapper) para a busca sequencial recursiva.
 * Ela esconde a complexidade do parâmetro 'indice' do usuário.
 */
int buscaSequencialRecursiva(const char* lista[], int tamanho, const char* item) {
    // A lógica recursiva real está na função auxiliar.
    // Começamos a busca a partir do índice 0.
    return buscaSequencialRecursiva_aux(lista, tamanho, item, 0);
}

/**
 * @brief Função auxiliar que implementa a lógica recursiva da busca sequencial.
 * @param indice O índice do elemento atual a ser verificado.
 * @return O índice do item se encontrado, ou -1 caso contrário.
 */
int buscaSequencialRecursiva_aux(const char* lista[], int tamanho, const char* item, int indice) {
    // Caso base 1: Falha (fim da lista)
    // Se o índice atual for igual ou maior que o tamanho da lista,
    // significa que já percorremos todos os elementos sem sucesso.
    if (indice >= tamanho) {
        return -1; // Item não encontrado.
    }

    // Caso base 2: sucesso
    // Compara o item na posição, 'indice' atual com o item procurado.
    if (strcmp(lista[indice], item) == 0) {
        return indice; // Item encontrado! Retorna o índice.
    }

    // Passo Recursivo
    // Se não for nenhum dos casos base, o item ainda pode estar no resto da lista.
    // Chamamos a função novamente, mas agora para o próximo índice (indice + 1).
    // O retorno desta chamada será propoagado para a chamada anterior.
    return buscaSequencialRecursiva_aux(lista, tamanho, item, indice + 1);
}

// ===================================================================================================
// BUSCA BINARIA
// ===================================================================================================
// Algoritmo de "dividir para conquistar"
// A cada passo, ele olha, para o elemento do meio da l (ou sub-lista).
// Se o item do meio for o procurado, a busca termina.
// Se o item procurado for menor, ele descarta a metade direita e busca na esquerda.
// Se for maior, descarta a metade esquerda e busca na direita.
// Requer que a lista esteja ORDENADA.
// ===================================================================================================

/**
* @brief Busca binária usando um laço (iteração).
* @return O índice do item se encontrado, ou -1 caso contrário.
*/
int buscaBinariaIterativa(const char* lista[], int tamanho, const char* item) {
    int inicio = 0;
    int fim = tamanho -1;
    int meio;

    // O laço continua enquanto a sub-lista for válida (inicio não ultrapassa o fim).
    while (inicio <= fim) {
        // Calcula o índice do meio.
        // Usar inicio + (fim - inicio) / 2 evita overflow para listas muito grandes.
        meio = inicio + (fim - inicio) / 2;

        // Compara a string do meio com o item procurado.
        int comparacao = strcmp(lista[meio], item);

        if (comparacao == 0) { // Encontrou!
            return meio;
        }
        else if (comparacao > 0) {
            // O item do meio vem DEPOIS do item procurado (ex: "leite" > "feijao").
            // Portanto, o item só pode estar na metade esquerda.
            // Descartamos a metade direita, ajustadno o 'fim'.
            fim = meio -1;
        }
        else { // comparacao < 0
            // o item do meio vem ANTES do item procurado (ex: "leite" < "sal").
            // Portanto, o item só pode estar na metade direita.
            // Descartamos a metade esquerda, ajustando o 'inicio'.
            inicio = meio + 1;
        }
    }

    // Se o laço 'while' terminar, significa que 'inicio' ultrapassou 'fim',
    // o espaço de busca se esgotou e o item não foi encontrado.
    return -1;
}

/**
* @brief Função 'invólucro' (wrapper) para a busca binária recursiva.
*/
int buscaBinariaRecursiva(const char* lista[], int tamanho, const char* item) {
    // Inicio a busca recursiva em toda a lista, do indice 0 até 'tamanho - 1'.
    return buscaBinariaRecursiva_aux(lista, item, 0, tamanho - 1);
}

/**
* @brief Função auxiliar que implementa a lógicarecursiva da binária binária.
* @param inicio O índice inicial da sub-lista atual.
* @param fim O indice final da sub-lista atual.
* @return O índice do item se encontrado, ou -1 caso contrário.
*/
int buscaBinariaRecursiva_aux(const char* lista[], const char* item, int inicio, int fim) {
    // CASO BASE 1: FALHA (Espaço de busca inválido)
    // Se o 'início' ultrapassar o 'fim', a sub-lista é vazia, o item não está aqui.
    if (inicio > fim) {
        return -1;
    }

    // PASSO RECURSIVO E CASO BASE DE SUCESSO
    int meio = inicio + (fim - inicio) / 2;
    int comparacao = strcmp(lista[meio], item);

    if (comparacao == 0) {
        // CASO BASE 2: SUCESSO
        return meio; // Encontramos o item!
    }
    else if (comparacao > 0) {
       // O item está na metade esquerda.
       return buscaBinariaRecursiva_aux(lista, item, inicio, meio - 1); // Linha corrigida
    } else {
       // O item está na metade direita.
       return buscaBinariaRecursiva_aux(lista, item, meio + 1, fim); // Bloco adicionado
    }
} // A chave de fechamento da função já existe no seu código

//=========================================================================================
// FUNÇÃO AUXILIAR DE IMPRESSÃO
//=========================================================================================

/**
 * @brief Formata e imprimi o resultado de uma busca.
 * @param nomeDaBusca O nome do algoritmo utilizado.
 * @param item O item que foi procurado.
 * @param resultado O resultado da busca (indice ou -1).
 */
void imprimirResultado(const char* nomeDaBusca, const char* item, int resultado) {
    printf("[%s] Buscando por \"%s\"... \n", nomeDaBusca, item);
    if (resultado != -1) {
        printf("     -> Resultado: Item encontrado no indice %d.\n", resultado);
    } else {
        printf("     -> Resultado: Item nao encontrado na lista. \n");
    }
}

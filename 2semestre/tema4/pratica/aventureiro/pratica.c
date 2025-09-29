/*
* Programa: implementação comparativa de BST e Trie em c
* --------------------------------------------------------------------------
Este programa demonstra como construir e utilizar duas estruturas clássicas:
    - BST (Árvore Binária de busca)
    - Trie (Árvore de prefixos para Strings)
    O código permite inserir a buscar strings, além de visualizar percursos.
    Ideal para compreender diferenças práticas e aplicações de cada estrutura.
PONTOS DE ATENÇÃO:
[#1] Definição das estruturas (BST e Trie)
[#2] Criação de nós e funções básicas para cada estrutura
[#4] Normalização: padroniza a palavra antes de inserir/buscar na Trie
[#5] Inserção e busca: as palavras são inseridas nas duas estruturas
[#6] Visualização: exige dados em ordem (BST) ou prefixos (Trie)
[#7] Liberação de memória de todas as estruturas
*/

#include <stdio.h>      // Biblioteca padrão de entrada/saída
#include <stdlib.h>     // Para malloc() e free()
#include <string.h>     // Para manipulação de strings
#include <stdbool.h>

// ========== BST: Árvore Binária de Busca ============

// Estrutura do nó da BST
// [#1] Definição das estruturas (BST e Trie)
typedef struct NoBST {
    char valor[50];         // Valor armazenado no nó (aqui, string)
    struct NoBST* esquerda;    // [Ponteiro] aponta para o nó-filho à esquerda
    struct NoBST* direita;     // [Ponteiro] aponta para o nó-filho à direita
} NoBST;

// Cria um novo nó da BST
// [#2] Criação de nós e funções básicas para cada estrutura
NoBST* criarNoBST(const char* valor) {
    // [Ponteiro] malloc retorna o endereço do nó criado, que será manipulado via ponteiro.
    NoBST* novo = (NoBST*) malloc(sizeof(NoBST));
    strcpy(novo->valor, valor);
    novo->esquerda = NULL;  // [Ponteiro] Um ponteiro nulo indica "não tem filho" (folha)
    novo->direita = NULL;   // [Ponteiro] Um ponteiro nulo indica "não tem filho" (folha)
    return novo;            // [Ponteiro] Retorna o endereço do nó criado.
}

NoBST* inserirBST(NoBST* raiz, const char* valor) {
    if (raiz == NULL)
        return criarNoBST(valor); // Se a árvore (ou sub-árvore) está vazia, cria um novo nó
    if (strcmp(valor, raiz->valor) < 0)
        // Insere na subárvore esquerda
        raiz->esquerda = inserirBST(raiz->esquerda, valor); // [Ponteiro] Liga o campo esquerda para apontar para o novo subnó
    else
        // Insere na subárvore direita
        raiz->direita = inserirBST(raiz->direita, valor); // [Ponteiro] Liga o campo esquerda para apontar para o novo subnó
    return raiz; // [Ponteiro] Sempre devolve o ponteiro (endereço) da raiz da árvore atualizada.
}

// Busca na BST
bool buscaBST(NoBST* raiz, const char* chave) {
    if (raiz == NULL)
        return false;
    if (strcmp(chave, raiz->valor) == 0)
        return true;
    if (strcmp(chave, raiz->valor) < 0)
        return buscaBST(raiz->esquerda, chave); // [Ponteiro] Vai seguindo o ponteiro para a esquerda
    else
        return buscaBST(raiz->direita, chave); // [Ponteiro] Vai seguindo o ponteiro para a direita
}

// Percurso em ordem (alfabética)
// Visita a subárvore esquerda, o nó atural e depois a direita. Exibe valores em ordem alfabética (ou numérica).
void emOrdemBST(NoBST* raiz) {
    if (raiz != NULL) {
        emOrdemBST(raiz->esquerda);            // [Ponteiro] Percorre a subárvore à esquerda
        printf("%s   |   ", raiz->valor);   // Visita o nó atual (raiz)
        emOrdemBST(raiz->direita);             // [Ponteiro] Percorre a subárvore à direita
    }
}

// Liberar memória da BST
void liberarBST(NoBST* raiz) {
    if (raiz != NULL) {
        liberarBST(raiz->esquerda);       // [Ponteiro] Libera toda a subárvore esquerda
        liberarBST(raiz->direita);        // [Ponteiro] Libera toda a subárvore direita
        free(raiz);                    // [Ponteiro] Libera o nó atual (raiz)        
    }
}

// =========== Trie: Árvore de Prefixos para Strings =============

#define TAMANHO_ALFABETO 26

// Estrutura do nó Trie
// [#1] Definição das estruturas (BST e Trie)
typedef struct NoTrie {
    struct NoTrie* filhos[TAMANHO_ALFABETO];
    bool ehFimDePalavra;
} NoTrie;

// Cria um novo nó Trie
// [#2] Criação de nós e funções básicas para cada estrutura
NoTrie* criarNoTrie() {
    // [Ponteiro] malloc retorna o endereço do nó criado, que será manipulado via ponteiro.
    NoTrie* novo = (NoTrie*) malloc(sizeof(NoTrie));
    novo->ehFimDePalavra = false;
    for (int i = 0; i < TAMANHO_ALFABETO; i++)
        novo->filhos[i] = NULL;
    return novo;
}

//Normaliza string (minúsculas, sem espaços/acentos)
// [#4] Normalização: padroniza a palavra antes de inserir/buscar na Trie
void normalizar(const char* entrada, char* saida) {
    int j = 0;
    for (int i = 0; entrada[i] != '\0'; i++) {
        char c = entrada[i];
        if (c >= 'A' && c <= 'Z') c += 32; // Converte maiúscula
        if (c >= 'a' && c <= 'z') saida[j++] = c; // Apenas letras
    }
    saida[j] = '\0';
}

// Insere palavra na Trie
void inserirTrie(NoTrie* raiz, const char* palavra) {
    NoTrie* atual = raiz;
    for (int i = 0; palavra[i] != '\0'; i++) {
        int indice = palavra[i] - 'a';
        if (atual->filhos[indice] == NULL)
            atual->filhos[indice] = criarNoTrie();
        atual = atual->filhos[indice];
    }
    atual->ehFimDePalavra = true;
}

// Busca palavra na Trie
bool buscaTrie(NoTrie* raiz, const char* palavra) {
    NoTrie* atual = raiz;
    for (int i = 0; palavra[i] != '\0'; i++) {
        int indice = palavra[i] - 'a';
        if (atual->filhos[indice] == NULL)
            return false;
        atual = atual->filhos[indice];
    }
    return atual != NULL && atual->ehFimDePalavra;
}

// Lista palavras na Trie (lexicográfica)
void listarPalavras(NoTrie* no, char* buffer, int nivel) {
    if (no->ehFimDePalavra) {
        buffer[nivel] = '\0';
        printf("%s   |   ", buffer);
    }
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (no->filhos[i] != NULL) {
            buffer[nivel] = 'a' + i;
            listarPalavras(no->filhos[i],buffer, nivel + 1);
        }
    }
}

// Liberar memória da Trie
void liberarTrie(NoTrie* no) {
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (no->filhos[i] != NULL) {
            liberarTrie(no->filhos[i]);
        }
    }
    free(no);
}

// ========= Função principal: main() - DEMONSTRAÇÃO =========

// [#5] Inserção e busca: as palavras são inseridas nas duas estruturas
int main() {
    // -------- BST --------
    printf("\n==== BST ====\n\n");
    NoBST* raizBST = NULL;
    raizBST = inserirBST(raizBST, "Pegadas de Lama");
    raizBST = inserirBST(raizBST, "Chave perdida");
    raizBST = inserirBST(raizBST, "Livro com Pagina faltando");
    raizBST = inserirBST(raizBST, "Lencol manchado");
    raizBST = inserirBST(raizBST, "Gaveta perdida");

    printf("Em ordem (BST) \n");
    emOrdemBST(raizBST);
    printf("\nBuscar 'Lencol Manchado' (BST): %s\n", buscaBST(raizBST, "Lencol Manchado") ? "Encontrado" : "Não encontrado" );
    printf("\nBuscar 'Oculos' (BST): %s\n", buscaBST(raizBST, "Oculos") ? "Encontrado" : "Nao encontrado" );
    
    // -------- Trie --------
    printf("\n==== Trie ==== \n\n");
    char normalizada[100];
    NoTrie* raizTrie = criarNoTrie();

    //Inserção de palavras normalizadas (minúsculo, sem espaço)
    normalizar("Pegadas de Lama", normalizada);
    inserirTrie(raizTrie, normalizada);
    normalizar("Chave Perdida", normalizada);
    inserirTrie(raizTrie, normalizada);
    normalizar("Livro com pagina faltando", normalizada);
    inserirTrie(raizTrie, normalizada);
    normalizar("Lencol manchado", normalizada);
    inserirTrie(raizTrie, normalizada);
    normalizar("Gaveta perdida", normalizada);
    inserirTrie(raizTrie, normalizada);

    printf("Palavras na Trie: ");
    char buffer[100];
    // [#6] Visualização: exige dados em ordem (BST) ou prefixos (Trie)
    listarPalavras(raizTrie, buffer, 0);
    printf("\n");
    normalizar("Lencol manchado", normalizada);
    printf("Buscar 'lencolmanchado' (Trie): %s\n", buscaTrie(raizTrie, normalizada) ? "Encontrado" : "Nao encontrado");
    normalizar("Oculos", normalizada);
    printf("Buscar 'oculos' (Trie): %s\n", buscaTrie(raizTrie, normalizada) ? "Encontrado" : "Nao encontrado");

    // Liberacao da memoria
    // [#7] Liberação de memória de todas as estruturas
    liberarBST(raizBST);
    liberarTrie(raizTrie);

    return 0;
}

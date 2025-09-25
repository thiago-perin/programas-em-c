// Inclui as bibliotecas necessárias para o programa.
#include <stdio.h>      // Biblioteca padrão de entrada/saída, usada para funções como printf().
#include <stdlib.h>     // Biblioteca padrão que inclui funções de gerenciamento de memória, como malloc() e free().
#include <string.h>     // Biblioteca para manipulação de strings, usada para strcpy() e strcmp().

/*
 * [#1] ESTRUTURA BÁSICA DO NÓ DA ÁRVORE BINÁRIA
 * --------------------------------------------------------------------------
 * Um "Nó" é o bloco de construção fundamental de uma árvore.
 * Cada nó contém:
 * - O dado que queremos armazenar (neste caso, uma string 'valor').
 * - Um ponteiro para o nó filho à esquerda.
 * - Um ponteiro para o nó filho à direita.
 * Se um filho não existe, seu ponteiro será NULL.
*/
typedef struct No {
    char valor[50];         // Um array de caracteres para armazenar o nome/valor do nó.
    struct No* esquerda;    // Ponteiro para outro 'No', que será o filho da esquerda.
    struct No* direita;     // Ponteiro para outro 'No', que será o filho da direita.
} No;


/*
 * [#2] FUNÇÃO RESPONSÁVEL POR CRIAR UM NOVO NÓ
 * --------------------------------------------------------------------------
 * Esta função é como uma "fábrica" de nós. Ela automatiza o processo de:
 * 1. Alocar (reservar) um espaço na memória para um novo nó.
 * 2. Copiar o valor desejado para dentro desse nó.
 * 3. Iniciar seus filhos como NULL, pois um novo nó nunca tem filhos.
 * 4. Retornar o endereço de memória do nó recém-criado.
*/
No* criarNo(const char* valor) {
    // 1. Pede ao sistema operacional um bloco de memória do tamanho exato de um 'No'.
    No* novo = (No*) malloc(sizeof(No));

    // É uma boa prática verificar se a memória foi alocada com sucesso.
    if (novo == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);     // Encerra o programa se não houver memória disponível.
    }

    // 2. Copia a string recebida como parâmetro para o campo 'valor' do novo nó.
    // Em C, não podemos atribuir strings com '=', precisamos usar strcpy().
    strcpy(novo->valor, valor);

    // 3. Um nó recém-criado é sempre uma "folha", ou seja, não aponta para outros nós.
    novo->esquerda = NULL;
    novo->direita = NULL;

    // 4. Retorna o ponteiro para o novo nó, para que ele possa ser conectado à árvore.
    return novo;
}


/*
 * [#3] FUNÇÃO DE INSERÇÃO
 * --------------------------------------------------------------------------
 * Esta função encontra o local correto na árvore para inserir um novo valor.
 * Ela funciona de forma recursiva (chama a si mesma) e segue uma regra simples:
 * - Se a árvore (ou o galho atual) estiver vazia, cria o novo nó aqui.
 * - Compara o novo valor com o valor do nó atual:
 * - Se for "menor" (alfabeticamente), tenta inserir na subárvore da esquerda.
 * - Se for "maior ou igual", tenta inserir na subárvore da direita.
*/
No* inserir(No* raiz, const char* valor) {
    // CASO BASE DA RECURSÃO: Encontramos um local vazio (um ponteiro NULL).
    // É aqui que o novo nó deve ser criado e inserido.
    if (raiz == NULL) {
        return criarNo(valor); // Cria o nó e retorna seu endereço para o pai.
    }

    // PASSO RECURSIVO: Compara o valor a ser inserido com o valor do nó atual.
    // A função strcmp() retorna:
    //   < 0 se o primeiro valor for menor que o segundo.
    //   > 0 se o primeiro valor for maior.
    //   = 0 se forem iguais.
    if (strcmp(valor, raiz->valor) < 0) {
        // Se o novo valor é menor, ele pertence à esquerda.
        // A mágica acontece aqui: o resultado da chamada recursiva (o endereço do novo nó)
        // é atribuído ao ponteiro 'esquerda' do nó atual, conectando-o à árvore.
        raiz->esquerda = inserir(raiz->esquerda, valor);
    } else {
        // Se for maior ou igual, pertence à direita.
        // O mesmo processo de conexão acontece aqui.
        raiz->direita = inserir(raiz->direita, valor);
    }

    // Retorna o ponteiro da raiz atual sem modificação para manter a estrutura da árvore intacta
    // nos níveis superiores da recursão.
    return raiz;
}

/*
 * [#4A] PERCURSO PRÉ-ORDEM (RAIZ, ESQUERDA, DIREITA)
 * --------------------------------------------------------------------------
 * Uma das formas de "ler" ou "visitar" todos os nós da árvore.
 * A ordem é:
 * 1. Processa o nó ATUAL (Raiz).
 * 2. Visita recursivamente TODOS os nós da subárvore ESQUERDA.
 * 3. Visita recursivamente TODOS os nós da subárvore DIREITA.
*/
void preOrdem(No* raiz) {
    // A condição 'if (raiz != NULL)' é o que impede a recursão infinita.
    // Quando chegamos a um ponteiro NULL (um galho que não leva a lugar nenhum), a função simplesmente retorna.
    if (raiz != NULL) {
        printf("%s   |   ", raiz->valor);      // 1. Processa o nó atual (a "raiz" deste galho).
        preOrdem(raiz->esquerda);              // 2. Explora todo o lado esquerdo.
        preOrdem(raiz->direita);               // 3. Explora todo o lado direito.
    }
}

/*
 * [#4B] PERCURSO EM ORDEM (ESQUERDA, RAIZ, DIREITA)
 * --------------------------------------------------------------------------
 * Outra forma de visitar os nós. Para uma árvore binária de busca, este
 * percurso exibe os valores em ordem crescente (alfabética, neste caso).
 * A ordem é:
 * 1. Visita recursivamente TODOS os nós da subárvore ESQUERDA.
 * 2. Processa o nó ATUAL (Raiz).
 * 3. Visita recursivamente TODOS os nós da subárvore DIREITA.
*/
void emOrdem(No* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);               // 1. Explora todo o lado esquerdo primeiro.
        printf("%s   |   ", raiz->valor);      // 2. Processa o nó atual.
        emOrdem(raiz->direita);                // 3. Explora todo o lado direito por último.
    }
}


/*
 * [#4C] PERCURSO PÓS-ORDEM (ESQUERDA, DIREITA, RAIZ)
 * --------------------------------------------------------------------------
 * A terceira forma principal de visitar os nós. É muito útil para
 * operações como deletar a árvore, pois garante que os filhos sejam
 * processados antes do pai.
 * A ordem é:
 * 1. Visita recursivamente TODOS os nós da subárvore ESQUERDA.
 * 2. Visita recursivamente TODOS os nós da subárvore DIREITA.
 * 3. Processa o nó ATUAL (Raiz).
*/
void posOrdem(No* raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esquerda);              // 1. Explora todo o lado esquerdo.
        posOrdem(raiz->direita);               // 2. Explora todo o lado direito.
        printf("%s   |   ", raiz->valor);      // 3. Processa o nó atual por último.
    }
}


/*
 * [#5] LIBERAÇÃO DE TODA MEMÓRIA USADA PELA ÁRVORE
 * --------------------------------------------------------------------------
 * Esta função usa o percurso em Pós-Ordem para liberar a memória de
 * forma segura. É crucial liberar os filhos ANTES do pai, pois se
 * liberarmos o pai primeiro, perderemos os ponteiros para seus filhos,
 * resultando em "vazamento de memória" (memory leak).
*/
void liberar(No* raiz) {
    if (raiz != NULL) {
        liberar(raiz->esquerda);       // Libera toda a subárvore esquerda primeiro.
        liberar(raiz->direita);        // Depois, libera toda a subárvore direita.
        free(raiz);                    // Por último, libera o nó atual.
    }
}


/*
 * [#6] EXECUÇÃO DO PROGRAMA (main)
 * --------------------------------------------------------------------------
 * Aqui é onde o programa realmente começa.
 * 1. Criamos um ponteiro para a raiz da árvore e o iniciamos como NULL.
 * 2. Inserimos alguns valores para construir a árvore.
 * 3. Demonstramos os três tipos de percurso.
 * 4. Liberamos toda a memória alocada para evitar vazamentos.
*/
int main() {
    // 1. Inicia a árvore. Uma árvore vazia é representada por um ponteiro NULL.
    No* raiz = NULL;

    // 2. Insere elementos na árvore. A cada inserção, a função 'inserir'
    // navega pela árvore e encontra o local correto para o novo nó.
    // O ponteiro 'raiz' é atualizado para sempre apontar para o início da árvore.
    raiz = inserir(raiz, "Hall de Entrada");
    raiz = inserir(raiz, "Sala de Estar");
    raiz = inserir(raiz, "Biblioteca");
    raiz = inserir(raiz, "Quarto");

    // 3. Exibe os elementos da árvore usando cada tipo de percurso.
    printf("Pré-ordem:  ");
    preOrdem(raiz); // Ordem de inserção: Hall, Sala, Biblioteca, Quarto
    printf("\n");

    printf("Em ordem: ");
    emOrdem(raiz); // Ordem alfabética: Biblioteca, Hall, Quarto, Sala
    printf("\n");

    printf("Pós-ordem: ");
    posOrdem(raiz); // Ordem de "folhas primeiro": Biblioteca, Quarto, Sala, Hall
    printf("\n");

    // 4. Libera toda a memória que foi alocada dinamicamente com malloc().
    // Isso é muito importante em programas maiores para garantir que o
    // programa não consuma memória desnecessariamente.
    liberar(raiz);

    // Retorna 0 para indicar que o programa foi executado com sucesso.
    return 0;
}

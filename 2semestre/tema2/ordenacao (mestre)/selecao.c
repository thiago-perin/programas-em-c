// ######################################################
// buble sort - insert sort - selection sort
// ######################################################
#include <stdio.h>
#include <stdlib.h> // Para as funções rand(), srand()
#include <time.h> // para a função time(), usada para semear o gerador de números aleatórios

// ######################################################
// Define o tamanho das listas que serão ordenadas.
// Facilita a alteração para testes com mais ou menos elementos.
// ######################################################
#define TAMANHO 10

// ######################################################
// - - - DECLARAÇÃO DAS FUNÇÕES (PROTÓTIPOS) - - -
// Informa ao compilador sobre a existencia e a "assinatura" das funções
// que serão implementadas mais abaixo.
// ######################################################

// Funções para gerar os diferentes tipos de listas
void gerarListaOrdenada(int lista[], int n);
void gerarListaInversa(int lista[], int n);
void gerarListaAleatoria(int lista[], int n);

// Funções de ordenação
void bubbleSort(int lista[], int n);
void insertionSort(int lista[], int n);
void selectionSort(int lista[], int n);

// Funções auxiliares
void imprimirLista( const char* titulo, const int lista[], int n);
void copiarLista(int destino[], const int origem[], int n);

// - - - FUNÇÃO PRINCIPAL (main) - - -
// O programa inicia sua execução aqui.
int main() {
    // Semeia o gerador de números aleatórios com a hora atual.
    // Isso garante que a lista aleatória seja diferente a cada execução do programa.
    // Deve ser chamado apenas uma vez.
    srand(time(NULL));

    // Declara os tres tipos de listas que servirão como nossos casos de teste.
    int listaOrdenada[TAMANHO];
    int listaInversa[TAMANHO];
    int listaAleatoria[TAMANHO];

    // Preenche as listas com os padroes desejados.
    printf("--- GERANDO LISTAS DE ENTRADA ---\n");
    gerarListaOrdenada(listaOrdenada, TAMANHO);
    gerarListaInversa(listaInversa, TAMANHO);
    gerarListaAleatoria(listaAleatoria, TAMANHO);
    printf("----------------------------------\n\n");

    // Declara uma lista temporária que será usada para receber uma cópia
    // dos dados antes de cada ordenação. Isso é crucial para garantir que
    // cada algoritmo receba a mesma lista de entrada original.
    int listaTemp[TAMANHO];

    // ==========================================================================
    // 1. TESTANDO O BUBBLE SORT
    // ==========================================================================
    printf(" ========== 1. BUBBLE SORT ========== \n");

    // a) Melhor Caso: Lista já ordenada
    copiarLista(listaTemp, listaOrdenada, TAMANHO);
    bubbleSort(listaTemp, TAMANHO);
    imprimirLista("Bubble Sort | Melhor Caso (ja ordenada)", listaTemp, TAMANHO);

    // b) Melhor Caso: Lista em ordem inversa
    copiarLista(listaTemp, listaInversa, TAMANHO);
    bubbleSort(listaTemp, TAMANHO);
    imprimirLista("Bubble Sort | Pior Caso (ordem inversa)", listaTemp, TAMANHO);

    // c) Caso Médio: Lista aleatória
    copiarLista(listaTemp, listaAleatoria, TAMANHO);
    bubbleSort(listaTemp, TAMANHO);
    imprimirLista("Bubble Sort | Caso Medio (aleatorio)", listaTemp, TAMANHO);
    printf("\n");

    // ============================================================================
    // 2. TESTANDO O INSERTION SORT
    // ============================================================================
    printf(" ========== 2. INSERTION SORT ========== \n");

    // a) Melhor Caso: Lista já ordenada
    copiarLista(listaTemp, listaOrdenada, TAMANHO);
    insertionSort(listaTemp, TAMANHO);
    imprimirLista("Insertion Sort | Melhor Caso (ja ordenada)", listaTemp, TAMANHO);

    // b) Pior Caso: Lista em ordem inversa
    copiarLista(listaTemp, listaInversa, TAMANHO);
    insertionSort(listaTemp, TAMANHO);
    imprimirLista("Insertion Sort | Pior Caso (ordem inversa)", listaTemp, TAMANHO);

    // c) Caso Médio: Lista aleatória
    copiarLista(listaTemp, listaAleatoria, TAMANHO);
    insertionSort(listaTemp, TAMANHO);
    imprimirLista("Insertion Sort | Caso Medio (aleatorio)", listaTemp, TAMANHO);
    printf("\n");

    // ============================================================================
    // 3. TESTANDO O SELECTION SORT
    // ============================================================================
    printf(" ========== 3. SELECTION SORT ========== \n");

        // a) Melhor Caso: Lista já ordenada
    copiarLista(listaTemp, listaOrdenada, TAMANHO);
    selectionSort(listaTemp, TAMANHO);
    imprimirLista("Selection Sort | Melhor Caso (ja ordenada)", listaTemp, TAMANHO);

    // b) Pior Caso: Lista em ordem inversa
    copiarLista(listaTemp, listaInversa, TAMANHO);
    selectionSort(listaTemp, TAMANHO);
    imprimirLista("Selection Sort | Pior Caso (ordem inversa)", listaTemp, TAMANHO);

    // c) Caso Médio: Lista aleatória
    copiarLista(listaTemp, listaAleatoria, TAMANHO);
    selectionSort(listaTemp, TAMANHO);
    imprimirLista("Selection Sort | Caso Medio (aleatorio)", listaTemp, TAMANHO);
    printf("\n");

    return 0; // FIM DO PROGRAMA
}


// ===================================================================================
// - - - IMPLEMENTAÇÃO DAS FUNÇÕES DE ORDENAÇÃO - - -
// ===================================================================================

/**
 * @brief Ordena uma lista usando o algoritmo Bubble Sort.
 * @details Lógica: Compara pares de elementos adjacentes e os troca se estiverem
 *          na ordem errada. Repete o processo até que a linha esteja ordenada.
 *          Elementos maiores "borbulham" para o final da lista.
 *          - Melhor caso: Com a otimização, faz apenas uma passagem.
 *          - Pior Caso: Lista em ordem inversa.
 */
void bubbleSort(int lista[], int n) {
    int i, j, temp;
    int trocou; // Flag de otimização

    // Laço externo controla o número de passagens pela lista.
    // Após cada passagem'i', o i-ésimo maior elemento estará no lugar certo.
    for (i = 0; i < n - 1; i++) {
        trocou = 0; // No inicio de cada passagem, assumimos que não haverá trocas.

        // Laço interno percorre a lista, comparando elementos adjacentes.
        // O limite é 'n - i - 1' porque os elementos no final já estão ordenados.
        for (j = 0; j < n - i - 1; j++) {
            // Se o elemento atual for maior que o próximo...
            if (lista[j] > lista[j + 1]) {
                // ... troca eles de lugar.
                temp = lista[j];
                lista[j] = lista [j+1];
                lista[j+1] = temp;
                trocou = 1; // Marca que uma troca ocorreu.
            }
        }

        // OTIMIZAÇÃO: Se após uma passagem completa nenhuma troca foi feita,
        // a lista já está ordenada e podemos parar o algoritmo.
        if (trocou == 0) {
            break;
        }
    }
}

/**
 * @brief Ordena uma lista usando o algoritmo Insertion Sort.
 * @details Lógica: Percorre a lista, construindo uma sub-lista ordenada à esquerda.
 *          A cada passo, pega o próximo elemento e o insere na posição correta
 *          dentro da sub-lista já ordenada, "empurrando" os maiores para a direita.
 *          - Melhor caso: Lista já ordenada, só percorre.
 *          - Pior Caso: Lista em ordem inversa.
 */
void insertionSort(int lista[], int n) {
    int i, chave, j;
    
    // Começa do segundo elemento (indice 1), pois o primeiro já é uma sub-lista ordenada.
    for (i = 1; i < n; i++) {
        // 'chave' é o elemento que queremos inserir na parte ordenada.
        chave = lista[i];
        // 'j' aponta para o último elemento da parte ordenada.
        j = i - 1;
        
        // Move os elementos da parte ordenada que são maiores que a 'chave'
        // uma posição para a direita, para abrir espaço para a inserção.
        while (j >= 0 && lista[j] > chave) {
            lista[j + 1] = lista[j];
            j = j - 1;
        }

        // Insere na posição correta (o "buraco" aberto).
        lista[j + 1] = chave;
    }
}

/**
 * @brief Ordena uma lista usando o algoritmo Selection Sort.
 * @details Lógica: Divide a lista em duas partes: ordenada (à esquerda) e
 *          não ordenada (à direita). A cada passo, encontra o menor elemento
 *          na parte não ordenada e o troca com o primeiro elemento da parte não ordenada.
 * 
 *          O número de comparações é o mesmo independentemente da ordem inicial.
 */
void selectionSort(int lista[], int n) {
    int i, j, indice_min, temp;
    
    // Laço externo percorre a lista para definir  a fronteira  entre
    // a parte ordenada e não ordenada.
    for (i = 0; i < n - 1; i++) {
        // Assume que o primeiro elemento da parte não ordenada é o menor.
        indice_min = i;
        // 'j' aponta para o último elemento da parte ordenada.
        
        // Laço interno percorre a parte não ordenada para encontrar o menor elemento.
        for (j = i + 1; j < n; j++) {
            // Se encontrar um elemento menor que o mínimo atual...
            if (lista[j] < lista[indice_min]) {
                // ...atualiza o indice do minimo.
                indice_min = j;
            }
        }

        // Se o menor elemento não for o primeiro da parte não ordenada,
        // troca-os de lugar.
        if (indice_min != i) {
            temp = lista[i];
            lista[i] = lista[indice_min];
            lista[indice_min] = temp;
        }
    }
}

//=========================================================================================
// - - - IMPLEMENTAÇÃO DAS FUNÇÕES AUXILIARES - - - 
//=========================================================================================
/**
 * @brief Gera uma lista de 'n' elementos em ordem crescente.
 */

void gerarListaOrdenada(int lista[], int n) {
    for (int i = 0; i < n; i++) {
        lista[i] = i + 1;
    }
    imprimirLista("Original - Ordenada", lista, n);
}

/**
 * @brief Gera uma lista de 'n' elementos em ordem decrescente.
 */

void gerarListaInversa(int lista[], int n) {
    for (int i = 0; i < n; i++) {
        lista[i] = n - i;
    }
    imprimirLista("Original - Inversa", lista, n);
}

/**
 * @brief Gera uma lista de 'n' elementos com valores aleatórios entre 1 e 100.
 */

/**
 * @brief Gera uma lista de 'n' elementos com valores aleatórios entre 1 e 100.
 */
void gerarListaAleatoria(int lista[], int n) {
    for (int i = 0; i < n; i++) {
        // Gera um número aleatório entre 1 e 100
        lista[i] = (rand() % 100) + 1;
    }
    imprimirLista("Original - Aleatoria", lista, n);
}

/**
 * @brief Copia os elementos de uma lista de origem para uma de destino.
 */
void copiarLista(int destino[], const int origem[], int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origem[i];
    }
}

/**
 * @brief Imprime uma lista de inteiros com um título.
 */
void imprimirLista(const char* titulo, const int lista[], int n) {
    printf("%s: [ ", titulo);
    for (int i = 0; i < n; i++) {
        printf("%d", lista[i]);
        // Adiciona vírgula exceto para o último elemento
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf(" ]\n");
}

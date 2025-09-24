// Inclui a biblioteca de entrada e saída padrão (Standard Input/Output),
// que permite usar funções como printf e scanf.
#include <stdio.h>

// Inclui a biblioteca padrão (Standard Library), que contém funções
// de propósito geral, como alocação de memória, controle de processos, etc.
#include <stdlib.h>

// Inclui a biblioteca de manipulação de strings (String Library),
// que fornece funções para trabalhar com texto, como copiar e comparar strings.
#include <string.h>

// Define uma constante chamada MAX com o valor 50.
// Isso será usado para definir o tamanho máximo da nossa pilha e fila.
#define MAX 50

// Define um novo tipo de dado chamado 'Pessoa' usando uma estrutura (struct).
// Uma struct agrupa diferentes tipos de dados em uma única variável.
typedef struct {
    // Um campo para armazenar o nome, com capacidade para até 29 caracteres + o caractere nulo ('\0').
    char nome[30];
    // Um campo para armazenar a idade.
    int idade;
} Pessoa;

// Define um novo tipo de dado chamado 'Fila' (Queue em inglês) usando uma struct.
typedef struct {
    // Um array (vetor) de 'Pessoa' que armazenará os itens da fila. O tamanho é definido pela constante MAX.
    Pessoa dados[MAX];
    // Uma variável para controlar a posição do início da fila (de onde os elementos são removidos).
    int inicio;
    // Uma variável para controlar a posição do fim da fila (onde os novos elementos são inseridos).
    int fim;
} Fila;

// Define um novo tipo de dado chamado 'Pilha' (Stack em inglês).
typedef struct {
    // Um array (vetor) de 'Pessoa' que armazenará os itens da pilha. O tamanho é definido pela constante MAX.
    Pessoa dados[MAX];
    // Uma variável para controlar a posição do último elemento inserido (o topo da pilha).
    int topo;
} Pilha;

// Função para inicializar a fila.
// Recebe um ponteiro para uma Fila como argumento para poder modificar a fila original.
void inicializarFila(Fila *f) {
    // Define o início da fila como 0.
    f->inicio = 0;
    // Define o fim da fila como 0.
    // Em uma fila vazia, início e fim apontam para a mesma posição.
    f->fim = 0;
}

// Função para inicializar a pilha.
// Recebe um ponteiro para uma Pilha como argumento para poder modificar a pilha original.
void inicializarPilha(Pilha *p) {
    // Define o topo como -1, que é a convenção para indicar que a pilha está vazia,
    // já que o primeiro elemento será inserido na posição de índice 0.
    p->topo = -1;
}

// Função para inserir uma pessoa no fim da fila (operação de enfileirar).
// Recebe um ponteiro para a fila e a pessoa a ser inserida.
void inserir(Fila *f, Pessoa p) {
    // Verifica se a fila não está cheia (se o índice 'fim' é menor que o tamanho máximo).
    if (f->fim < MAX) {
        // Adiciona a pessoa na posição 'fim' do array de dados.
        // O '++' depois de 'f->fim' faz com que o valor de 'fim' seja incrementado
        // logo após a atribuição, preparando para a próxima inserção.
        f->dados[f->fim++] = p;
    }
}

// Função para remover uma pessoa do início da fila (operação de desenfileirar).
// Recebe um ponteiro para a fila e retorna a pessoa que foi removida.
Pessoa remover(Fila *f) {
    // Retorna a pessoa que está na posição 'inicio' do array de dados.
    // O '++' depois de 'f->inicio' faz com que o valor de 'inicio' seja incrementado
    // após o retorno, efetivamente "removendo" o elemento ao mover o início da fila para frente.
    return f->dados[f->inicio++];
}

// Função para adicionar um novo elemento (uma 'Pessoa') no topo da pilha.
// O ato de adicionar é chamado de "push".
// Recebe um ponteiro para a pilha e a pessoa a ser inserida.
void push(Pilha *p, Pessoa pes) {
    // Verifica se a pilha ainda tem espaço. O último índice válido é MAX - 1.
    // Se o topo for menor que MAX - 1, significa que ainda há espaço.
    if (p->topo < MAX - 1) {
        // Primeiro, incrementa o topo ('++p->topo') e, em seguida,
        // insere o novo elemento 'pes' na nova posição do topo.
        p->dados[++p->topo] = pes;
    }
    // Nota: Se a pilha estiver cheia, esta função simplesmente não faz nada.
    // Em um sistema real, seria bom informar o usuário sobre o "estouro da pilha" (stack overflow).
}

// Função para remover e retornar o elemento do topo da pilha.
// O ato de remover é chamado de "pop".
// Recebe um ponteiro para a pilha e retorna a 'Pessoa' que estava no topo.
Pessoa pop(Pilha *p) {
    // Retorna o elemento que está na posição atual do topo.
    // O '--' (pós-decremento) faz com que o valor de 'topo' seja diminuído em 1
    // *depois* que o elemento na posição 'p->topo' for retornado.
    // Isso efetivamente remove o elemento do topo da pilha.
    return p->dados[p->topo--];
    // Nota: Esta implementação não verifica se a pilha está vazia antes de remover.
    // Tentar "pop" de uma pilha vazia ('topo' seria -1) levaria a um comportamento indefinido.
}

// Função para exibir todos os elementos de uma fila.
// Recebe uma cópia da Fila (não um ponteiro), pois não precisa modificá-la.
void exibirFila(Fila f) {
    // Cria um laço 'for' que começa no 'inicio' da fila e vai até o 'fim'.
    // A variável 'i' percorrerá os índices dos elementos válidos na fila.
    for (int i = f.inicio; i < f.fim; i++) {
        // Para cada elemento na fila, imprime o nome e a idade formatados.
        printf("Nome: %s | Idade: %d\n", f.dados[i].nome, f.dados[i].idade);
    }
}

// Função principal, onde a execução do programa começa.
int main() {
    // Declara as estruturas de dados que serão usadas no programa.
    Fila filaOriginal, filaInvertida; // Duas filas, uma para os dados originais e outra para o resultado.
    Pilha auxiliar; // Uma pilha que será usada como estrutura auxiliar no processo de inversão.

    // Inicializa as filas e a pilha para garantir que seus valores de controle (inicio, fim, topo)
    // estejam corretos antes do uso.
    inicializarFila(&filaOriginal);
    inicializarFila(&filaInvertida);
    inicializarPilha(&auxiliar);

    // Declara e inicializa um array de 'Pessoa' com dados pré-definidos.
    Pessoa pessoas[] = {
        {"Carlos", 45}, {"Joana", 62}, {"Lucas", 30}, {"Maria", 75}
    };
    // NOTA: Na versão original, faltava um ponto e vírgula (;) aqui. Eu o adicionei mentalmente para comentar a lógica.
    // O código original não compilaria sem ele.

    // Laço para popular a fila original.
    // O laço percorre o array 'pessoas' e insere cada uma na 'filaOriginal'.
    for (int i = 0; i < 4; i++ ) {
        inserir(&filaOriginal, pessoas[i]);
    }

    // Imprime um título na tela para indicar o que será exibido a seguir.
    printf("Fila original: \n");
    // Chama a função 'exibirFila' para mostrar o conteúdo da 'filaOriginal'.
    exibirFila(filaOriginal);

    // --- LÓGICA DE INVERSÃO DA FILA ---

    // Passo 1: Transferir todos os elementos da fila original para a pilha auxiliar.
    // A condição (f.inicio < f.fim) verifica se a fila ainda possui elementos.
    while(filaOriginal.inicio < filaOriginal.fim) {
        // Remove o elemento do início da fila e o empilha na pilha auxiliar.
        // O primeiro a sair da fila ("Carlos") se tornará a base da pilha.
        // O último ("Maria") ficará no topo da pilha.
        push(&auxiliar, remover(&filaOriginal));
    }

    // Passo 2: Transferir todos os elementos da pilha de volta para uma nova fila (filaInvertida).
    // A condição (p.topo >= 0) verifica se a pilha ainda possui elementos.
    while(auxiliar.topo >= 0) {
        // Remove o elemento do topo da pilha (LIFO - Last In, First Out) e o insere na nova fila.
        // O primeiro a sair da pilha será "Maria", que se tornará o primeiro da nova fila.
        // O último a sair ("Carlos") se tornará o último da nova fila.
        inserir(&filaInvertida, pop(&auxiliar));
    }

    // Adiciona uma linha em branco para separar as saídas.
    printf("\n");
    // Imprime o título para a fila invertida.
    printf("Fila invertida: \n");
    // Exibe o conteúdo da 'filaInvertida', que agora contém os elementos em ordem reversa.
    exibirFila(filaInvertida);

    // Retorna 0 para o sistema operacional, indicando que o programa foi executado com sucesso.
    return 0;
}

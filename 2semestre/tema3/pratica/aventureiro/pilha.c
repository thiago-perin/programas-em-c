// Inclui a biblioteca de entrada e saída padrão (Standard Input/Output),
// que permite usar funções como printf e scanf.
#include <stdio.h>

// Inclui a biblioteca padrão (Standard Library), que contém funções
// de propósito geral, como alocação de memória, controle de processos, etc.
#include <stdlib.h>

// Define uma constante chamada MAX com o valor 5.
// Isso será usado para definir o tamanho máximo da nossa pilha.
#define MAX 5

// Define um novo tipo de dado chamado 'Pessoa' usando uma estrutura (struct).
typedef struct {
    // Um campo para armazenar o nome, com capacidade para até 29 caracteres + o caractere nulo ('\0').
    char nome[30];
    // Um campo para armazenar a idade.
    int idade;
} Pessoa;

// Define um novo tipo de dado chamado 'Pilha' (Stack em inglês).
typedef struct {
    // Um array (vetor) de 'Pessoa' que armazenará os itens da pilha. O tamanho é definido pela constante MAX.
    Pessoa itens[MAX];
    // Uma variável para controlar a posição do último elemento inserido (o topo da pilha).
    int topo;
} Pilha;

// Função para inicializar a pilha.
// Recebe um ponteiro para uma Pilha como argumento.
void inicializarPilha(Pilha *p) {
    // Define o topo como -1, que é a convenção para indicar que a pilha está vazia.
    p->topo = -1;
}

// Função que verifica se a pilha está vazia.
// Retorna um valor verdadeiro (1) se estiver vazia, e falso (0) caso contrário.
int pilhaVazia(Pilha *p) {
    // Compara se o topo é igual a -1. Se for, a pilha está vazia.
    return p->topo == -1;
}

// Função que verifica se a pilha está cheia.
// Retorna verdadeiro (1) se estiver cheia, e falso (0) caso contrário.
int pilhaCheia(Pilha *p) {
    // Compara se o topo atingiu a última posição possível do array (MAX - 1, pois os índices começam em 0).
    return p->topo == MAX - 1;
}

// Procedimento (função sem retorno) para adicionar um novo elemento (uma 'Pessoa') no topo da pilha.
// O ato de adicionar é chamado de "push".
void push(Pilha *p, Pessoa nova) {
    // Verifica se a pilha já está cheia antes de tentar inserir.
    if (pilhaCheia(p)) {
        // Se estiver cheia, exibe uma mensagem de erro.
        printf("Pilha cheia. Nao e possivel inserir. \n");
        // E encerra a execução da função.
        return;
    }

    // Incrementa o topo, movendo-o para a próxima posição livre.
    p->topo++;
    // Atribui a nova 'Pessoa' à posição do topo no array de itens.
    p->itens[p->topo] = nova;
}

// Procedimento para remover o elemento do topo da pilha.
// O ato de remover é chamado de "pop".
// Recebe um ponteiro para a pilha e um ponteiro para uma variável 'Pessoa', onde o item removido será guardado.
void pop(Pilha *p, Pessoa *removida) {
    // Verifica se a pilha está vazia. Não é possível remover de uma pilha vazia.
    if (pilhaVazia(p)) {
        // Se estiver vazia, exibe uma mensagem de erro.
        printf("Pilha vazia. Nao e possivel remover. \n");
        // E encerra a função.
        return;
    }

    // Copia o valor do elemento que está no topo da pilha para a variável 'removida'.
    // O '*' é usado para acessar o conteúdo para o qual o ponteiro 'removida' aponta.
    *removida = p->itens[p->topo];
    // Decrementa o topo. Isso efetivamente "remove" o elemento, pois ele será sobrescrito na próxima inserção (push).
    p->topo--;
}

// Procedimento para "espiar" ou "visualizar" o elemento do topo sem removê-lo.
// Recebe um ponteiro para a pilha e um ponteiro para uma variável 'Pessoa' onde o item do topo será copiado.
void peek(Pilha *p, Pessoa *visualizada) {
    // Verifica se a pilha está vazia.
    if (pilhaVazia(p)) {
        // Se estiver vazia, exibe uma mensagem de erro.
        printf("Pilha vazia. Nada para espiar. \n");
        // E encerra a função.
        return;
    }

    // Copia o valor do elemento que está no topo da pilha para a variável 'visualizada'.
    // Diferente do 'pop', o valor de 'topo' não é alterado.
    *visualizada = p->itens[p->topo];
}

// Procedimento para mostrar todos os elementos da pilha.
void mostrarPilha(Pilha *p) {
    // Imprime um cabeçalho para indicar o que será mostrado. A ordem é do topo para a base.
    printf("Pilha (topo -> base):\n");
    // Cria um laço 'for' que começa do topo da pilha (p->topo) e vai até o primeiro elemento (índice 0).
    for (int i = p->topo; i >= 0; i--) {
        // Para cada elemento, imprime o nome e a idade formatados.
        printf("[%s, %d]\n", p->itens[i].nome, p->itens[i].idade);
    }
    // Imprime uma linha em branco no final para melhor formatação da saída.
    printf("\n");
}

// Função principal, onde a execução do programa começa.
int main() {
    // Declara uma variável do tipo 'Pilha' chamada 'p'. Esta será a nossa estrutura de dados principal.
    Pilha p;
    // Chama a função para inicializar a pilha, passando o endereço de memória de 'p'.
    // Isso define o topo como -1, indicando que a pilha começa vazia.
    inicializarPilha(&p);

    // Declara e inicializa três variáveis do tipo 'Pessoa' com seus respectivos nomes e idades.
    Pessoa a = {"Ana", 20};
    Pessoa b = {"Bruno", 35};
    Pessoa c = {"Carlos", 28};

    // Empilha a pessoa 'a' (Ana). Agora ela é o único elemento, na base e no topo.
    push(&p, a);
    // Empilha a pessoa 'b' (Bruno). Bruno fica no topo, acima de Ana.
    push(&p, b);
    // Empilha a pessoa 'c' (Carlos). Carlos se torna o novo topo da pilha.
    push(&p, c);

    // Chama a função para exibir o conteúdo da pilha.
    // A saída será: Carlos (topo), Bruno, Ana (base).
    mostrarPilha(&p);

    // Declara uma variável do tipo 'Pessoa' para armazenar o valor que será removido da pilha.
    Pessoa removida;

    // Desempilha o elemento do topo (Carlos) e o armazena na variável 'removida'.
    pop(&p, &removida);

    // Imprime os dados da pessoa que foi removida da pilha.
    printf("Removida: %s, %d\n", removida.nome, removida.idade);

    // Exibe a pilha novamente. Agora o topo é Bruno.
    // A saída será: Bruno (topo), Ana (base).
    mostrarPilha(&p);

    // Desempilha o elemento do topo (Carlos) e o armazena na variável 'removida'.
    pop(&p, &removida);

    // Imprime os dados da pessoa que foi removida da pilha.
    printf("Removida: %s, %d\n", removida.nome, removida.idade);

    // Exibe a pilha novamente. Agora o topo é Bruno.
    // A saída será: Bruno (topo), Ana (base).
    mostrarPilha(&p);

     // Desempilha o elemento do topo (Carlos) e o armazena na variável 'removida'.
    pop(&p, &removida);

    // Imprime os dados da pessoa que foi removida da pilha.
    printf("Removida: %s, %d\n", removida.nome, removida.idade);

    // Exibe a pilha novamente. Agora o topo é Bruno.
    // A saída será: Bruno (topo), Ana (base).
    mostrarPilha(&p);
    
    // Declara uma variável para "espiar" o topo. ATENÇÃO: a variável 'topo' não foi declarada, o que causará um erro de compilação.
    Pessoa topo;
    // O objetivo desta linha é chamar a função peek para copiar o elemento do topo atual (Bruno) para a variável 'topo', sem removê-lo.
    peek(&p, &topo);
    // Imprime os dados da pessoa que está no topo atual da pilha, com base no que foi copiado pela função 'peek'.
    printf("Topo atual: %s, %d\n", topo.nome, topo.idade);

    // Retorna 0 para o sistema operacional, indicando que o programa foi executado com sucesso.
    return 0;
}

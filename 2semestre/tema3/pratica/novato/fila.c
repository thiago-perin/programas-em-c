#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nome[30];
    int idade;
} Pessoa;

#define MAX 5

typedef struct {
    Pessoa itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

void inserir(Fila *f, Pessoa p) {
    if (filaCheia(f)) {
        printf("\n\nFila cheia. Não é possível inserir. \n");
        return;
    }

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX;
    f->total++;
}

void remover(Fila *f, Pessoa *p) {
    if (filaVazia(f)) {
        printf("\n\nFila vazia. Não é possível remover. \n");
        return;
    }

    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
}

void mostrarFila(Fila *f) {
    printf("\n\nFila: ");

    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
        printf("[%s, %d]", f->itens[idx].nome, f->itens[idx].idade);
    }

    printf("\n");
}

int main() {
    Fila f;
    inicializarFila(&f); // Inicializa a fila

    Pessoa p1 = {"João", 25}; // Descrição de pessoas que vão entrar na fila
    Pessoa p2 = {"Maria", 30};
    Pessoa p3 = {"Nathan", 28};

    // Insere algumas pessoas na fila
    inserir(&f, p1);
    inserir(&f, p2);
    inserir(&f, p3);

    mostrarFila(&f); // Mostra a fila antes da remoção

    // Remove uma pessoa da fila
    Pessoa removida;
    remover(&f, &removida); // Aqui usamos a função de remoção
    printf("Pessoa removida: %s, %d\n", removida.nome, removida.idade);
    mostrarFila(&f); // Mostra a fila depois da remoção

    // Remove uma pessoa da fila
    remover(&f, &removida); // Aqui usamos a função de remoção
    printf("Pessoa removida: %s, %d\n", removida.nome, removida.idade);
    mostrarFila(&f); // Mostra a fila depois da remoção


    return 0;
}

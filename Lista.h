#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

////////////////// ESTRUTURAS DA LISTA
typedef struct no No;
struct no{
    char Item;
    No* ptr_prox;
};
typedef struct{
    int size;
    No *ptr_primeiro, *ptr_ultimo;
} TipoLista;

///////////////// FUNCOES DA LISTA DINAMICA
void CriaLista(TipoLista *Lista);
int Vazia(TipoLista *Lista);
void Insere_last(char x, TipoLista *Lista);
void Busca(char x, TipoLista *Lista);
void Imprime(TipoLista *Lista);
int Remove_last(TipoLista *Lista);
void Destroi_lista(TipoLista *Lista);
void Legenda();


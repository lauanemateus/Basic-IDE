#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include "Lista.h"

void CriaLista(TipoLista *Lista){
    Lista->ptr_primeiro = (No*)malloc(sizeof(No));
    Lista->ptr_ultimo = Lista->ptr_primeiro;
    Lista->ptr_primeiro->ptr_prox = NULL;
    Lista->size=0;
}

int Vazia(TipoLista *Lista){
    if(Lista->ptr_primeiro == Lista->ptr_ultimo) return 1;
    return 0;
}

void Insere_last(char x, TipoLista *Lista){ 
    No *new_Node = (No*)malloc(sizeof(No));
    new_Node->Item =  x;
    if(Lista->size==0){
        Lista->ptr_ultimo = Lista->ptr_primeiro->ptr_prox = new_Node;
        Lista->size++;
        return;
    }
    Lista->ptr_ultimo->ptr_prox = new_Node;
    Lista->ptr_ultimo = new_Node;
    Lista->ptr_ultimo->ptr_prox = NULL;
    Lista->size++;
}

int Remove_last(TipoLista *Lista){
    if(Lista->size==0) return 0;
    Lista->size--;
    if(Lista->size==0){
        free(Lista->ptr_primeiro->ptr_prox);
        Lista->ptr_primeiro->ptr_prox = NULL;
        return 1;
    }
    No *aux = Lista->ptr_primeiro;
    while(aux->ptr_prox != Lista->ptr_ultimo){
        aux = aux->ptr_prox;
    }
    free(Lista->ptr_ultimo);
    Lista->ptr_ultimo = aux;
    Lista->ptr_ultimo->ptr_prox = NULL;
    return 1;
}

void Busca(char x, TipoLista *Lista){
    No *atual = Lista->ptr_primeiro;
    int p;
    for(p=1; p<=Lista->size; p++){
        atual = atual->ptr_prox;
        if(atual->Item==x){
            printf("%d está na posição %d\n", x,p);
            return;
        }
    }
    printf("O número %d não está na lista\n", x);
}

void Imprime(TipoLista *Lista){
    No* temp = Lista->ptr_primeiro->ptr_prox;
    if(Vazia(Lista)){
        printw("A Lista está vazia\n");
    }
    else{ 
        printw("Valores da Lista: ");
        while(temp!=NULL){
            printw("%c ", temp->Item);
            temp = temp->ptr_prox;
        }
        printw("\n");
    }
}

void Destroi_lista(TipoLista *Lista){
    No *temp, *aux;
    if(Lista->ptr_primeiro == NULL) return;
    temp = Lista->ptr_primeiro;
    while(temp->ptr_prox!=NULL){
        aux = temp->ptr_prox;
        free(temp);
        temp = aux;
    }
    //CriaLista(Lista);
}

void Legenda(){
    printf("\nDigite um número:\n");
        printf("(0)Sair do programa\n");
        printf("(1)Criar uma nova lista\n");
        printf("(2)Inserir um número no final da lista\n");
        printf("(3)Remover um ítem no final da lista\n");
}
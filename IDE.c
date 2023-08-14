// TRABALHO 2 DE AED2
// Dupla: Lauane Mateus Oliveira de Moraes e Nelsi de Sousa Barbosa Junior

// É necessario ter instalado a biblioteca ncurses
// Para compilar no linux, use o comando gcc IDE.c Lista.c Lista.h -o IDE -lncurses -ltinfo
// Esta é uma IDE que simula no terminal a interface de um compilador

// CARACTERÍSTICAS PRINCIPAIS
// O programa realiza auto completação de palavras através da tecla TAB, podendo ser ultilizada em qualquer momento da digitação
// A compilação pode ser feita sem fechar o programa, editando e compilando várias vezes em tempo real
// Deletar letras não atrapalha o funcionamento do autocomplete e do compilador
// O programa verifica se os fechamentos e abrimentos dos sinais '{', '}', '(', ')', '[', '], estão corretos
// O programa verifica a falta de ';' na linha
// O programa vertifica se as variáveis declaradas são palavras reservadas do c (inseridas na função inicializar())

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include "Lista.h"

#define MAX_N 1000060
#define ALFA 256

struct node{
    bool end;
    int prox[ALFA];
};
int idx=0;
struct node nodes[MAX_N];
char string[30];
int aux=0, cont_ch=0;
char last_c;
int eh_var, deu_enter,cont_var;

char fun_Reservadas[43][25]={"for\0", "if\0", "else\0", "atoi\0", "atol\0",
"rand\0", "free\0", "calloc\0", "malloc\0", "realloc\0", "atof\0" "do\n", "sizeof\0", "while\0",
"strlen\0", "strcat\0", "strncat\0", "strcpy\0", "strncpy\0", "strcmp\0", 
"strncmp\0", "strchr\0", "strrchr\0", "strstr\0", "strcspn\0", "strspn\0",
"strpbrk\0", "srtok\0", "cos\0", "cosh\0", "sin\0", "sinh\0", "tanh\0", "exp\0",
"log\0", "log10\0", "modf\0", "pow\0", "sqrt\0", "ceil\0", "fabs\0", "floor\0", "fmod\0"};

char strings_Reservadas[8][25]={"main\0","break\0", "continue\0", "default\0",
"return\0", "#include\0", "# include \0", "switch\0"};

char bibliotecas[8][25]={"stdio.h\0", "ncurses.h\0", "curses.h\0", "stdbool.h\0", "string.h\0", "stdlib.h\0", "stdio.h\0", "math.h\0"};

char var_Reservadas[17][25]={"char\0", "const int\0", 
"const char\0", "const float\0", "const double\0", "double\0", "float\0", "int\0", "long int\0", 
"long float\0", "short\0", "unsigned int\0", "unsigned long int\0", "long double\0", 
"unsigned char\0", "long long int\0", "void\0"};

char var_usuario[100][100];
int qtd_var=0;

bool inicializar();
void insert_trie(char* s);
int find_trie(char* s,int kesm);
void next_word(int t,int *kesm);
void autocomplete(char ch[], int *i, int *pass, int *mark, FILE **f, FILE **faux );
void send_to_file(char at, FILE **f, FILE **faux);
int escreve_arquivo_na_tela(FILE *arq, char ch[], int *i, TipoLista *Lista, char erros[][200], int linha_erro[], int *linha, int *e, FILE **f, FILE **faux, int *pass, int *mark);
FILE* retorna_conteudo_do_arq(FILE **dest);
void print_nline(int *linha, FILE **f, FILE **faux);
void menu();
int ide_base(char at, char ch[], int *i, TipoLista *Lista, char erros[][200], int linha_erro[], int *linha, int *e, FILE **f, FILE **faux, int *pass, int *mark);
int varre_arq_desde_inicio(char at, char ch[], int *i, TipoLista *Lista, char erros[][200], int linha_erro[], int *linha, int *e, FILE **f, FILE **faux, int *pass, int *mark);
void mydel();
int verifica_include(char at, char ch[], int *i, TipoLista *Lista, char erros[][200], int linha_erro[], int *linha, int *e, FILE **f, FILE **faux, int *pass, int *mark);
int ide(char *at, char ch[], int *i, TipoLista *Lista, char erros[][200], int linha_erro[], int *linha, int *e, FILE **f, FILE **faux, int *pass, int *mark);
bool eh_reservada(char ch[]);

int main(){
    FILE *f = fopen("arq.txt", "w"),
    *faux = fopen("arqaux.txt", "w");
    initscr();  
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    char ch[30];
    int i=0;

    TipoLista Lista;
    CriaLista(&Lista);
    char erros[100][200];
    int linha_erro[100];
    int e=0, linha=1;

    if(inicializar()) printw("Inicialização concluida - Dicionario inserido\n");
    else printw("Erro ao inicializar\n");
    refresh();
    menu();
    refresh();
    print_nline(&linha, &f, &faux);


    int mark = 0,pass = 0;
    char at;
    do{
        at = getch();
        if(ide_base(at,ch,&i,&Lista,erros,linha_erro,&linha, &e, &f, &faux, &pass,&mark)==0){
            endwin();
            return 0;
        }
    } while(at!=14); // F6
    refresh();
    endwin();
    return 0;
}

bool inicializar(){
    for(int i=0; i<43; i++){
        insert_trie(fun_Reservadas[i]);
    }
    for(int i=0; i<8; i++){
        insert_trie(strings_Reservadas[i]); 
    }
    for(int i=0; i<17; i++){
        insert_trie(var_Reservadas[i]);
    }
    for(int i=0; i<8; i++){
        insert_trie(bibliotecas[i]);
    }
    return 1;
}

void insert_trie(char* s){
    int curr=0;
    for(int i=0; i<strlen(s); i++){
        if(nodes[curr].prox[s[i]]==0)
            nodes[curr].prox[s[i]]=++idx;
        curr=nodes[curr].prox[s[i]];
    }
    nodes[curr].end=true;
}
int find_trie(char* s,int kesm){
    aux=0;
    int curr=0, i;
    for(i=0; i<strlen(s); i++){
        string[aux++]=s[i];
        if(nodes[curr].prox[s[i]]==0)
            return 0;
        curr=nodes[curr].prox[s[i]];
    }
    next_word(curr,&kesm);
    return 2;
}
void next_word(int t,int *kesm){
    if(nodes[t].end==true){
        string[aux]='\0';
        kesm--;
        if(kesm == 0)
            return;
    }
    for(int i=0; i<ALFA; i++){
        if(nodes[t].prox[i]!=0){
            string[aux++]=i;
            next_word(nodes[t].prox[i],kesm);
            if(kesm == 0)
                break;
            aux--;
        }
    }
}
void mydel(){
    int x,y;
    getyx(stdscr,y,x);
    mvaddch(y, x-1, ' ');
    move(y, x-1);
}
void autocomplete(char ch[], int *i, int *pass, int *mark, FILE **f, FILE **faux){
    ch[*i]='\0';
    if(*pass!=0)
    {
        for (int j = 0; j < *mark ; ++j)
        {
            mydel();
        }
    }
    int result=find_trie(ch,(*pass)++);
    if(result==2){
        *mark = 0;
        int j;
        for (j = *i; j < strlen(string); ++j)
        {
            (*mark)++;
            printw("%c",string[j]);
            send_to_file(string[j], f, faux);
        }
        *i=j;
        string[j]='\0';
        strcpy(ch, string);
    }else
        *pass = 0;
}

void send_to_file(char at, FILE **f, FILE **faux){
    char auxc;
    if(at == 7){
        cont_ch--;
        fclose(*f), fclose(*faux);
        *f = fopen("arq.txt", "r");
        *faux = fopen("arqaux.txt", "w");
        for(int i=0; i<cont_ch; i++){ // conteudo da tela(execto ultimo caracter) eh copiado para o arquivo auxiliar
            auxc = fgetc(*f);
            fputc(auxc, *faux);
        }
        fclose(*f), fclose(*faux);
        *f = fopen("arq.txt", "w");
        *faux = fopen("arqaux.txt", "r");
        for(int i=0; i<cont_ch; i++){ // conteudo do arquivo auxiliar eh copiado para o arquivo arq.txt
            auxc = fgetc(*faux);
            fputc(auxc, *f);
        }
    }
    else{
        fputc(at, *f);
        cont_ch++;
    }
}

FILE* retorna_conteudo_do_arq(FILE **orig){
    char auxc;
    FILE *dest;
    fclose(*orig);
    *orig = fopen("arq.txt", "r");
    dest = fopen("arqaux2.txt", "w");
    for(int i=0; i<cont_ch; i++){ // conteudo de orig eh copiado para o arquivo de dest
        auxc = fgetc(*orig);
        fputc(auxc, dest);
    }
    fclose(*orig), fclose(dest);
    *orig = fopen("arq.txt", "w");
    dest = fopen("arqaux2.txt", "r");
    for(int i=0; i<cont_ch; i++){ // conteudo do arquivo dest eh copiado para o arquivo orig
        auxc = fgetc(dest);
        fputc(auxc, *orig);
    }
    refresh();
    return dest;
}

int escreve_arquivo_na_tela(FILE *arq, char ch[], int *i, TipoLista *Lista, char erros[][200], int linha_erro[], int *linha, int *e, FILE **f, FILE **faux, int *pass, int *mark){
    char at;
    fclose(arq);
    arq = fopen("arqaux2.txt", "r");
    *linha=1;
    for(int j=0; j<cont_ch; j++){
        at = fgetc(arq);
        if(varre_arq_desde_inicio(at,ch,i,Lista,erros,linha_erro,linha,e,f,faux, pass,mark)==0)
            return 0;
            if(j==0) (*linha)++;
    }
    refresh();
    return 1;
}

int varre_arq_desde_inicio(char at, char ch[], int *i, TipoLista *Lista, char erros[][200], int linha_erro[], int *linha, int *e, FILE **f, FILE **faux, int *pass, int *mark){
    deu_enter=0;
    if(at!=' ' && at!='\n') {
        if((at>=0+'0' && at<=9+'9' && last_c==27)==0) last_c=at;   
    }
    if((at==' ' || at==',' || at==';' || at=='=') && eh_var!=0){  
        //printw(" (ch -> %s) ",ch);
        if(eh_reservada(ch)){
            strcpy(erros[(*e)],"Variável declarada é uma palavra reservada da linguagem C\0");
            linha_erro[(*e)++] = *linha-1;
        }
        /*
        else{
            int j;
            for(j=0; j<qtd_var; j++){
                if(strcmp(ch, var_usuario[j])==0){
                    strcpy(erros[(*e)],"Autodeclaração de variável\0");
                    printw("auto de (%s) ", var_usuario[j]);
                    linha_erro[(*e)++] = *linha-1;
                    break;
                }
            }
            if(j==qtd_var){
                strcpy(var_usuario[qtd_var++], ch);
            }
        }*/
    }
    if(at==')'){
        if(*i!=0) ch[*i]='\0';
        *i=0;
        printw("%c", at);
        refresh();
        if(Lista->size==0 || Lista->ptr_ultimo->Item!='('){
            strcpy(erros[(*e)],"Parêntes ')' não está completo\0");
            linha_erro[(*e)++] = *linha-1;
        }
        else{
            Remove_last(Lista);
        }
    }
    else if(at=='}'){
        if(*i!=0) ch[*i]='\0';
        *i=0;
        printw("%c", at);
        refresh();
        if(Lista->size==0 || Lista->ptr_ultimo->Item!='{'){
            strcpy(erros[(*e)],"Chave '}' não está completa\0");
            linha_erro[(*e)++] = *linha-1;
        }
        else{
            Remove_last(Lista);
        }
    }
    else if(at==']'){
        if(*i!=0) ch[*i]='\0';
        *i=0;
        printw("%c", at);
        refresh();
        if(Lista->size==0 || Lista->ptr_ultimo->Item!='['){
            strcpy(erros[(*e)],"Chave '[' não está completa\0");
            linha_erro[(*e)++] = *linha-1;
        }
        else{
            Remove_last(Lista);
        }
    }
    else if(at=='>' || at=='<' || at==',' || at==';' || at=='='){
        if(*i!=0) ch[*i]='\0';
        *i=0;
        pass = 0;
        printw("%c",at);
        refresh();
    }
    else if(at=='(' || at=='{' || at=='['){
        if(*i!=0) ch[*i]='\0';
        *i=0;
        printw("%c", at);
        //printw("ch-> %s ",ch);
        refresh(); 
        Insere_last(at, Lista);
    }
    else if(at == ' ')
    {
        deu_enter=1;
        if(*i!=0) ch[*i]='\0';
        *i=0;
        pass = 0;
        printw("%c",at);
        for(int j=0; j<17; j++){
            if(strcmp(ch, var_Reservadas[j])==0){
                eh_var=1;
                //printw(" ehh var ");
                break;
            }
        }
    }else if(at == '\n'){
        //printw(" last_c=%d ",(int)last_c);
        eh_var=0;
        if(last_c!='}' && last_c!='{' && last_c!='>' && last_c!=';' && last_c!=27){
            strcpy(erros[(*e)],"Falta ';'\0");
            linha_erro[(*e)++] = *linha-1;
        }
        last_c=27;
        if(*i!=0) ch[*i]='\0';
        *i=0;
        pass = 0;
        printw("%c",at);
        (*linha)++;
    }
    else if(at==14){
        return 0;
    }
    else{
        *pass = 0;
        printw("%c", at);
        ch[(*i)++]=at;
        ch[(*i)]='\0';
    }
    return 1;
}

void print_nline(int *linha, FILE **f, FILE **faux){
    printw("%2d ", *linha);
    if((*linha)<10){
        send_to_file(' ',f,faux);
        send_to_file((*linha)+'0',f,faux);
        send_to_file(' ',f,faux);
    }
    else{
        send_to_file((*linha)/10 + '0',f, faux);
        send_to_file((*linha)%10 + '0',f,faux);
        send_to_file(' ',f,faux);
    }
    refresh();
    (*linha)++;
    //printw(" linha agora eh=%d ", *linha);
}

void menu(){
    printw("Pressione F6 para finalizar\n");
    printw("Pressione TAB caso queira realizar o autocomplete\n");
    printw("Digite F5 para compilar o código\n\n");
}

int ide_base(char at, char ch[], int *i, TipoLista *Lista, char erros[][200], int linha_erro[], int *linha, int *e, FILE **f, FILE **faux, int *pass, int *mark){

    // COMPILA
    if(at == 13){ // F5
        last_c=27; // (27 - ESC) é o caracter de controle
        eh_var=0, deu_enter=0, qtd_var=0,cont_var=0;
        clear();
        menu();
        fclose(*faux);
        *i=0;
        *e=0;
        Destroi_lista(Lista);
        CriaLista(Lista);
        *faux = retorna_conteudo_do_arq(f);
        if(escreve_arquivo_na_tela(*faux,ch,i, Lista, erros,linha_erro, linha, e, f,faux, pass, mark)==0)
            return 0;

        printw("\n------------------------------------------------------------------------------\n");
        if(last_c!='}' && last_c!='{' && last_c!='>' && last_c!=';' && last_c!=27){
            strcpy(erros[(*e)],"Falta ';'\0");
            linha_erro[(*e)++] = *linha-1;
        }
        if(eh_var!=0 && deu_enter==0){  
            if(eh_reservada(ch)){
                strcpy(erros[(*e)],"Variável declarada é uma palavra reservada da linguagem C\0");
                linha_erro[(*e)++] = *linha-1;
            } 
            /* 
            else{
                int j;
                for(j=0; j<qtd_var; j++){
                    if(strcmp(ch, var_usuario[j])==0){
                        strcpy(erros[(*e)],"Autodeclaração de variável\0");
                        linha_erro[(*e)++] = *linha-1;
                        break;
                    }
                }
                if(j==qtd_var){
                    strcpy(var_usuario[qtd_var++], ch);
                }
            }*/
        }
        if(*e==0 && Lista->size==0) {
            printw("Não há erros de compilação.\n");
            refresh();
        }
        else{
            for(int j=0; j<*e; j++){
                printw("Linha %d: %s\n", linha_erro[j], erros[j]);
            }
            if(Lista->size>0){
                printw("Há '{', '[', '(' não fechado(s).\n");
            }
        }
        
        printw("Compilação terminada. Clique F5 para voltar ao código\n"); 
        refresh();
        at = getch();
        if(at == 13){
            clear();
            menu();
            fclose(*faux);
            *i=0;
            *e=0;
            Destroi_lista(Lista);
            CriaLista(Lista);
            *faux = retorna_conteudo_do_arq(f);
            if(escreve_arquivo_na_tela(*faux,ch,i, Lista, erros,linha_erro, linha, e, f,faux, pass, mark)==0)
                return 0;
        }
        else return 0;
    }
    else if(at==')' || at=='(' || at=='{' || at=='}' || at=='[' || at==']' || at=='<' || at=='>'){
        if(*i!=0) ch[*i]='\0';
        *i=0;
        printw("%c", at);
        refresh();
        send_to_file(at, f, faux);
    }
    else if(at == ' ')
    {
        ch[*i]='\0';
        *i=0;
        send_to_file(at, f, faux);
        pass = 0;
        ch[*i]='\0';
        printw("%c",at);
    }else if(at == '\n'){
        ch[*i]='\0';
        *i=0;
        send_to_file(at, f, faux);
        pass = 0;
        printw("%c",at);
        //printw(" enter -> imprimir linha=%d ", *linha);
        print_nline(linha, f, faux);
    }else if(at == 7){ // Backespace
        send_to_file(at, f, faux);
        refresh();
        *pass = 0;
        mydel();
        if((*i)>0) (*i)--;
    }
    else if(at == 9 && (*i)>=2) // tecla TAB
    { 
        autocomplete(ch, i, pass, mark, f, faux);
        
    }
    else if(at==14){ // F6
        return 0;
    }
    else{
        send_to_file(at, f, faux);
        *pass = 0;
        printw("%c", at);
        ch[(*i)++]=at;
        ch[(*i)]='\0';
    }
    return 1;
}

bool eh_reservada(char ch[]){
    for(int i=0; i<43; i++){
        if(strcmp(fun_Reservadas[i], ch)==0)
            return 1;
    }
    for(int i=0; i<8; i++){
        if(strcmp(strings_Reservadas[i], ch)==0)
            return 1; 
    }
    for(int i=0; i<8; i++){
        if(strcmp(bibliotecas[i], ch)==0)
            return 1;
    }
    for(int i=0; i<17; i++){
        if(strcmp(var_Reservadas[i], ch)==0)
            return 1;
    }
    return 0;
}

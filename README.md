# Basic Compiler

  **TRABALHO DE Algorítimo e Estrutura de Dados 2**
  
  Dupla: Lauane Mateus Oliveira de Moraes e Nelsi de Sousa Barbosa Junior

  É necessario ter instalado a biblioteca ncurses
  Para compilar no linux, use o comando gcc IDE.c Lista.c Lista.h -o IDE -lncurses -ltinfo
  Esta é uma IDE que simula no terminal a interface de um compilador
  
  **CARACTERÍSTICAS PRINCIPAIS**
  
  * O programa realiza auto completação de palavras através da tecla TAB, podendo ser ultilizada em qualquer momento da digitação
  * A compilação pode ser feita sem fechar o programa, editando e compilando várias vezes em tempo real
  * Deletar letras não atrapalha o funcionamento do autocomplete e do compilador
  * O programa verifica se os fechamentos e abrimentos dos sinais '{', '}', '(', ')', '[', '], estão corretos 
  * O programa verifica a falta de ';' na linha
  * O programa vertifica se as variáveis declaradas são palavras reservadas do c (inseridas na função inicializar())

  ------------------------------------------------------------------------------------------------

Pair: Lauane Mateus Oliveira de Moraes and Nelsi de Sousa Barbosa Junior

It is necessary to have the ncurses library installed.
To compile on Linux, use the command: gcc IDE.c Lista.c Lista.h -o IDE -lncurses -ltinfo.
This is an IDE that simulates a compiler interface in the terminal.

** MAIN FEATURES **

* The program performs word autocompletion using the TAB key, and it can be used at any time during typing.
* Compilation can be done without closing the program, allowing editing and compiling multiple times in real-time.
* Deleting letters does not interfere with the functionality of autocompletion and the compiler.
* The program checks if the opening and closing of {, }, (, ), [, ] are correct.
* The program checks for missing ; in the line.
* The program checks if the declared variables are reserved C keywords (inserted in the initialize() function).

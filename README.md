# Basic-IDE

  **TRABALHO DE AED2**
  
  Dupla: Lauane Mateus Oliveira de Moraes e Nelsi de Sousa Barbosa Junior

  É necessario ter instalado a biblioteca ncurses
  Para compilar no linux, use o comando gcc IDE.c Lista.c Lista.h -o IDE -lncurses -ltinfo
  Esta é uma IDE que simula no terminal a interface de um compilador
  
  **CARACTERÍSTICAS PRINCIPAIS**
  
  O programa realiza auto completação de palavras através da tecla TAB, podendo ser ultilizada em qualquer momento da digitação
  
  A compilação pode ser feita sem fechar o programa, editando e compilando várias vezes em tempo real
  
  Deletar letras não atrapalha o funcionamento do autocomplete e do compilador
  
  O programa verifica se os fechamentos e abrimentos dos sinais '{', '}', '(', ')', '[', '], estão corretos
  
  O programa verifica a falta de ';' na linha
  
  O programa vertifica se as variáveis declaradas são palavras reservadas do c (inseridas na função inicializar())

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX 256
#define MAX_ARGS 50

int main() {
  char comando[MAX];
  char *comandos[MAX_ARGS]; 
  const char espac[2] = " ";
  int pid;
  int concorrente, red_input, red_output;
  char *input;
  char *output;

  while (1) {
    // Sanitiza todas as variaveis de controle e cadeias de caracteres;
    concorrente = 0;
    red_input = 0;
    red_output = 0;
    input = NULL;
    output = NULL;
    memset(comandos, 0, MAX_ARGS*sizeof(char*));

    // Inicia o loop
    printf("> ");
    fgets(comando, MAX , stdin);

    // Caso nenhum input, volte ao inicio;
    if (strcmp(comando, "\n") == 0) {
      continue;
    }
    
    // Retira a terminacao \n da string de comando e substitui por um \0;
    comando[strcspn(comando, "\n")] = 0;

    //Caso de saida
    if (!strcmp(comando, "exit")) {
      exit(EXIT_SUCCESS);
    }

    // TOKENIZANDO A ENTRADA

    //primeiro token com a string alvo;
    comandos[0] = strtok(comando, espac);

    //Pendente sera analisado a cada iteracao para entrar na lista de argumentos ou apenas setar uma variavel de controle;
    char* pendente = NULL;

    // Enquanto nao chegamos ao limite de argumentos ou atingimos a terminacao da cadeia original, consumimos um token;
    int i = 1;
    while (i < MAX_ARGS && (pendente = strtok(NULL, espac)) != NULL) {

      // Se for um &, setamos a variavel de concorrencia para verdadeira e prosseguimos;
      if (strcmp(pendente, "&") == 0) {
        concorrente = 1;
        continue;
      }

      // Se for um <, setamos a variavel de redirecionamento de entrada para verdadeira e prosseguimos;
      if (strcmp(pendente, "<") == 0) {
        red_input = 1;
        input = strtok(NULL, espac);
        continue;
      }

      // Se for um >, setamos a variavel de redirecionamento de saida para verdadeira e prosseguimos;
      if (strcmp(pendente, ">") == 0) {
        red_output = 1;
        output = strtok(NULL, espac);
        continue;
      }

      // Se chegou aqui, o token pendente eh um argumento portanto o inserimos na string de comandos;
      comandos[i] = pendente;
      i++;
    }

    pid = fork();
    if (pid) {
      // Se possui algum &, o processo pai deve continuar sem esperar o processo filho acabar;
      if (!concorrente) {
        waitpid(pid, NULL, 0);
      } else {
        continue;
      }
      
    } else {
      // Analisamos as variaveis de controle para ver se ha necessidade de redirecionar a entrada ou saida; 
      if (red_input) {
        freopen(input, "r+",stdin);
      }
      if (red_output) {
        freopen(output, "w+",stdout);
      }

      // Executamos o programa no primeiro argumento da entrada, com o restante dos argumentos;
      execvp(comandos[0], comandos);
      printf("Erro ao executar comando!\n");
      exit(EXIT_FAILURE);
    }
  }
}

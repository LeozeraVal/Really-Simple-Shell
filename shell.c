#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX 256
#define MAX_ARGS 50

int main() {
  char comando[MAX];
  char* comandos[MAX_ARGS] = {NULL}; 
  const char espac[2] = " ";
  int pid;
  int concorrente = 0;

  while (1) {
    concorrente = 0;
    printf("> ");
    fgets(comando, MAX , stdin);
    if (strcmp(comando, "\n") == 0) {
      continue;
    }
    
    comando[strcspn(comando, "\n")] = 0;
    //scanf("%s", comando);
    if (!strcmp(comando, "exit")) {
      exit(EXIT_SUCCESS);
    }

    //Quebra o comando os tokens
    comandos[0] = strtok(comando, espac);
    for (size_t i = 1; i < MAX_ARGS && comandos[i-1] != NULL; i++) {
      if (!strcmp(comandos[i-1], "&")) {
        concorrente = 1;
        comandos[i-1] = NULL;
      }
      
      comandos[i] = strtok(NULL, espac);
    }

    pid = fork();
    if (pid) {
      // Se possui algum &, o processo deve continuar sem esperar o processo filho acabar
      if (!concorrente) {
        waitpid(pid, NULL, 0);
      } else {
        continue;
      }
      
    } else {
      // Usar fopen pra redirecionar a entrada e saida padrão antes do exec
      //execlp(comando, comando, NULL);
      execvp(comandos[0], comandos);
      printf("Erro ao executar comando!\n");
      exit(EXIT_FAILURE);
    }
  }
}

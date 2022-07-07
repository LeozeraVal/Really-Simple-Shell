#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX 256

int main() {
  char comando[MAX];
  char* comandos[50] = {NULL}; 
  const char espac[2] = " ";
  int pid;

  while (1) {
    printf("> ");
    fgets(comando, MAX , stdin);
    comando[strcspn(comando, "\n")] = 0;
    //scanf("%s", comando);
    if (!strcmp(comando, "exit")) {
      exit(EXIT_SUCCESS);
    }

    //pid = fork();
    if (0){//pid) {
      waitpid(pid, NULL, 0); 
    } else {
      // Usar fopen pra redirecionar a entrada e saida padrão antes do exec
      comandos[0] = strtok(comando, espac);
      for (size_t i = 1; i < 50 && comandos[i-1] != NULL; i++) {
        comandos[i] = strtok(NULL, espac);
      }
      
      //execlp(comando, comando, NULL);
      execvp(comandos[0], comandos);
      printf("Erro ao executar comando!\n");
      exit(EXIT_FAILURE);
    }
  }
}

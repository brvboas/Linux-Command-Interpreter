#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define MAX 256
#define MAXARG 10
 
int main() {
 //variáveis  
 char comando[MAX];
 char *argumentos[MAXARG];
 char *pch;
 char *arg;
 char *retira;
 int pid;
 int i=0;
 int salvaNome = 0;
 int eComercial=0;
 int entradaSaida;
 char *nomeArquivo;
 FILE *arq;

 while(1){
  entradaSaida = 0;
  //atribui NULL para o vetor de argumentos;
  for(i=0;i<MAXARG;i++)
   argumentos[i] = NULL;
 
  i = 1;
  printf("> "); 
  fgets(comando,MAX,stdin);
  
  //retira o \n do final do comando que é feito com o stdin no fgets
  retira = strtok(comando,"\n");
  strcpy(comando,retira);

  if (strcmp(comando," ")){  
    if (!strcmp(comando, "exit")) {
     exit(EXIT_SUCCESS);
  }

  pch = strtok(comando, " ");
  argumentos[0] = pch;
  pch = strtok(NULL, " ");
 
  while(pch != NULL){
 
    salvaNome = 0;
    arg = pch;

    // se o argumento for & a variavel nao passará pelo waitpid depois;
    if (!strcmp(arg,"&"))
     eComercial = 1;
 
    else{
     // se o argumento for > quer dizer que o interpretador deve salvar alguma coisa em um arquivo e, futuramente, abrirá o arquivo passado para escrita("w");

     if(pch[0] == '>'){
      salvaNome = 1;
      entradaSaida = 1;
     }
 
     // se o argumento for < quer dizer que o interpretador abrirá um arquivo para leitura("r");
     else{
       if(pch[0] == '<'){
       salvaNome = 1;
       entradaSaida = 2;
     }

     else{
      argumentos[i] = arg;
      i++;
     }
}
    }

   //armazena o nome do arquivo na variável 'nomeArquivo';
   if (salvaNome == 1){
	pch = strtok(NULL, " ");
	nomeArquivo=pch;
   }
   pch = strtok(NULL, " ");
  }
 
	

  //executa o fork
  pid = fork();
  if (pid) {

   //caso nao tenha um "&" entre os argumentos ele executa o waitpid();
   if (!eComercial)
    waitpid(pid, NULL, 0);
  }
  else {
   
   // se precisar escrever em um arquivo executará aqui
   if(entradaSaida==1)
    arq = freopen(nomeArquivo, "w", stdout);
   
  // caso contrario executará aqui 
   if(entradaSaida==2)
    arq = freopen(nomeArquivo, "r",stdin);
   
  //finalmente o comando e seus argumentos sao executados pelo execvp 
   execvp(comando,argumentos);
   fclose(stdin);
   printf("Erro ao executar comando!\n");
   exit(EXIT_FAILURE);
  }
 }
}
}

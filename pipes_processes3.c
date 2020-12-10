#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/wait.h> 
#include <stdlib.h>

int main(int argc, char **argv)
{  
  pid_t pid_1;
  pid_t pid_2;
  
  int pipefd1[2];
  int pipefd2[2];
  
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL, NULL};
  char *cat_args[] = {"cat", "scores", NULL};

  pipe(pipefd1);
  pipe(pipefd2);

  pid_1 = fork();

  int i = 0;
  if (pid_1 != 0){
    dup2(pipefd1[1], 1);
    
    for (i = 0; i <= 1; i++){
      close(pipefd1[i]);
      close(pipefd2[i]);
    }
    
    execvp(cat_args[0], cat_args);
  }else{
    pid_2 = fork();
    
    if (pid_2 == 0){       
      dup2(pipefd2[0], 0);

      for (i = 0; i <= 1; i++){
        close(pipefd1[i]);
        close(pipefd2[i]);
      }

      execvp(sort_args[0], sort_args);
    }else{
      dup2(pipefd1[0], 0);
      dup2(pipefd2[1], 1);

      for (i = 0; i <= 1; i++){
        close(pipefd1[i]);
        close(pipefd2[i]);
      }
      
      execvp(grep_args[0], grep_args);
     }
  }
  
  wait(NULL);

  return 0;
}

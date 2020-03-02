#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
void sig_handler(int signal) {
  int status;
  int result = wait(&status);

  printf("Wait returned %d\n", result);
}

main() {
  int output;
  int input;
  char *output_filename;
  char *input_filename;

  int i;
  char **args[10][200];
  int index=0;
  int result;
  int block;
 
 
  sigset(SIGCHLD, sig_handler);

  while(1) {
    printf("#");


    char temp[200];
    gets(temp);

    char* buff = strtok(temp," ");

    while(buff!=NULL)
    {
      strcpy(args[index],buff);
      index++;
      buff = strtok(NULL," ");
    }
    printf("%i\n",index );


    if(args[0] == NULL)
      continue;
     
    if(strcmp(args[0], "exit") == 0) {
        exit(0);
      }

    
    input = redirect_input(args, &input_filename, index);
  
    output = redirect_output(args, &output_filename, index);

    do_command(args, block, input, input_filename, output, output_filename);
  }
}

int do_command(char **args[10][200], int block,int input, char *input_filename, int output, char *output_filename) {
  
  pid_t child_id;
  int status;
  int result;
  
  child_id = fork();
  if(child_id == 0) {

    if(input)
      freopen(input_filename, "r", stdin);

    if(output)
      freopen(output_filename, "w+", stdout);

    printf("%s\n", "Debug");
    result = execvp(args[0], args);

    exit(-1);
  }

  if(block) {
    printf("Waiting for child, pid = %d\n", child_id);
    result = waitpid(child_id, &status, 0);
  }
}


int redirect_input(char **args[10][200], char **input_filename, int ind) {
  int i;
  int j;

      
  for(i = 0; i < ind ; i++) {
    printf("%s\n", args[i]);

    if( strcmp(args[i],"<")==0) {
      


      free(args[i]);

      if(i+1 < ind) {
	     *input_filename = args[i+1];
      } 
      else {
	     return -1;
      }

      for(j = i; j-1 < ind; j++) {

        strcpy(args[j],args[j+2]);
      }

      return 1;
    }
  }

  return 0;
}

int redirect_output(char **args[10][200], char **output_filename, int ind) {
  int i;
  int j;

  for(i = 0; i < ind; i++) {

    if( strcmp(args[i],">")==0) {
      free(args[i]);

      if(i+1 < ind) {
	     *output_filename = args[i+1];
      } 
      else {
	     return -1;
      }

      for(j = i; j-1 < ind; j++) {
	       strcpy(args[j],args[j+2]);
      }

      return 1;
    }
  }

  return 0;
}
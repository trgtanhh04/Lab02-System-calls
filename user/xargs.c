#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

#define MAXARG_LEN 512

void
copy_argv(char **ori_argv, int ori_argc, char *new_argv, char **argv)
{
  int k = 0;
  for (int i = 0; i < ori_argc; i++) 
  {
    argv[k] = malloc(strlen(ori_argv[i]) + 1);
    memcpy(argv[k++], ori_argv[i], strlen(ori_argv[i]) + 1);
  }
  argv[k] = malloc(strlen(new_argv) + 1);
  memcpy(argv[k++], new_argv, strlen(new_argv) + 1);
}

int 
main(int argc, char *argv[])
{
  if (argc <= 1) 
  {
    fprintf(2, "Usage: xargx [command] [arg ...]\n");
    exit(1);
  }

  char param[MAXARG_LEN];
  int i = 0;
  char ch;
  int ignore = 0;
  while (read(0, &ch, 1) > 0)
  {
    if (ch == '\n') 
    {
      if (ignore) 
      {
        i = 0;
        ignore = 0;
        continue;
      }
      param[i] = 0;
      i = 0;

      int pid = fork();
      if (pid == 0) 
      {
        // Child process
        int cmd_argc = argc;
        
        char *cmd_argv[MAXARG_LEN];

        copy_argv(argv + 1, argc - 1, param, cmd_argv);
        cmd_argv[cmd_argc] = 0;
        
        exec(cmd_argv[0], cmd_argv);

        exit(0);
      }
      else 
      {
        wait((int *)0);
      }
      
    }
    else
    {
      if (!ignore && i >= MAXARG_LEN - 1) 
      {
        printf("xargs: too long arguments...\n");
        ignore = 1;
      }

      if (!ignore) 
      {
        param[i++] = ch;
      }
    }
  }

  exit(0);
}
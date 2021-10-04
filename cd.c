#include "header.h"

char helper_buf[1000];

void pwd()
{
  printf("%s\n", cust_cwd(0));
}

char *cust_cwd(int shorten)
{
  if (getcwd(helper_buf, 1000) == NULL)
  {
    perror("Error");
    return "";
  }
  if (!shorten)
  {
    return helper_buf;
  }

  if (check_prefix(helper_buf, shell_home))
  {
    // int l = strlen(shell_home);
    // int n = strlen(helper_buf);
    char *new_path = malloc(strlen(helper_buf));
    strcpy(new_path, "~");
    strcat(new_path, helper_buf + strlen(shell_home));
    strcpy(helper_buf, new_path);
    free(new_path);
  }

  return helper_buf;
}

void cd(char **args)
{
  int l = arg_len(args);
  if (l > 2)
  {
    printf("cd cannot have more than 1 argument\n");
    return;
  }

  int cd_ret;

  if (l == 1 || strcmp(args[1], "~") == 0)
  {
    cd_ret = chdir(shell_home);
  }

  else if (args[1][0] == '~')
  {
    int pathlen = strlen(args[1]);
    char *new_path = malloc(strlen(shell_home) + pathlen + 20);
    strcpy(new_path, shell_home);
    if (pathlen > 1)
    {
      strcat(new_path, args[1] + 1);
    }

    cd_ret = chdir(new_path);

    free(new_path);
  }

  else if (strcmp(args[1], "-") == 0)
  {
    if (chdir(shell_home) < 0)
    {
      perror("Error");
      return;
    }
    pwd();
    return;
  }

  else if (args[1][0] == '/')
  {
    cd_ret = chdir(args[1]);
  }
  else
  {
    int cwd_len = strlen(cust_cwd(0));
    char *s = malloc(cwd_len + strlen(args[1]) + 5);
    strcpy(s, cust_cwd(0));
    if (s[cwd_len - 1] != '/')
    {
      strcat(s, "/");
    }
    strcat(s, args[1]);

    cd_ret = chdir(s);
    free(s);
  }

  // checking if execution proper
  if (cd_ret < 0)
  {
    perror("cd error");
  }
}
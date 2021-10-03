#include "header.h"

void get_process_path(char **path, int pid)
{
  char *s = malloc(50);
  sprintf(s, "/proc/%d/exe", pid);
  if (readlink(s, *path, 500) == -1)
  {
    perror("Error");
  }
  free(s);
}

void pinfo(char **args)
{
  int l = arg_len(args);

  if (l > 2)
  {
    printf("pinfo does not take more than 1 argument\n");
    return;
  }

  char *stat_file = malloc(200);
  if (l == 1)
  {
    sprintf(stat_file, "/proc/self/stat");
  }
  else
  {
    sprintf(stat_file, "/proc/%s/stat", args[1]);
  }

  int pid = (l == 1) ? getpid() : atoi(args[1]);

  FILE *fptr;
  fptr = fopen(stat_file, "r");
  if (fptr == NULL)
  {
    perror("Error");
    return;
  }

  char *info = malloc(1000);
  strcpy(info, "");

  char c;
  c = fgetc(fptr);
  int i;
  for (i = 0; c != EOF; i++)
  {
    info[i] = c;
    c = fgetc(fptr);
  }
  info[i] = '\0';

  char **parsed_stats = convert_to_array(info);

  // getting process status
  char *process_status = malloc(10);
  strcpy(process_status, parsed_stats[2]);
  if (pid == shell_pid)
  {
    strcat(process_status, "+");
  }

  // getting path
  char *path = malloc(500);
  get_process_path(&path, pid);

  strcpy(info, "");
  sprintf(info, "pid -- %d\nProcess Status -- %s\nmemory -- %s bytes\nExecutable Path -- %s\n",
          pid,
          process_status,
          parsed_stats[22],
          path);

  printf("%s", info);

  // freeing up stuff
  for (int j = 0; 1; j++)
  {
    if (parsed_stats[j] == NULL)
      break;
    free(parsed_stats[j]);
  }
  free(parsed_stats);
  free(stat_file);
  free(path);
  free(process_status);
  free(info);
  fclose(fptr);
}
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

void get_stats(pid_t pid, char ***parsed_stats)
{
  char *stat_file = malloc(200);
  sprintf(stat_file, "/proc/%d/stat", pid);

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

  *parsed_stats = convert_to_array(info);

  free(info);
  free(stat_file);
  fclose(fptr);
}

void get_proc_name(pid_t pid, char **name)
{
  char **parsed_stats;
  get_stats(pid, &parsed_stats);

  *name = realloc(*name, strlen(parsed_stats[1]) + 2);
  strcpy(*name, parsed_stats[1] + 1);
  (*name)[strlen(*name) - 1] = '\0';

  for (int j = 0; 1; j++)
  {
    if (parsed_stats[j] == NULL)
      break;
    free(parsed_stats[j]);
  }
  free(parsed_stats);
}

void pinfo(char **args)
{
  int l = arg_len(args);

  if (l > 2)
  {
    printf("pinfo does not take more than 1 argument\n");
    return;
  }

  pid_t pid = (l == 1) ? getpid() : atoi(args[1]);

  char **parsed_stats;
  get_stats(pid, &parsed_stats);

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

  char info[500];
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
  free(path);
  free(process_status);
}
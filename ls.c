#include "header.h"

void set_perms(mode_t mode, char **perms)
{
  (*perms)[0] = (S_ISDIR(mode)) ? 'd' : '-';
  (*perms)[1] = (mode & S_IRUSR) ? 'r' : '-';
  (*perms)[2] = (mode & S_IWUSR) ? 'w' : '-';
  (*perms)[3] = (mode & S_IXUSR) ? 'x' : '-';
  (*perms)[4] = (mode & S_IRGRP) ? 'r' : '-';
  (*perms)[5] = (mode & S_IWGRP) ? 'w' : '-';
  (*perms)[6] = (mode & S_IXGRP) ? 'x' : '-';
  (*perms)[7] = (mode & S_IROTH) ? 'r' : '-';
  (*perms)[8] = (mode & S_IWOTH) ? 'w' : '-';
  (*perms)[9] = (mode & S_IXOTH) ? 'x' : '-';
  (*perms)[10] = '\0';
}

int print_stats(char *file, char *file_path, int l)
{
  if (!l)
  {
    printf("%s\n", file);
    return 0;
  }

  struct stat stat_buf;
  if (stat(file_path, &stat_buf) == -1)
  {
    perror("stat error");
    return 0;
  }

  struct passwd *owner = getpwuid(stat_buf.st_uid);
  if (owner == NULL)
  {
    perror("Error");
  }

  struct group *grp = getgrgid(stat_buf.st_gid);
  if (grp == NULL)
  {
    perror("Error");
  }

  char *perms = malloc(20);
  set_perms(stat_buf.st_mode, &perms);

  char *time_buf = malloc(50);

  time_t curr_time;
  time(&curr_time);

  if (difftime(mktime(localtime(&curr_time)), mktime(localtime(&(stat_buf.st_mtime)))) > 15778476)
  {
    // print year
    strftime(time_buf, 50, "%b %d %Y", localtime(&(stat_buf.st_mtim.tv_sec)));
  }
  else
  {
    // print time
    strftime(time_buf, 50, "%b %d %H:%M", localtime(&(stat_buf.st_mtim.tv_sec)));
  }

  printf("%s %ld %s %s %ld %s %s\n", perms, stat_buf.st_nlink, owner->pw_name, grp->gr_name, stat_buf.st_size, time_buf, file);

  return stat_buf.st_blocks;

  // // free stuff
  free(time_buf);
  free(perms);
}

void set_flags(char *s, int *a, int *l)
{
  int n = strlen(s);
  for (int i = 0; i < n; i++)
  {
    if (s[i] == 'a')
    {
      *a = 1;
    }
    else if (s[i] == 'l')
    {
      *l = 1;
    }
  }
}

void ls(char **args)
{
  int a = 0, l = 0;
  char **paths = malloc(sizeof(char *));
  paths[0] = malloc(1);
  paths[0] = NULL;

  int n = arg_len(args);
  for (int i = 1; i < n; i++)
  {
    if (args[i][0] == '-')
    {
      set_flags(args[i], &a, &l);
    }
    else
    {
      add_to_array(&paths, args[i]);
    }
  }

  if (arg_len(paths) == 0)
  {
    add_to_array(&paths, cust_cwd(0));
  }

  int no_of_paths = arg_len(paths);

  // declare required variables

  for (int i = 0; i < no_of_paths; i++)
  {

    char *parsed_path = malloc(1000);
    char *parsed_file_path = malloc(500);

    parse_path(paths[i], &parsed_path);

    if (no_of_paths > 1)
    {
      printf("%s:\n", parsed_path);
    }

    // check if file exists
    struct stat stat_buf;
    if (stat(parsed_path, &stat_buf) == -1)
    {
      perror("Path Error");
      return;
    }

    // file exists:
    else
    {
      // check if file or directory:
      if (S_ISDIR(stat_buf.st_mode))
      {
        // dir
        DIR *dir_ptr = opendir(parsed_path);
        if (dir_ptr == NULL)
        {
          perror("Error");
          return;
        }

        struct dirent *dir_ent;

        errno = 0;
        dir_ent = readdir(dir_ptr);
        if (errno != 0 && dir_ent == NULL)
        {
          perror("Dir Error");
          return;
        }
        int total = 0;
        while (dir_ent != NULL)
        {
          // add file to dir path
          sprintf(parsed_file_path, "%s/%s", parsed_path, dir_ent->d_name);
          if (a || (dir_ent->d_name)[0] != '.')
          {
            total += print_stats(dir_ent->d_name, parsed_file_path, l);
          }
          errno = 0;
          dir_ent = readdir(dir_ptr);
          if (errno != 0 && dir_ent == NULL)
          {
            perror("Dir Error");
            return;
          }
        }
        if (l)
        {
          printf("total %d\n", total / 2);
        }
        printf("\n");
      }
      else
      {
        // file
        print_stats(paths[i], parsed_path, l);
        printf("\n");
      }
    }
    free(parsed_path);
    free(parsed_file_path);
  }
  // free stuff

  for (int j = 0; j < no_of_paths; j++)
  {
    free(paths[j]);
  }
  free(paths);
}
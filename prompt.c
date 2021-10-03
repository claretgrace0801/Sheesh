#include "header.h"

char username[1000];
char hostname[1000];
char path[1000];

void display_user()
{

  if (getlogin_r(username, 1000) != 0)
  {
    perror("Username Error");
    return;
  }

  if (gethostname(hostname, 1000) != 0)
  {
    perror("Hostname Error");
    return;
  }

  printf("\033[0;36m%s\033[0;37m@\033[0;33m%s\033[0;37m:\033[0;32m%s\033[0;37m -> ", username, hostname, cust_cwd(1));
}
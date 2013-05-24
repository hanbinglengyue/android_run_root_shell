#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "kallsyms.h"

bool
kallsyms_exist(void)
{
  struct stat st;

  if (stat("/proc/kallsyms", &st) < 0) {
    return false;
  }
  return st.st_mode & S_IROTH;
}

void *
kallsyms_get_symbol_address(const char *symbol_name)
{
  FILE *fp;
  char function[BUFSIZ];
  char symbol;
  void *address;
  int ret;

  fp = fopen("/proc/kallsyms", "r");
  if (!fp) {
    printf("Failed to open /proc/kallsyms due to %s.", strerror(errno));
    return 0;
  }

  while((ret = fscanf(fp, "%p %c %s", &address, &symbol, function)) != EOF) {
    if (!strcmp(function, symbol_name)) {
      fclose(fp);
      return address;
    }
  }
  fclose(fp);

  return NULL;
}


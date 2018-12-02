#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
  {
   char *w = getenv("JAVA");
   printf("Env: %s\n",w);
   return 0;
  }


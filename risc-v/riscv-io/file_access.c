#include <stdio.h>
#include <stdlib.h>

int main ()
{
  printf("Hello World\n");

  FILE *fp;
  if ((fp = fopen("sample_hello.txt", "w")) == NULL) {
    perror ("sample_hello.txt");
    exit(EXIT_FAILURE);
  }
  fprintf (fp, "Hello World\n");
  fclose (fp);

  if ((fp = fopen("sample_hello.txt", "r")) == NULL) {
    perror ("sample_hello.txt");
    exit(EXIT_FAILURE);
  }
  char cin[100];
  fscanf (fp, "%s", cin);
  printf ("%s\n", cin);

  // char c[100];
  // scanf("%s", c);
  // printf("%s\n", c);

  return 0;
}

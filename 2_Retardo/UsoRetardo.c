#include <stdio.h>

extern void retardo(unsigned long int a);

int main( int argc, char *argv[] )
{
  long unsigned int numRetardo=900000000;


  retardo(numRetardo);
  printf("Esto es");
  retardo(numRetardo);

  printf(" \n un mensaje \n");
  retardo(numRetardo);

  printf(" progresivo.\n");
  retardo(numRetardo);

  printf("Se comporta rara la primer línea.\n");
  retardo(numRetardo);

  printf("Adiós.\n");
  retardo(numRetardo);


  return 0;
}




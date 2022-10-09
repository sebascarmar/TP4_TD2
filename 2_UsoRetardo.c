#include <stdio.h>

extern void retardo(unsigned long int a);

#define NUM_RETARDO 500000

int main( int argc, char *argv[] )
{
  printf("\n Esto es");
  retardo(NUM_RETARDO);

  printf(" \n un mensaje \n");
  retardo(NUM_RETARDO);

  printf(" progresivo.\n");

  return 0;
}




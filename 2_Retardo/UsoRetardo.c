#include <stdio.h>

#define FD_STDOUT 1
extern void retardo(unsigned long int a);

int main( int argc, char *argv[] )
{
  long unsigned int numRetardo=500000000;

  retardo(numRetardo);
  dprintf(FD_STDOUT, "Esto es ");
  retardo(numRetardo);

  dprintf(FD_STDOUT, "un mensaje ");
  retardo(numRetardo);

  dprintf(FD_STDOUT, "progresivo");
  retardo(numRetardo);

  for( int i=1 ; i <= 6 ; i++ ) // Puntos suspensivos.
  {
    dprintf(FD_STDOUT, ".");
    retardo(numRetardo);
  
    if( i%3==0 )
    {
      dprintf(FD_STDOUT, "\b\b\b   \b\b\b");
      retardo(numRetardo);
    }
  }

  dprintf(FD_STDOUT, ".\nAdiós.\n");
  return 0;
}




#include <stdio.h>

extern int operaciones(int op1, int op2, int operacion);

int main(int argc, char *argv[])
{
  int a=20;
  int b=10;
  int resultado = 0;

  // Suma.
  resultado = operaciones(a, b, 0);
  printf("%d + %d  = %d \n", a, b, resultado);

  // Resta.
  resultado = operaciones(a, b, 1);
  printf("%d - %d  = %d \n", a, b, resultado);

  // And.
  resultado = operaciones(a, b, 2);
  printf("0x%x & 0x%x  = 0x%x \n", a, b, resultado);

  // Or.
  resultado = operaciones(a, b, 3);
  printf("0x%x | 0x%x  = 0x%x \n", a, b, resultado);


  return 0;
}

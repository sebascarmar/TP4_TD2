#include "EasyPIO.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define FD_STDIN 0

int main( int argc, char *argv[] )
{
  int leds[4]     = { 16, 20, 21, 26 };  // Arreglo que contiene los leds.
  int switches[4] = { 5, 6, 13, 19 };   // Arreglo que contiene los pines del dip switchx4.
  int valorLectura = 0;                 // Almacena la lectura de los switches (1 o 0).

  struct termios t_old, t_new;          // Estructuras para atributos del teclado.
  int detectaTecla;                     // Almacena lo que retorna la función read().
  char buf[1]= {'\0'};                  // Almacena lo leído por que lee read().

  /*********************Seteo del modo NO canónico NO bloqueante********************/
  tcgetattr( FD_STDIN, &t_old );        // Lee atributos del teclado.
  t_new = t_old;                        // Guarda los atributos originales.

  t_new.c_lflag &= ~(ECHO | ICANON);  // Anula entrada canónica y eco.
  t_new.c_cc[VMIN] = 0;               // No espera que ingrese ningún caracter.
  t_new.c_cc[VTIME] = 0;              // Tampoco espera ningún tiempo.

  tcsetattr( FD_STDIN,TCSANOW,&t_new ); // Setea los valores nuevos de la config.


  /**********************Mapeo y configuración de pines*****************************/
  pioInit();

  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(13, INPUT);
  pinMode(19, INPUT);

  pinMode(16, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(26, OUTPUT);

  /******************Bucle que termina cuando se aprieta una tecla*****************/
  while( detectaTecla == 0 )
  {
    for( int i = 0 ; ( detectaTecla == 0 )&&( i<4 ) ; i++ ) // Si detecta tecla, sale.
    {
      valorLectura = digitalRead( switches[i] ); // Lee el switch.
      digitalWrite( leds[i], valorLectura );     // Escribe en el led lo leído en el switch.
      detectaTecla = read( FD_STDIN, buf, 1 ); // read() retorna la cantidad de  
    }                                          //caracteres que lee.
  }
  
  for( int i = 0 ; i < 4 ; i++ ) // Apaga todos los leds antes de salir del programa.
      digitalWrite( leds[i], 0 );

  /************************Seteo del modo canónico*****************************/
  tcsetattr(FD_STDIN, TCSANOW, &t_old); // Actualiza los atributos del teclado 
                                        //con los valores previos.

  return 0;
}

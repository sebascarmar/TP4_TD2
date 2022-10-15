#include "EasyPIO.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define FD_STDIN 0

int main( int argc, char *argv[] )
{
  int leds[8] = {23, 24, 25, 12, 16, 20, 21, 26}; // Arreglo que contiene los leds.
  
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

  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(26, OUTPUT);


  /******************Bucle que termina cuando se aprieta una tecla*****************/
  while( detectaTecla == 0 )
  {
    for( int i = 0 ; ( detectaTecla == 0 )&&( i<8 ) ; i++ ) // Act./Desact. los leds
    {                                                       //en un sentido.

      digitalWrite( leds[i], 1 );
      usleep(100000);
      digitalWrite( leds[i], 0 );
      detectaTecla = read( FD_STDIN, buf, 1 ); // read() retorna la cantidad de  
    }                                          //caracteres que lee.

  
  for( int i = 6 ; ( detectaTecla == 0 )&&( i>0 ) ; i-- ) // Act./Desact. los leds
    {                                                     //en el otro sentido.

      digitalWrite( leds[i], 1 );
      usleep(100000);
      digitalWrite( leds[i], 0 );
      detectaTecla = read( FD_STDIN, buf, 1 ); // read() retorna la cantidad de  
    }                                          //caracteres que lee.
                     
  }
  

  /************************Seteo del modo canónico*****************************/
  tcsetattr(FD_STDIN, TCSANOW, &t_old); // Actualiza los atributos del teclado 
                                        //con los valores previos.


  return 0;
}

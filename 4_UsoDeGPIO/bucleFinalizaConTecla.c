#include "EasyPIO.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define FD_STDIN 0

int main( int argc, char *argv[] )
{
  int leds[4]     = { 16, 20, 21, 26 }; // Arreglo que contiene los pines de algunos leds.
  int switches[4] = { 5, 6, 13, 19 };   // Arreglo que contiene los pines del dip switchx4.
  int valorLectura = 0;                 // Almacena la lectura de los switches (1 o 0).

  struct termios t_old, t_new;          // Estructuras para atributos del teclado.
  int detectaTecla;                     // Almacena lo que retorna la función de lectura del teclado (read).
  char buf[1]= {'\0'};                  // Buffer en donde se almacena el caracter leído del teclado (read).


  /*******************************Seteo del modo NO canónico NO bloqueante***************************/
  tcgetattr(FD_STDIN, &t_old);        // Lee atributos del teclado.
  t_new = t_old;                      // Copia los atributos del modo canónico para re configurar antes de salir.

  t_new.c_lflag &= ~(ECHO | ICANON);  // Anula entrada canónica y eco.
  t_new.c_cc[VMIN] = 0;               // No espera por ningún caracter ingresado por telcado.
  t_new.c_cc[VTIME] = 10;             // Solo espera 10ds a que se ingrese algo, luego sigue.

  tcsetattr(FD_STDIN,TCSANOW,&t_new); // Actualiza con los valores nuevos de la config (TCSANOW = activa la 
                                      //modificación inmediatamente).


  /*************************Mapeo y configuración de pines*********************************************/
  pioInit();

  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(13, INPUT);
  pinMode(19, INPUT);

  pinMode(16, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(26, OUTPUT);


 /***********************Bucle que termina cuando se aprieta una tecla********************************/
  while( detectaTecla == 0 )
  {
    for( int i = 0 ; ( detectaTecla == 0 )&&( i<4 ) ; i++ ) 
    {
      valorLectura = digitalRead( switches[i] ); // Lee el switch.
      digitalWrite( leds[i], valorLectura );     // Escribe en el led lo leído en el switch.
      detectaTecla = read( FD_STDIN, buf, 1 );  // read se ejecuta hasta que pasen TIME [dseg] o que se ingrese un caracter. Si no 
    }                                           //se ingresa nada, retorna 0 (ver >man termios).
                                                //Prototipo: ssize_t read(int fd, void *buf, size_t count);
  }
  

  /*********************************Seteo del modo canónico*******************************************/
  tcsetattr(FD_STDIN, TCSANOW, &t_old); // Actualiza los atributos del teclado con los valores previos.


  return 0;
}

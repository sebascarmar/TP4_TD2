#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>


#define FD_STDIN 0

int main( int argc, char *argv[] )
{  
  struct termios t_old, t_oldUART, t_new, t_newUART; // Estructuras para atributos del teclado.
  int fdUART;                           // Descriptor de archivo de la UART. 
  char buf[1]= {'\0'};                  // Almacena el caracter leído por read().


	// APERTURA DEL DISPOSITIVO COMO ARCHIVO.
  fdUART = open( "/dev/ttyUSB0",  O_RDWR | O_NOCTTY | O_NDELAY ); //O_DSYNC
  if( fdUART == -1 )
  {
    fprintf( stderr, "ERROR: abriendo el archivo %s. \n" , argv[1] );
    return -1;
  }
  /**********Seteo del modo NO canónico NO bloqueante en la ENTRADA ESTANDAR*********/
  tcgetattr( FD_STDIN, &t_old );        // Lee atributos del teclado.
  t_new = t_old;                        // Guarda los atributos originales.

  t_new.c_lflag &= ~(ECHO | ICANON);  // Anula entrada canónica y eco.
        t_new.c_cc[VMIN] = 0;               // No espera que ingrese ningún caracter.
  t_new.c_cc[VTIME] = 0;              // Tampoco espera ningún tiempo.

  tcsetattr( FD_STDIN,TCSANOW,&t_new ); // Setea los valores nuevos de la config.



  /************Seteo del modo NO canónico NO bloqueante en el PUERTO SERIAL**********/
  tcgetattr( fdUART, &t_oldUART );        // Lee atributos de la UART.
  t_newUART = t_oldUART;                  // Guarda los atributos originales.
	
  //A PARTIR DE AQUÍ SE COMIENZAN A CONFIGURAR LOS NUEVOS ATRIBUTOS
  cfsetospeed( &t_newUART, 9600 ); // Velocidad de comunicación de salida.
  cfsetispeed( &t_newUART, 9600 ); // Velocidad de comunicación de entrada.
  
  t_newUART.c_cflag = ( t_newUART.c_cflag & ~CSIZE ) | CS8; // 8 bits de datos (8)
  t_newUART.c_cflag &= ~( PARENB | PARODD );	            // sin paridad     (N)
  t_newUART.c_cflag &= ~CSTOPB;			            // 1 bit de stop   (1)
  t_newUART.c_iflag |= IGNCR;  // Ignora el caracter '\r' en la entrada.
	
  t_newUART.c_lflag &= ~(ECHO | ICANON);  // Anula entrada canónica y eco.
  t_newUART.c_cc[VMIN] = 0;               // No espera que ingrese ningún caracter.
  t_newUART.c_cc[VTIME] = 0;              // Tampoco espera ningún tiempo.

  if( tcsetattr( fdUART, TCSAFLUSH, &t_newUART) != 0 ) // Setea las configuraciones.
  {
    printf( "ERROR: tcsetatrr\n" ) ;
    return -1 ;
  }

  /****Bucle  de ESCRITURA en la UART (termina cuando se aprieta una tecla)****/
  printf(" --------------------------------------\n");
  printf(" - Com. serie (termina al enviar '!') -\n");
  printf(" --------------------------------------\n");
  while( buf[0] != '!'  )
  {     
    read( FD_STDIN, buf, 1 ); // read() lee 1 caracter de la ENTRADA ESTANDAR
                              //y lo almacena en buf[0].
    //dprintf(1, "%c", buf[0]); 
    write( 1, buf, 1  ); // Imprime el caracter ingresado, simula entrada canónica
   
    write( fdUART, buf, 1  ); // Escribe 1 byte del contenido de cadena al "fdUART".
    tcdrain( fdUART ); // Espera a que lo que se haya escrito en "fdUART" se transmita.
    tcflush(fdUART, TCIOFLUSH);
    
    if(buf[0] != '!') // Evita que se imprima el mismo caracter indefinidamente.
        buf[0]='\0';  
  }

  /***LECTURA de la cadena en la UART e impresión en pantalla de lo recibido***/
  write(1,"\n", 1);
  while( buf[0] != '\0' )
  {
    read( fdUART, buf, 1 ); // read() lee 1 caracter del PUERTO SERIAL y lo
                            //almacena en buf[0].
    if(buf[0] != '!')
      //dprintf(1, "%c", buf[0]);
      write( 1, buf, 1  ); // Imprime 1 byte del contenido de la cadena.
    if(buf[0] != '\0') // Evita que se imprima el mismo caracter indefinidamente.
      buf[0]='!';  
  }
  printf("\n");


  /************************Seteo del modo canónico*****************************/
  tcsetattr(FD_STDIN, TCSANOW, &t_old); // Actualiza los atributos del teclado 
                                        //con los valores previos.
  tcsetattr(fdUART, TCSANOW, &t_oldUART); // Actualiza los atributos del teclado 
                                        //con los valores previos.
	
  if( close(fdUART)==-1 )
    printf("Error al cerrar el puerto serial\n");

  return 0;
}

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main( int argc, char *argv[] )
{
  int puertoSerial ;    // Almacena el descriptor de archivos del puerto serie.
  char datoRecibido;    // Almacena lo recibido por el puerte serie (byte a byte),
  char avisaFin[] = "\nComunicacion finalizada\n";


/***********************Apertura del puerto serie************************************/
  puertoSerial = serialOpen ("/dev/ttyS0", 9600);
  if( puertoSerial < 0 )
  {
    fprintf (stderr, "Error al abrir \\dev\\ttyS0: %s\n", strerror (errno)) ;
    return 1 ;
  }


/****************************Mapeo de pines****************************************/
  if ( wiringPiSetup() == -1 ) // Inicializa los pines siguiendo el esquema de WiringPi.
  {
    fprintf (stdout, "Error al inicializar wirinfPi: %s\n", strerror (errno)) ;
    return 1 ;
  }


/**************Inicia lectura y escritura mediante el puerto serie******************/
  printf (" ------Comunicación serie (finaliza con caracter '!')------\n") ;
  while( datoRecibido != '!' )
  {
    if( serialDataAvail(puertoSerial) )             // Retorna el número de caracteres
    {                                               //disponible para leer.
      datoRecibido = serialGetchar( puertoSerial ); // Retorna el siguiente caracter 
                                                    //disponible en el dispositivo serial.	
      printf ("%c", datoRecibido);                  // Imprime en pantalla el carcater.
      if( datoRecibido != '!' )
        serialPutchar( puertoSerial, datoRecibido );// Envía el caracter leído por
                                                    //el puerto serie.
    }
  }


/************Envía cadena de aviso de finalización por puerto serie*****************/
  for( int i = 0 ; avisaFin[i] != '\0' ; i++ )
  {
      serialPutchar( puertoSerial, avisaFin[i] );
  }
  printf("\n");


/*************************Cierre del puerto serie************************************/
  serialClose( puertoSerial );

  return 0;
}

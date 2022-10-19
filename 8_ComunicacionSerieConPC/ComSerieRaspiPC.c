#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <string.h>

int main( int argc, char *argv[] )
{
  int puertoSerial ;    // Almacena el descriptor de archivos del puerto serie.
  char datoRecibido;    // Almacena lo recibido por el puerte serie (byte a byte),
  char avisaFin[] = "Comunicacion Finalizada";


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
  printf(" --------------------------------------\n");
  printf(" -Comunicación serie (termina con '!')-\n");
  printf(" --------------------------------------\n");
  while( datoRecibido != '!' )
  {
    if( serialDataAvail(puertoSerial) )             // Retorna el número de caracteres
    {                                               //disponible para leer.
      datoRecibido = serialGetchar( puertoSerial ); // Retorna el siguiente caracter 
                                                    //disponible en el dispositivo serial.	
      printf ("%c", datoRecibido);                  // Imprime en pantalla el carcater.
                                                    //el puerto serie.
    }
  }


/************Envía cadena de aviso de finalización por puerto serie*****************/
  for( int i = 0 ; i <= strlen(avisaFin) ; i++ )
  {
      serialPutchar( puertoSerial, avisaFin[i] );
  }
  printf("\n");


/*************************Cierre del puerto serie************************************/
  serialClose( puertoSerial );

  return 0;
}

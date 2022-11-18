#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
  int serial_port ;
  int a = 10;
  char dat;
  
  if ((serial_port = serialOpen ("/dev/serial0", 9600)) < 0) //abre el serial port y setea la velocidad
                                                             //de transmision
  {
    fprintf (stderr, "ERRO. No pudo abrir el puerto serie: %s\n", strerror (errno)) ;
    return 1 ;
  }

  if (wiringPiSetup () == -1) //inicia el wiringPi 
  {
    fprintf (stdout, "No pudo iniciar wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }

  /*****************lectura de los datos enviados************************/ 
  printf("Recibiendo datos\n");
  while(1){

    if(serialDataAvail(serial_port)) {
      dat = serialGetchar(serial_port);   //Lee el caracter que llega
      printf ("%c", dat);                //imprime en pantalla el caracter

        }

  }
}

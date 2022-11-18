//Compilar: gcc -Wall send.c termset.c -o send */
//Ejecutar: ./send */  

# include <stdio.h>  
# include <fcntl.h>  
# include <unistd.h> 
# include "termset.h"


int main ( void )
{
	int fd; 
	struct termios oldtty , newtty; 
	char dato;
	 
	/***********************Apertura del puerto serie************************************/

	/*0_RDWR   --> Se abre en modo escritura y lectura*/
	/*0_NOCTTY --> La terminal no se convertira en la terminal controladora del proceso*/
	/*0_NDELAY --> El puerto serie se abre en modo no bloqueante*/
	
	fd = open ("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY );
	if ( fd == -1)
	{
		printf ("ERROR : no se pudo abrir el dispositivo.\n");
		return -1;
	}
	
	/***********************Configuracion del TTY**************************************/
	
	if (termset(fd , 9600, &oldtty, &newtty) == -1) 
	{
		printf (" ERROR : no se pudo configurar el TTY \n" );
		return -1;
	}
	
	//tcflush()-->descarta datos escritos al objeto referido por fd pero no transmitidos
	tcflush(fd, TCIOFLUSH);
	 
	printf("\nA continuación ingresar el dato a enviar:");
	printf("\nPara terminar la ejecución presionar 'ctrl+c'.\n");
	  
	while(1)
	{
		scanf("%c",&dato);
		write(fd , &dato , 1);
		tcdrain(fd);
		write(fd , "\n" , 1);
		tcdrain(fd);
		//tcdrain --> 	espera hasta que se haya transmitido toda la salida escrita al
		//		objeto referido por fd.
	  
		sleep (1);
	}
	  
	close (fd);
	return 0;
}

#include <stdio.h>
#include "termset.h"


int termset ( int fd, int baudrate, struct termios ttyold, struct termios ttynew )
{
switch ( baudrate )// establece la velocidad de transmision
{
	case 115200: baudrate = B115200;
	break ;
	case 57600: baudrate = B57600;
	break ;
	case 38400: baudrate = B38400;
	break ;
	case 19200: baudrate = B19200;
	break ;
	case 9600: baudrate = B9600;
	break ;
	default : baudrate = B115200;
	break ;
}	

if(tcgetattr(fd, ttyold) != 0) //obtiene los parámetros asociados con el objeto referido
                              //por fd y los almacena en la estructura termios
{
  printf(" ERROR : tcgetattr \n");
  return -1;
}


/*****************setea los parametros de transmision************************/

ttynew = ttyold;
cfsetospeed(ttynew, baudrate); //establece la tasa de baudios de salida almacenada en la estructura termios
cfsetispeed(ttynew, baudrate); //establece la tasa de baudios de entrada almacenada en la estructura termio
ttynew -> c_cflag = ( ttynew -> c_cflag & ~CSIZE ) | CS8 ; // establece 8 bits de datos
ttynew -> c_cflag &= ~( PARENB | PARODD ); // establece la no paridad
ttynew -> c_cflag &= ~CSTOPB ;// establece 1 bit de stop

ttynew -> c_cflag |= (CLOCAL | CREAD);         // ignorar las líneas de estado del módem y habilita la lectura
ttynew -> c_cflag &= ~CRTSCTS;                 // no hay control de flujo

ttynew -> c_iflag &= ~IGNBRK ;                  // disable break processing
ttynew -> c_iflag &= ~( IXON | IXOFF | IXANY ); // shut off xon / xoff ctrl

ttynew -> c_lflag = 0;
ttynew -> c_oflag = 0;
ttynew -> c_cc[VMIN] = 0;
ttynew -> c_cc[VTIME] = 100;


if(tcsetattr(fd, TCSANOW, ttynew) != 0)

{
	printf(" ERROR : tcsetattr \n" );
	return -1;
}
	return 0;
}

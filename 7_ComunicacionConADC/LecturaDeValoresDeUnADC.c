#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

/*********Descriptor de entrada estándar y direcciones de registros*****************/
#define FD_STDIN 0
#define ADDRESS 0x48
#define A0 ADDRESS+0
#define A1 ADDRESS+1
#define A2 ADDRESS+2
#define A3 ADDRESS+3


int main( int argc, char *argv[] )   
{
  struct termios t_old, t_new;  // Estructuras para atributos del teclado.
  int detectaTecla = 0;         // Almacena lo que retorna la función read().
  char buf[1] = {'\0'};         // Almacena lo leído por que lee read().
  
  int fd, valorADC;	        // Declaracion de variables para acceder al ADC.

  
/*************Seteo del modo NO canónico y  bloqueante por cierto tiempo************/
  tcgetattr( FD_STDIN, &t_old );        // Lee atributos del teclado.
  t_new = t_old;                        // Guarda los atributos originales.

  t_new.c_lflag &= ~(ECHO | ICANON);  // Anula entrada canónica y eco.
  t_new.c_cc[VMIN] = 0;               // No espera que ingrese ningún caracter.
  t_new.c_cc[VTIME] = 5;              // Espera 500ms por si ingresa un caracter.

  tcsetattr( FD_STDIN,TCSANOW,&t_new ); // Setea los valores nuevos de la config.


/****************************Mapeo de pines****************************************/
  if ( wiringPiSetup() == -1 ) // Inicializa los pines siguiendo el esquema de WiringPi.
  {
    printf("Error al mapear el GPIO.\n");
    exit(EXIT_FAILURE);	
  }
  
  fd = wiringPiI2CSetup(ADDRESS); // Inicializa el sistema I2C con el ID del dispos.
  if( fd <= -1 )
  {
    printf("Error al inicializar el systema I2C.\n");
    exit(EXIT_FAILURE);	
  }


/****************Lectura de los valores del ADC mediante I2C************************/
  while( detectaTecla == 0 )
  {
    // Lectura del canal ADC0 (potenciómetro).
    wiringPiI2CReadReg8(fd, A0); // Activa la conversión.
    valorADC = wiringPiI2CReadReg8(fd, A0);// Lectura del valor.
    printf("Voltaje en el potenciómetro = %.3f V\n", (float) valorADC*3.3/255);
    
    // Lectura del canal ADC1 (fotoresistor).
    wiringPiI2CReadReg8(fd, A1); // Activa la conversión.
    valorADC = wiringPiI2CReadReg8(fd, A1); // Lectura del valor.
    printf("Voltaje en el fotoresistor  = %.3f V\n", (float) valorADC*3.3/255);
    
    // Lectura del canal ADC2 (termistor).
    wiringPiI2CReadReg8(fd, A2); // Activa la conversión.
    valorADC = wiringPiI2CReadReg8(fd, A2); // Lectura del valor.
    printf("Voltaje en el termistor     = %.3f V\n", (float) valorADC*3.3/255);
    		  
    // Lectura del canal ADC3 (puesto a GND).
    wiringPiI2CReadReg8(fd, A3); // Activa la conversión.
    valorADC = wiringPiI2CReadReg8(fd, A3); // Lectura del valor.
    printf("Voltaje en el cuarto canal  = %.3f V\n\n", (float) valorADC*3.3/255);
    
    // La función read() espera por 500ms. Esto sirve para que la que se
    //imprime en pantalla se pueda leer de forma prolija. Además, permite
    //salir del bucle.
    detectaTecla = read( FD_STDIN, buf, 1 ); // read() retorna la cantidad de  
                                             //caracteres que lee.
  }		  
  

/************************Seteo del modo canónico*****************************/
  tcsetattr(FD_STDIN, TCSANOW, &t_old); // Actualiza los atributos del teclado 
                                        //con los valores previos.
			  
  return 0;
}

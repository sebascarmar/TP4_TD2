#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

/***********************Direcciones de registros***********************************/
#define ADDRESS 0x48
#define A0 ADDRESS+0
#define A1 ADDRESS+1
#define A2 ADDRESS+2
#define A3 ADDRESS+3


int main( int argc, char *argv[] )   
{
  int fd, valorADC;	// Declaracion de variables para acceder al ADC.
  
/****************************Mapeo de pines****************************************/
  if ( wiringPiSetup() == -1 ) // Inicializa los pines siguiendo el esquema de WiringPi.
  {
    printf("Error al mapear el GPIO.\n");
    exit(EXIT_FAILURE);	
  }
  
  fd = wiringPiI2CSetup(ADDRESS); // Inicializa el sistema I2C con el ID 
                                 //del dispositivo.
  if( fd <= -1 )
  {
    printf("Error al inicializar el systema I2C.\n");
    exit(EXIT_FAILURE);	
  }

/****************Lectura de los valores del ADC mediante I2C************************/
  while( 1 )
  {
    // Lectura del canal ADC0 (potenciómetro).
    wiringPiI2CReadReg8(fd, A0); // Indica el registro a leer.
    valorADC = wiringPiI2CReadReg8(fd, A0);// Lectura del valor.
    printf("Voltaje en el potenciómetro = %.3f V\n", (float) valorADC*3.3/255);
    
    // Lectura del canal ADC1 (fotoresistor).
    wiringPiI2CReadReg8(fd, A1); // Indica el registro a leer.
    valorADC = wiringPiI2CReadReg8(fd, A1); // Lectura del valor.
    printf("Voltaje en el fotoresistor  = %.3f V\n", (float) valorADC*3.3/255);
    
    // Lectura del canal ADC2 (termistor).
    wiringPiI2CReadReg8(fd, A2); // Indica el registro a leer.
    valorADC = wiringPiI2CReadReg8(fd, A2); // Lectura del valor.
    printf("Voltaje en el termistor     = %.3f V\n", (float) valorADC*3.3/255);
    		  
    // Lectura del canal ADC3 (puesto a GND).
    wiringPiI2CReadReg8(fd, A3); // Indica el registro a leer.
    valorADC = wiringPiI2CReadReg8(fd, A3); // Lectura del valor.
    printf("Voltaje en el cuarto canal  = %.3f V\n\n", (float) valorADC*3.3/255);
    
    delay(500);
  }		  
			  
  return 0;
}

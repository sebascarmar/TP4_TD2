#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>


/*************************ANCHO PARA EL INFORME************************************/


#define ADDRESS 0x48
#define A0 ADDRESS+0
#define A1 ADDRESS+1
#define A2 ADDRESS+2
#define A3 ADDRESS+3
#define A4 ADDRESS+4


int main()   
{
	//const char PCF8591 = 0x48; // direccion base
	
	int fd, adcVal;	//declaracion de variables para acceder al adc

	if (wiringPiSetup () == -1) exit (1);	//

	fd = wiringPiI2CSetup(ADDRESS);	
	// configuracion del bus I2C, inicializa el sistema I2C con el ID del dispositivo.

	while (1)   {
	//read ADC0
	wiringPiI2CReadReg8(fd, A4); //primer lectura 
	adcVal = wiringPiI2CReadReg8(fd, A4);//segunda lectura valida
	printf("Pot value = %d \n", adcVal);
	
	//read ADC1
	wiringPiI2CReadReg8(fd, A1); //primer lectura
	adcVal = wiringPiI2CReadReg8(fd, A1);
	printf("Photocell = %d \n", adcVal);
	
	//read ADC2
	wiringPiI2CReadReg8(fd, A2); //primer lectura
	adcVal = wiringPiI2CReadReg8(fd, A2);
	printf("Termister = %d \n\n", adcVal);
			  
	delay(500);
			  
     }
			  
return 0;
			  
}

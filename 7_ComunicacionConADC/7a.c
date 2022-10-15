#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

const char PCF8591 = 0x48; // direccion base

int fd, adcVal;	//declaracion de variables para acceder al adc

int main()   
{

	if (wiringPiSetup () == -1) exit (1);	//

	fd = wiringPiI2CSetup(PCF8591);		// configuracion del bus I2C, inicializa el sistema I2C con el ID del dispositivo.

	while (1)   {
	//read ADC0
	wiringPiI2CReadReg8(fd, PCF8591 + 3) ; //primer lectura 
	adcVal = wiringPiI2CReadReg8(fd, PCF8591 + 3) ;//segunda lectura valida
	printf("Pot value = %d \n", adcVal);
	//read ADC1
	wiringPiI2CReadReg8(fd, PCF8591 + 4) ; 
	adcVal = wiringPiI2CReadReg8(fd, PCF8591 + 4) ;
	printf("Photocell = %d \n", adcVal);
	//read ADC2
	wiringPiI2CReadReg8(fd, PCF8591 + 2) ; 
	adcVal = wiringPiI2CReadReg8(fd, PCF8591 + 2) ;
	printf("Termister = %d \n\n", adcVal);
			  
	delay(500);
			  
     }
			  
return 0;
			  
}

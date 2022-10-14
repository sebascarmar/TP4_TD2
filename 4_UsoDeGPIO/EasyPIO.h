#ifndef EASY_PIO_H
#define EASY_PIO_H

/*##################################################################################################*/
/*##                                 Inclusión de headers                                         ##*/
/*##################################################################################################*/
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>   // Posee las funciones para manipular archvivos open()/close().
#include <stdlib.h>   // Posee la función exit().

/*##################################################################################################*/
/*##                                    Constantes                                                ##*/
/*##################################################################################################*/

// GPIO FSEL types (6 registros, 3 por bit).
#define INPUT  0
#define OUTPUT 1
#define ALT0   4
#define ALT1   5
#define ALT2   6
#define ALT3   7
#define ALT4   3
#define ALT5   2

/*##################################################################################################*/
/*##                           Mapeo de memoria                                                   ##*/
/*##################################################################################################*/
#define BCM2835_PERI_BASE   0x3F000000 // Dirección base de todos los periféricos (en el libro figura 0x20000000)
#define GPIO_BASE           (BCM2835_PERI_BASE + 0x200000) // Dir. base del GPIO.
#define BLOCK_SIZE          (4*1024)

// Punteros  que serán mapeados en memeoria cuando se llama a pioInit().
volatile unsigned int       *gpio; // Puntero a la base de gpio.

/*##################################################################################################*/
/*##                                   Registros GPIO                                             ##*/
/*##################################################################################################*/
#define GPFSEL              ((volatile unsigned int *)(gpio + 0) )
#define GPSET               ((volatile unsigned int *)(gpio + 7) )
#define GPCLR               ((volatile unsigned int *)(gpio + 10) )
#define GPLEV               ((volatile unsigned int *)(gpio + 13) )

/*##################################################################################################*/
/*##                              Funciones generales                                             ##*/
/*##################################################################################################*/
void pioInit( )
{
  int mem_fd;
  void *reg_map;

  mem_fd = open("/dev/mem", O_RDWR | O_SYNC); // /dev/mem es un pseudo_driver para acceso a memoria en Linux.
  if( mem_fd < 0)                             // Verifica si se pudo abrir el archivo. Si no, aborta. 
  {
    printf("Error al abrir /dev/mem.\n");
    exit(EXIT_FAILURE);
  }

  reg_map = mmap(
                 NULL,                        // Dirección desde la cual se inicia el mapeo local (null=no importa).
                 BLOCK_SIZE,                  // Tamaño del bloque de memoria mapeado (4kB).
                 PROT_READ | PROT_WRITE,      // Habilita tanto escritura como lectura para la memoria mapeada.
                 MAP_SHARED,                  // Acceso no exclusivo a esta memoria.
                 mem_fd,                      // Puntero a /dev/mem.
                 GPIO_BASE);                  // Offset al periférico GPIO.
  if (reg_map == MAP_FAILED)                  // Verifica si se pudo realiar el mapeo. Si no, aborta.
  {
    printf("Error al  mapear GPIO  %d\n", (int)reg_map);
    close(mem_fd);
    exit(EXIT_FAILURE);
  }

  gpio = (volatile unsigned *)reg_map;
  close(mem_fd);
}


/*##################################################################################################*/
/*##                                Funcionetes GPIO                                              ##*/
/*##################################################################################################*/

void pinMode( int pin, int function ) // Los registros GPFSEL determina la función de cada pin.
{
  int reg       = pin/10;
  int offset    = (pin%10)*3;
  GPFSEL[reg]  &= ~( (0b111 & ~ function) << offset ); // Setea los ceros.
  GPFSEL[reg]  |= ( (0b111 &  function) << offset );   // Setea los unos.
}
/*--------------------------------------------------------------------------------------------------*/ 
void digitalWrite( int pin, int val )
{
  int reg    = pin/32;
  int offset = pin%32;

  if(val)
    GPSET[reg] = 1 << offset;
  else
    GPCLR[reg] = 1 << offset;
}
/*--------------------------------------------------------------------------------------------------*/ 
int digitalRead( int pin )
{
  int reg    = pin/32;
  int offset = pin%32;

  return ( GPLEV[reg] >> offset ) & 0x00000001; // Pone en cero todos los bits que no interesan.
}

#endif

#include <sys/mman.h>

#define BCM2835_PERI_BASE   0x20000000
#define GPIO_BASE           (BCM2835_PERI_BASE + 0x200000)
volatile unsigned int       *gpio; // Puntero a la base de gpio.
#define GPLEVO              (*(volatile unsigned int *)(gpio + 13) )
#define BLOCK_SIZE          (4*1024)

void pioInit( )
{
  int mem_fd;
  void *reg_map;

  mem_fd = open("/dev/mem", O_RDWR | O_SYNC); // /dev/mem es un pseudo_driver para acceso a memoria en Linux.
  reg_map = mmap(
                 NULL,                        // Dirección desde la cual se inicia el mapeo local (null=no importa).
                 BLOCK_SIZE,                  // Tamaño del bloque de memoria mapeado (4kB).
                 PROT_READ | PROT_WRITE,      // Habilita tanto escritura como lectura para la memoria mapeada.
                 MAP_SHARED,                  // Acceso no exclusivo a esta memoria.
                 mem_fd,                      // Puntero a /dev/mem.
                 GPIO_BASE);                  // Offset al periférico GPIO.

  gpio = (volatil unsigned *)reg_map;
  close(mem_fd);
}


/*##################################################################################################*/
/*##                                GPIO Functions                                                ##*/
/*##################################################################################################*/

void pinMode( int pin, int function )
{
  int reg       = pin/10;
  int offset    = (pin%10)*3;
  GPFSEL[reg]  &= ~( (0b111 & ~ function) << offset );
  GPFSEL[reg]  |= ( (0b111 & ~ function) << offset );
}
/*--------------------------------------------------------------------------------------------------*/ 
void digitalWrite( int pin, int val )
{
  int reg    = pin/32;
  int offset = pin%32;

  if(val) GPSET[reg] = 1 << offset;
  else    GPCLR[reg] = 1 << offset;
}
/*--------------------------------------------------------------------------------------------------*/ 
int digitalRead( int pin )
{
  int reg    = pin/32;
  int offset = pin%32;

  return ( GPLEV[rev] >> offset ) & 0x00000001;
}

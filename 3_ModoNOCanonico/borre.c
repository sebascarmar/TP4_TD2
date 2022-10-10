#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define FD_STDIN 0
#define LENGTH_PSSW 5

int main( int argc, char *argv[] )
{
  struct termios t_old, t_new;                              // Estructuras para atributos del teclado.
  //char tecla;                                               // Variable que almacena cada caracter que se ingresa.
  //int i, asteriscosImpresos, contraCorrecta;     
  int a=0;
  char buf[1]= {'\0'};
  char contra[LENGTH_PSSW+1] = {'8','3','3','8','A','\0'};  // Contraseña de LENGTH_PSSW carcateres (más el caracter NULL).


  /*******************************SETEO DEL MODO NO CANÓNICO*****************************************/
  tcgetattr(FD_STDIN, &t_old);        // Lee atributos del teclado.
  t_new = t_old;
  t_new.c_lflag &= ~(ECHO | ICANON);  // Anula entrada canónica y eco.
  t_new.c_cc[VMIN] = 0;
  t_new.c_cc[VTIME] = 1;
  tcsetattr(FD_STDIN,TCSANOW,&t_new); // Actualiza con los valores nuevos de la config (TCSANOW = activa la modificación inmediatamente).
 //while(a==0){
 // printf("8\n" );
 // 
////ssize_t read(int fd, void *buf, size_t count);
 // a=read(0, buf, 1);
 // if(a!=0)
 //   printf("caca");
 //}

  while(a==0){
   for(int i=0; a==0 && i<5 ; i++){
   // printf("%c ", contra[i]); //Así no  imprime en pantalla hasta que no se termina.
    //printf("%c\n", contra[i]); //Solo imprime con el salto de línea.
    //write(1, &contra[i], 1 );  //Funciona!
    dprintf(1, "%c ", contra[i]);//También funciona!
    
    a=read(0, buf, 1);
    if(a!=0)
      printf("ca%cca", buf[0]);
   }
}
  /*********************************SETEO DEL MODO CANÓNICO*******************************************/
  tcsetattr(FD_STDIN, TCSANOW, &t_old); // Actualiza los atributos del teclado con los valores previos.

  return 0;
}

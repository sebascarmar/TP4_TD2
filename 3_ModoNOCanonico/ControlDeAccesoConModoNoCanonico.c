#include <stdio.h>
#include <termios.h>

#define FD_STDIN 0
#define LENGTH_PSSW 5

int main( int argc, char *argv[] )
{
  struct termios t_old, t_new;                              // Estructuras para atributos del teclado.
  char contra[LENGTH_PSSW+1] = {'8','3','3','8','A','\0'};  // Contraseña de LENGTH_PSSW carcateres (más el caracter NULL).
  char password[LENGTH_PSSW+1] = {'\0'};                    // Buffer para almacenar la contraseña que se ingresa.
  char tecla;                                               // Variable que almacena cada caracter que se ingresa.
  int i, asteriscosImpresos, contraCorrecta;     


  /*******************************SETEO DEL MODO NO CANÓNICO*****************************************/
  tcgetattr(FD_STDIN, &t_old);        // Lee atributos del teclado.
  t_new = t_old;
  t_new.c_lflag &= ~(ECHO | ICANON);  // Anula entrada canónica y eco.
  tcsetattr(FD_STDIN,TCSANOW,&t_new); // Actualiza con los valores nuevos de la config (TCSANOW = activa la modificación inmediatamente).

  
  /**********************INGRESO DE CONTRASEÑA  Y PROCESAMIENTO DE DATOS****************************/
  for(int numIntentos = 0 ; numIntentos < 3; numIntentos++)
  {
    tecla = 0;              // Almacena cada caracter que se ingresa.
    i = 0;                  // Índice para guardar los caracteres en el buffer 'password'.
    asteriscosImpresos = 0; // Cuenta los ast. que se imprimen, para poder borrar todos si se quiere.
    contraCorrecta = 0;     // Al recorrer 'password' y comparar con 'contra', si vale LENGTH_PSSW entonces es válida la passwd ingresada.

    printf(" Ingrese su password de 5 dígitos: ");
    while ( tecla!=10 )    // Ingresar hasta oprimir enter. Parte de la simulacion del modo canónico.
    {
      tecla = getchar();
      
      if( tecla!=10 ) // Si no se presiona ENTER es verdadero. Si se presiona ENTER, lo toma como un intento directamente.
      {
        if( tecla!=127 ) // Si NO se presiona DEL es verdadero.
        {
          if( i<=(LENGTH_PSSW-1) && i==asteriscosImpresos)// 'i' no puede ser mayor al largo de la contra, y debe coincidir con 
          {                                               //los asteriscos en pantalla.
            password[i] = tecla;
            i++;
          }                   
          
          printf("*");
          asteriscosImpresos++;
          
        }else            // Si SÍ se presiona DEL es verdadero.
        {
          if( asteriscosImpresos>0 ) // Solo borra caracteres en pantalla si fueron asteriscos impresos.
          {
            printf("\b \b"); // Imprime un backspace, sobreescribe con un espacio, y vuelve el cursor hacia atrás con otro backspace.
            asteriscosImpresos--;
            if( i>0 ) // Se puede agregar que borre solo cuando asteriscosImpresos vale 4 o menos.
              i--;       
          }
        }
      } 
    }
    
    for(int j = 0 ; j<LENGTH_PSSW ; j++) // Analiza si la contraseña ingresada es correcta.
    { 
      if( contra[j] == password[j] )
        contraCorrecta++;
    }
    
    if( contraCorrecta == 5 ){
      printf("\n Bienvenido al sistema\n");
      numIntentos = 3; // Puede ser 2, pero para mayor claridad se pone el 3.
    }else
      printf("\n Contraseña incorrecta\n");
    
  }

  /*********************************SETEO DEL MODO CANÓNICO*******************************************/
  tcsetattr(FD_STDIN, TCSANOW, &t_old); // Actualiza los atributos del teclado con los valores previos.

  return 0;
}

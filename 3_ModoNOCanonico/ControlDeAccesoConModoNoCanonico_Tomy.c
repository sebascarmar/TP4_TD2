#include<stdio.h>
#include<termios.h>

#define FD_STDIN 0

int main()
{   
       	char tec, tec_Aux;
       	char contrasena [6]= {'1','2','3','4','5','\0'};
       	
    	struct termios t_old, t_new;
	tcgetattr(FD_STDIN, &t_old); // lee atributos del teclado
	t_new = t_old;
      	t_new.c_lflag &= ~(ECHO | ICANON); // anula entrada canónica y eco
      	tcsetattr(FD_STDIN,TCSANOW,&t_new); // actualiza con los valores nuevos de la config. TCSANOW = activar la modificación inmediatamente
      	tec = 0;
      
      
      	int abortar = 0;
      	int bien = 0;
      	      	
      	while (bien == 0) 
      	{ 
      		int i = 0;
      		int aux = 0;
      		int cont_caracteres = 0;
      		
      		printf("\nContraseña:");
      		
      		while(tec != 10)				//Mientras no apriete el ENTER entra
      		{        				
         		tec = getchar();
         		
         		if(i == 0)
         			aux=0;
         		
         		if(tec != 127 && tec != 10)		//Si no apreto DEL o ENTER escribe el asterisco
         		{
         			printf("*");
         			if(tec == contrasena[i]) 	//Algoritmo para comparar con la contra correcta, compara 
         			{				//cada vez que se ingresa un caracter que no sea DEL y ENTER
         				aux++;
         				tec_Aux = tec;		//Si es correcto el caracter ingresado, se guarda en una
        							//variable aux
         			}
         				
         			cont_caracteres++;		//cuenta los caracteres
         		}
         		
         		if(tec == 127 && cont_caracteres == 0)	//Si quiere borrar cuando no hay ningun caracter escrito
         			i--;				//se decrementa el iterador, porque cada vez que se aprieta
         							//una tecla, este se incrementa
         		
         		if(tec == 127 && cont_caracteres > 0)	//Si aprieta DEL y hay ningun asterisco escrito
         		{					//se borra el asterisco 
         			printf("\b \b");
         			
         			if(tec_Aux == contrasena[i-1])	//Si el ultimo caracter ingresado era correcto pero se borro
         				aux--;			//Se decrementa aux
         			
         			i = i-2;			//Deja el iterador que simula el cursor donde estaba
         			
         			cont_caracteres--;
         		}
         		        		
         		i++;		    			
      		}
           	
           		
      		if(aux == 5) 			
      		{						
      			printf("\nContraseña correcta!\n");
      			//printf("\nAux: %i\n",aux);
      			//printf("\ni: %i\n",i);
      			bien = 1;
      		}    
      		else
      		{	
      			printf("\nContraseña incorrecta\n");
      			//printf("\nAux: %i\n",aux);
      			//printf("\ni: %i\n",i);
      			abortar++;
      		} 
      		
      		if(abortar == 3) 				//Si ingreso 3 veces mal la contraseña sale
      			bien =1;
      			
      		tec = 0;
      					
      	}
    	
	tcsetattr(FD_STDIN, TCSANOW, &t_old); 			// actualiza con los valores previos 
 

	return 0;
}


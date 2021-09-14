#include <wiringPi.h>
#include <stdio.h>
#include <math.h>

/*
 * Este programa prende y apaga una secuencia de 4 LEDs de la siguiente manera:
 * Secuencia A: Ascendente exclusiva: 0000 -> 0001 -> 0010 -> 0100 -> 1000
 * Secuencia B: Descendente exclusiva: 0000 -> 1000 -> 0100 -> 0010 -> 0001
 * Secuencia C: Ascendente aditiva: 0000 -> 0001 -> 0011 -> 0011 -> 1111
 * Secuencia D: Descendente aditiva: 1111 -> 0111 -> 0011 -> 0001 -> 0000
 * 
 * La secuencia que se ejecuta se repita cada segundo. Es decir, cada estado dura 0.2 segundos. 
 * Hay cambios de secuencia cada 5 segundo. 
 * Por los primeros 90 segundos, las secuencias se ejecutan en este orden: A -> C -> B -> D (Es decir, este orden de  secuencias se repite  18 veces)
 * Por los siguientes 30 segundos, las secuencias se ejecutan en este orden: D -> B -> C -> A (Es decir, este orden de secuencias se repite 6 veces)
 */

/*
 * Estos son las funciones utilizadas. 
 */ 
void actualizar_conjunto_leds(short int estado);
void prender_led(short int pin,short int valor);
void actualizar_secuencia();
void actualizar_estado_leds();
void actualizar_secuencia_de_secuencias(int tipo);
void actualizar_tipo_secuencias();
void prender_secuencia_leds_completa();
void imprimir_nombre_secuencias();

void inicializacion_leds_secuencias();

void enviar_datos_mqtt();

// Se especifican constantes para los pines en donde se encuentran los leds y el orden de las secuencias
const unsigned short int leds[] = {26,27,28,29}, secuencia_A[] = {0,2,1,3};

// Se definen constantes para los diferentes tiempos de actualizacion, para los leds, secuencias y orden de secuencias
const unsigned int actualizacion_leds = 200, actualizacion_secuencias = 5000, tiempo_cambio_sec_A = 30000, tiempo_cambio_sec_B = 90000;

// Se definen constantes para los nombres de  las secuencias utilizadas
const char* nombres_secuencias[] = {"Secuencia exclusiva ascendente","Secuencia exclusiva descendente","Secuencia aditiva ascendente","Secuencia aditiva descendente"};

// Se definen variables para actualizar leds, seceuncias y orden de secuencias
unsigned long int first_time = 0,last_time = 0,t_now = 0, last_time_secuencia = 0, last_time_tipo = 0;

// Se definen variables asociadas al estado de los leds
short int estado_leds = 0,valor_leds = 0;

// Se definen variables asociadas al estado de las secuencias
short int secuencia = 0, num_secuencia = 0, i_secuencia = 0, secuencia_tipo = 0;

/*
 * Esta es la funcion principal. Primer manda a llamar a la funcion que inicializa los pines de los leds
 * para luego ejecutar las secuencias en el orden que corresponda. 
 */ 
int main(void)
{
	// Inicializa los pines de los leds y constantes de tiempo para la ejecucion del programa
	inicializacion_leds_secuencias();
	imprimir_nombre_secuencias();
	
	while(1)
	{
		t_now = millis();
		// Actualiza los leds segun la secuencia correspondite y tambien actualiza la secuencia a utilizar
		prender_secuencia_leds_completa();
	}
	
	return 0;
}
/*
 * Esta funcion inicializa la libreria WiringPi, los pines en donde se encuentran los leds, tiempos y secuencias
 */ 
void inicializacion_leds_secuencias()
{
	// Se inicializa la librearia que permite manejar el hardware de la Raspberry
	wiringPiSetup();
	//printf("int(sizeof(leds)/2): %d\n",int(sizeof(leds)/sizeof(short int)));
	
	// Se inicializan los pines en donde se encuentran los leds
	for(int i = 0; i < int(sizeof(leds)/sizeof(short int)); i++)
	{
		pinMode(leds[i],OUTPUT);
		digitalWrite(leds[i],LOW);
	}
	// Se inicializan variables para los tiempos de actualizacion de leds, secuencias y orden de secuencias
	first_time = millis();
	last_time = first_time;
	last_time_secuencia = first_time;
	
	// Se inicializa la primera secuencia a utilizar
	secuencia_tipo = 0;
	secuencia = secuencia_A[0];
	
	return;
}

/*
 * Esta funcion imprime el nombre de la secuencia que se ejecuta en consola
 */ 
void imprimir_nombre_secuencias()
{
	printf("Num secuencia: %d , ",secuencia);
	printf(nombres_secuencias[i_secuencia]);
	printf("\n");
	
	return;
}

/*
 * Esta funcion actualiza la secuencia a ejecutar
 */
void prender_secuencia_leds_completa()
{
	// Se revisa si ya es tiempo de actualizacion de leds
	if((t_now - last_time) >= actualizacion_leds)
	{
		//printf("millis: %d\n",millis());
		//printf("Actualizacion leds: millis: %d\n",millis());
		//printf("(t_now - last_time): %d . ",int(t_now - last_time));
		
		// Se actualiza la variable de tiempo de actualizacion de leds
		last_time = t_now;

		// Obtiene la configuracion de leds prendidos y apagados segun la secuencia que se ejecutar
		actualizar_secuencia();
		
		// Prende o apaga los leds segun su estado actual segun la secuencia a utilizar
		actualizar_conjunto_leds(valor_leds);

		// Actualiza variables para la siguiente vez que se actualicen los leds
		actualizar_estado_leds();
	}
	
	// Se revisa si ya es tiempo de actualizacion de secuencia
	// Se revisa 150ms despues para no interferir en la actualizacion de los leds
	if((t_now - last_time_secuencia) >= (actualizacion_secuencias + 150))
	{
		printf("Actualizacion de secuencia. millis: %d\n",millis());
		
		// Se actualiza la variable de tiempo de actualizacion de secuencia
		last_time_secuencia = t_now-150;
		
		// Actualiza el orden de secuencias que se utilizara en la siguiente actualizacion de leds
		actualizar_tipo_secuencias();
		
		// Actualiza la secuencia que se utilizara en la siguiente actualizacion de leds
		actualizar_secuencia_de_secuencias(secuencia_tipo);
	}
	
	return;
}

/*
 * Actualiza el orden de secuencias a utilizar
 */ 
void actualizar_tipo_secuencias()
{
	// Revisa si pasaron 30 segundos desde la utilma actualizacion de orden de secuencias inversa
	if((t_now - last_time_tipo) >= tiempo_cambio_sec_B && secuencia_tipo == 1)
	{
		printf("Actualizacion secuencia A\n");
		
		// Se actualiza la variable de tiempo de actualizacion de orden de secuencias
		last_time_tipo = t_now-150;
		
		// Se actualiza la variable de orden de secuencias
		secuencia_tipo = 0;
	}
	// Revisa si pasar 90 segundos desde la utilma actualizacion de orden de secuencias sin invertir
	else if((t_now - last_time_tipo) >= tiempo_cambio_sec_A && secuencia_tipo == 0)
	{
		printf("Actualizacion secuencia A inversa\n");
		
		// Se actualiza la variable de tiempo de actualizacion de orden de secuencias
		last_time_tipo = t_now-150;
		
		// Se actualiza la variable de orden de secuencias
		secuencia_tipo = 1;
	}
	
	return;
}

/*
 * Esta funcion actualiza la secuencia a utilizar
 */
void actualizar_secuencia_de_secuencias(int tipo)
{
	// Segun el orden de secuencias que se utilice, actualiza la secuencia a utilizar
	switch(tipo)
	{
		case 0:
			// Se revisa si ya llego a la 4ta secuencia segun el orden de secuencias a utilizar
			// Si ya llego, reinicia a cero, de lo contrario sigue con la siguiente seceuncia
			if(i_secuencia < (int(sizeof(secuencia_A)/sizeof(short int))-1))
			{
				i_secuencia++;
			}
			else
			{
				i_secuencia = 0;
			}
		break;
		case 1:
			// Se revisa si ya llego a la primera segun el orden de secuencias a utilizar
			// Si ya llego, reinicia a la ultima secuencia, de lo contrario sigue con la siguiente seceuncia
			if(i_secuencia > 0)
			{
				i_secuencia--;
			}
			else
			{
				i_secuencia = (int(sizeof(secuencia_A)/sizeof(short int))-1);
			}
		break;
		default:
			printf("Error en tipo de secuencia\n");
	}
	
	// Se actualiza la secuencia a utilizar
	secuencia = secuencia_A[i_secuencia];
	
	// Imprime el nombre de la secuencia a utilizar
	imprimir_nombre_secuencias();
	
	return;
}

/*
 * Actualiza el estado de los leds, de 0 a 4, correspondiente a alguno de los estados de cada secuencia utilizada
 */
void actualizar_estado_leds()
{
	printf("estado_leds = %d\n",estado_leds);
	
	// Si todavia no ha llegado a 4, se incrementa, de lo contrario se actualiza a 0(inicia nuevamente la secuencia utilizada)
	if(estado_leds < 4)
	{
		estado_leds++;
	}
	else
	{
		estado_leds = 0;
	}
	
	return;
}

/*
 * Obtiene el valor que se utilizara para actualizar los leds
 */
void actualizar_secuencia()
{
	/*
	 * Segun la secuencia, se actualiza el valor de actualizacion de los leds
	 */ 
	switch(secuencia)
	{
		case 0:
			//printf("Secuencia exclusiva ascendente \n");
			valor_leds = 1 << (estado_leds-1);
			//printf("Valor de leds: 1 << %d = ",(estado_leds-1));
			//printf("%d\n",valor_leds);
			
		break;
		case 1:
			//printf("Secuencia exclusiva descendente\n");
			valor_leds = 16 >> estado_leds;
			//printf("Valor de leds: 1 >> %d = ",estado_leds);
			//printf("%d\n",valor_leds);
			
		break;
		case 2:
			//printf("Secuencia aditiva ascendente\n");
			valor_leds = pow(2,estado_leds)-1;
			//printf("Valor de leds: pow(2,%d)-1 = ",estado_leds);
			//printf("%d\n",valor_leds);
			
		break;
		case 3:
			//printf("Secuencia aditiva descendente\n");
			valor_leds = 16 - (pow(2,4-estado_leds));
			//printf("Valor de leds: 16 - pow(2,4-%d) = ",estado_leds);
			//printf("%d\n",valor_leds);
			
		break;
		default:
			printf("Error! Secuencia: %d no valida\n",secuencia);
	}
	
	return;
}

/*
 * Segun el valor de actualizacion de los leds, se prende o se agagan los leds
 */
void actualizar_conjunto_leds(short int estado)
{
	int respaldo_estado = 0;
	
	respaldo_estado = estado;
	
	// Se revisa el valor de cada bit del valor de actualizacion de los leds. 
	// Con base en el bit correspondiente, se prende o se apaga el led correspondiente
	for(int i = 0; i < int(sizeof(leds)/sizeof(short int)); i++)
	{
		estado = estado >> i;
		prender_led(leds[i],(estado & 1));
		//printf("Se encendio el LED %d con el valor %d\n",i,(estado & 1));
		estado = respaldo_estado;
	}
	
	//printf("\n");
	
	return;
}

/*
 * Prende y apaga un led, segun un pin y el valor de actualizacion correspondiente al led
 */
void prender_led(short int pin,short int valor)
{
	switch(valor)
	{
		case 0:
			digitalWrite(pin,LOW);
		break;
		case 1:
			digitalWrite(pin,HIGH);
		break;
		default:
			printf("Error, valor no valido\n");
	}
	
	return;
}


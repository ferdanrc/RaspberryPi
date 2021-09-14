# Raspberry

 Repositorio para los programas en los que se utiliza la RaspberryPi en el curso.
 
 ----------------------------------------------------------------------------------------------------

### Requisitos
 - Contar con una RaspberryPi
 - Tener instalado el sistema operativo RaspberryPi OS
 - Instalar el compilador gcc para la Raspberry
 - Instalar la librería WiringPi
 - Instalar python en la Raspberry

----------------------------------------------------------------------------------------------------

### Guias
 Instalación del sistema operativo RaspberryPi OS:
 
 Instalar gcc para Raspberry:
 
 Instalar la libería WiringPi: 

----------------------------------------------------------------------------------------------------

### Funcionamiento

14-septiembre-2021: La carpeta "Secuencia Leds" incluye un archivo con comandos en para ejecutar desde un shell de python para encender y apagar LEDs manualmente

14-septiembre-2021: La carpeta "SecLEDs" incluye un programa en c que prende y apaga 4 LEDs automaticamente en diferentes secuencias usando secuencias no bloqueantes

*Nota: Para el programa contenido en SecLEDs falta realizar el envío y recepción de mensajes utilizando MQTT (no visto en clase)

----------------------------------------------------------------------------------------------------

### Notas adicionales
El editor usado para compilar proagramas en c fue Geany en el cuál se modificó lo siguiente para que pueda compilar la librería WiringPi:

En Build -> Set Build Commands -> c++ commands modificar:
 - En el campo Compile: g++ -Wall -c "%f" -l wiringPi
 - En el campo Build: g++ -Wall -o "%e" "%f"  -l wiringPi
 
Por: Fernando Daniel Ramirez

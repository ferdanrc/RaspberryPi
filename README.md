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
 Confituracion de la Raspberry Pi:
 
 https://edu.codigoiot.com/course/view.php?id=823
 
 https://edu.codigoiot.com/course/view.php?id=815
 
 https://edu.codigoiot.com/enrol/index.php?id=824
 
 https://edu.codigoiot.com/enrol/index.php?id=843
 
 https://edu.codigoiot.com/enrol/index.php?id=844
 
 https://edu.codigoiot.com/course/view.php?id=816
 
 Instalar gcc para Raspberry:
 
 Instalar la libería WiringPi: 

----------------------------------------------------------------------------------------------------

### Funcionamiento

14-septiembre-2021: La carpeta "Secuencia Leds" incluye un archivo con comandos en python en para ejecutar desde un shell de python para encender y apagar LEDs manualmente. 

14-septiembre-2021: La carpeta "SecLEDs" incluye un programa en c que prende y apaga 4 LEDs automaticamente en diferentes secuencias usando secuencias no bloqueantes. 

La carpeta "Sensor_XML90614" contiene un script de python que obtiene lecturas de temperatura del sensor XML90614. 

La carpeta "RFID" contiene un script de python que lee una tarjeta de RF con un sensor RFID-RC522. 

La carpeta "Temperatura_CPU_node_red" contiene el flow de node red para desplegar uso y temperatura de los CPUs de la Raspberry. 

La carpeta "PI4J" contiene un programa en Java para prender y apagar un LED utilizando PI4J. 

Cada progama tiene como anexo fotos del circuito armado y un video que muestra su funcionamiento

----------------------------------------------------------------------------------------------------

### Notas adicionales
El editor usado para compilar proagramas en c fue Geany en el cuál se modificó lo siguiente para que pueda compilar la librería WiringPi:

En Build -> Set Build Commands -> c++ commands modificar:
 - En el campo Compile: g++ -Wall -c "%f" -l wiringPi
 - En el campo Build: g++ -Wall -o "%e" "%f"  -l wiringPi


El editor usado para ejecutar scripts en python fue el IDLE de python. 

Por: Fernando Daniel Ramirez

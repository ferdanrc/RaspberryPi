#se guarda con la extension .py
#es para python 3

from smbus2 import SMBus
from mlx90614 import MLX90614
import time

# Bucle infinito
while True:
    bus = SMBus(1) # Inicia el bus de I2C para leer el sensor
    sensor = MLX90614(bus, address=0x5A) #Inicializa el sensor en la direccion 0x%A
    print ("Temperatura ambiente :", sensor.get_ambient()) #Obtiene lectura de temperatura del ambiente
    print ("Temperatura de Persona u objeto :", sensor.get_object_1()) # Obtiene la lectura de temperatura 
    bus.close() # Cierra el bus de I2C
    time.sleep(1) # Espera 1 segundo

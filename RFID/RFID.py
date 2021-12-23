import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522
from time import sleep
led=40 # Pin del LED
GPIO.setwarnings(False)    # Ignore warning for now
GPIO.setmode(GPIO.BOARD) # Inicializar la raspberry
GPIO.setup(led,GPIO.OUT) # Inicializar el LED
GPIO.output(led, GPIO.LOW) # Apagar el LED
reader = SimpleMFRC522() # Instanciar un objeto de lectura para el sensor RFID
while True: # Bucle infinito
        try:
            id, text = reader.read() #Si hay una tarjeta RF cerca, la lee
            print(id) # Imprime el ID de la tarjeta
            print(type(id)) # Imprime el tipo del ID de la tarjeta
            print(text) # Imprime el texto guardado en la tarjeta
            sleep(2)

            if id==872589727: # Si el ID es valido
                sleep(2) # Esperar 2 segundos
                GPIO.output(led, GPIO.HIGH) # Prender el LED
                sleep(1) # Esperar 1 segundo
            else:
                GPIO.output(led, GPIO.LOW) # Apagar el LED
        except:
                GPIO.cleanup() # Limpiar 

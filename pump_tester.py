import RPi.GPIO as GPIO
from time import sleep
GPIO.setmode(GPIO.BCM) 
 
RELAYS_1_GPIO = 17
GPIO.setup(RELAYS_1_GPIO, GPIO.OUT)
GPIO.output(RELAYS_1_GPIO, GPIO.LOW)
GPIO.output(RELAYS_1_GPIO, GPIO.HIGH)
sleep(10)
GPIO.output(RELAYS_1_GPIO, GPIO.LOW)
GPIO.cleanup()
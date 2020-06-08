import requests
import random
import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BCM)

TOKEN = "BBFF-wmzRjKFusGT4Mh6lHrsmk9ID3m6c1l" 
DEVICE = "uniRPi3" 
VARIABLE = "waterSwitch"
DELAY = 1
PUMP_PERIOD = 10

RELAYS_1_GPIO = 17
GPIO.setup(RELAYS_1_GPIO, GPIO.OUT) # GPIO Assign mode

def get_var(DEVICE, VARIABLE):
    try:
        url = "http://industrial.api.ubidots.com/"
        url = url + \
            "api/v1.6/devices/{0}/{1}/".format(DEVICE, VARIABLE)
        headers = {"X-Auth-Token": TOKEN, "Content-Type": "application/json"}
        req = requests.get(url=url, headers=headers)
        return req.json()['last_value']['value']
    except:
        pass
   
def pump_water():   
    
    GPIO.output(RELAYS_1_GPIO, GPIO.LOW) 
    GPIO.output(RELAYS_1_GPIO, GPIO.HIGH)
    sleep(PUMP_PERIOD)
    GPIO.output(RELAYS_1_GPIO, GPIO.LOW)
    VARIABLE = 0


if __name__ == "__main__":
    while True:
        print(get_var(DEVICE, VARIABLE))
        if (VARIABLE == 1):
            pump_water()
        sleep(DELAY)
GPIO.cleanup()
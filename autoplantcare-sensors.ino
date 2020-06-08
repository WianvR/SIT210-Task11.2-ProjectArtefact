/*
 * Final Project: Automatic Plant Maintenance - Sensors
 * Description: This C code run on particle takes in light and temp readings by I2C and moisture through analogue 1
 * It then publishes it to Ubidots dashboard for display
 * Author: Wian van Rooyen
 * Date: 07/06/2020
 */

#include <Ubidots.h>
//#include <I2CSlaveRK.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_BMP280.h>


Adafruit_BMP280 bmp;
Adafruit_TSL2591 tsl;
//I2CSlave device(Wire, 0x10,10);

const unsigned long PERIOD = 5000;
const char* WEBHOOK_NAME = "Ubidots";

bool sensorReady = false;
unsigned long lastPublish = 0;
int soilSensor = 0;
uint32_t counter = 0;
uint16_t regAddr;
//uint16_t takeReading;
Ubidots ubidots("webhook", UBI_PARTICLE);

void setup()
{
    //Sensor Setup
    Serial.begin(9600);
    
    if (!bmp.begin()) {
        Serial.println("Can't find sensor BMP280");
    }
    Serial.println("BMP280 connected!");
    
    if (!tsl.begin()) {
        Serial.println("Can't find TSL2591");
    }
    Serial.println("TSL2591 connected!");
    
    Particle.variable("soilSensor", &soilSensor, INT);
    pinMode(A1, INPUT);
    
    //I2C Setup
    //device.begin();
    //Wire.onReceive(receiveEvent);
}

/* C Method take master data
void receiveEvent(int howMany)
    {
        while (Wire.available())
        {
            char takeReading = Wire.Read();
        }
    }
*/

void loop()
{
    
   if ((millis() - lastPublish >= PERIOD))
    {
        lastPublish = millis();

        int temp = round(bmp.readTemperature());
        
        uint32_t lum = tsl.getFullLuminosity();
        uint16_t ir, fullSpec;
        ir = lum >> 16;
        fullSpec = lum & 0xFFFF;
        
        soilSensor = analogRead(A1);
        
        Particle.publish("BMP_Reading", String(temp)+"C", PRIVATE);
        Particle.publish("TSL_Reading", String(fullSpec)+" lux", PRIVATE);
        Particle.publish("Soil_Moisture", String(soilSensor), PRIVATE);
        
        ubidots.add("Temperature", temp); 
        ubidots.add("Light", fullSpec);
        ubidots.add("Moisture", soilSensor);
        
        bool bufferSent = false;
        bufferSent = ubidots.send(WEBHOOK_NAME, PUBLIC);  // sends data

         if(bufferSent)
         {
            // Do something if values were sent properly
            Particle.publish("Transmit ", "Transmission Successful", PRIVATE);
         }
         
         else
        {
            Particle.publish("Transmit ", "Transmission Failed", PRIVATE);    
        }
        
        //Serial.printlnf("register 0 set to %u", counter); //Output data
		//device.setRegister(0, counter++);
    }
    
	//while(device.getRegisterSet(regAddr)) {
		// regAddr was updated from the I2C master
		//Serial.printlnf("master updated %u to %u", regAddr, device.getRegister(regAddr));
	//}

}
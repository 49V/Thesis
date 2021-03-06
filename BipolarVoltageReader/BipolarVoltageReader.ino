#include <Wire.h>;
#include "Plotter.h"
#define ADC_ADDRESS 0x10
#define NUMBER_OF_BYTES 2 
#define NUMBER_OF_LEVELS 4096
#define NUMBER_OF_POINTS 5000
#define EMPTY_BYTE B00000000

// Great idea. Now I can use a float and access each byte for transmission. Shout-outs:
// https://forum.arduino.cc/index.php?topic=246654.0
typedef union
{
 float number;
 uint8_t bytes[4];
} FLOATUNION_t;

void setup() {

  // Set the pull up resistor on my analog reading pin
  digitalWrite(A0, INPUT_PULLUP);

  // put your setup code here, to run once:
  analogReference(DEFAULT);
  //To settle voltage
  analogRead(0);

  // Initiate the Wire library
  Wire.begin();
  //Begin serial transmission
  Serial.begin(9600);
  delay(100);

// Enable internal reference
  Wire.beginTransmission(ADC_ADDRESS);// Writes ADC address such that ADC knows it is being communicated with
  Wire.write(B00001011); // Writes the Pointer byte
  Wire.write(B00000010); // Writes Data MSB
  Wire.write(B00000000); //Writes Data LSB
  Wire.endTransmission();

// Pre-Setup: Access General purpose contorl register and setting Vref* 2
  
  Wire.beginTransmission(ADC_ADDRESS);// Writes ADC address such that ADC knows it is being communicated with
  Wire.write(B00000011); // Writes the Pointer byte
  Wire.write(B00000000); // Writes Data MSB
  Wire.write(B00100000); //Writes Data LSB
  Wire.endTransmission();

// Step 1: Configure Io0 as a read

  Wire.beginTransmission(ADC_ADDRESS);
  Wire.write(B00000100); // Writes the Pointer byte
  Wire.write(B00000000); //Writes Data MSB
  Wire.write(B00000001); //Writes Data LSB
  Wire.endTransmission();

// Step 2: Write to ADC Configuration/Sequence Register
  Wire.beginTransmission(ADC_ADDRESS);
  Wire.write(B00000010); // Write the Pointer byte
  Wire.write(B00000010); // Writes Data MSB
  Wire.write(B00000001); //Writes Data LSB
  Wire.endTransmission();

// Step 3: Select ADCs for reading
  Wire.beginTransmission(ADC_ADDRESS);
  Wire.write(B01000000);
  Wire.endTransmission();

}

void loop() {
  // put your main code here, to run repeatedly:
  
  FLOATUNION_t voltage; 
  float LSB, MSB, voltageRaw;
  float voltageReference = 5;

  //Request Data from ADC
  Wire.requestFrom(ADC_ADDRESS, NUMBER_OF_BYTES);

  if(Wire.available() <= NUMBER_OF_BYTES){
    MSB = 256 * Wire.read();
    LSB = Wire.read();
  }

  // Raw voltage is our value read from the sensor
  voltageRaw = LSB + MSB;

  // Converting this to a meaningful value
  voltage.number = voltageRaw * (voltageReference / NUMBER_OF_LEVELS);

  // Transmit this value over serial as a float (4 bytes)
  Serial.print("C");
  Serial.write(voltage.bytes[3]);
  Serial.write(voltage.bytes[2]);
  Serial.write(voltage.bytes[1]);
  Serial.write(voltage.bytes[0]);
  
}

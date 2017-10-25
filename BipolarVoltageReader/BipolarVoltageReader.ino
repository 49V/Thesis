#include <Wire.h>;

#define WRITE_ADC_ADDRESS B00100000
#define READ_ADC_ADDRESS  B00100001
#define NUMBER_OF_BYTES 2 
#define NUMBER_OF_LEVELS 4096
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

// Pre-Setup: Access General purpose contorl register and setting Vref* 2
  
  Wire.beginTransmission(WRITE_ADC_ADDRESS);// Writes ADC address such that ADC knows it is being communicated with
  Wire.write(B00000011); // Writes the Pointer byte
  Wire.write(B00000000); // Writes Data MSB
  Wire.write(B00100000); //Writes Data LSB
  Wire.endTransmission();

// Step 1: Configure Io0 as a read

  Wire.beginTransmission(WRITE_ADC_ADDRESS);
  Wire.write(B00000100); // Writes the Pointer byte
  Wire.write(B00000000); //Writes Data MSB
  Wire.write(B00000001); //Writes Data LSB
  Wire.endTransmission();

// Step 2: Write to ADC Configuration/Sequence Register
  Wire.beginTransmission(WRITE_ADC_ADDRESS);
  Wire.write(B00000010); // Write the Pointer byte
  Wire.write(B00000010); // Writes Data MSB
  Wire.write(B00000001); //Writes Data LSB
  Wire.endTransmission();

// Step 3: Select ADCs for reading
  Wire.beginTransmission(WRITE_ADC_ADDRESS);
  Wire.write(B01000000);
  Wire.endTransmission();

}

void loop() {
  // put your main code here, to run repeatedly:

  double LSB, MSB;
  double voltageRaw, voltage;
  double voltageReference = 5;


  //Step 4: Read from ADC

  //Request Data
  Wire.requestFrom(READ_ADC_ADDRESS, NUMBER_OF_BYTES);

  if(Wire.available() <= NUMBER_OF_BYTES){
    MSB = 256 * Wire.read();
    LSB = Wire.read();
  }

  // Raw voltage is our value read from the sensor
  voltageRaw = LSB + MSB;

  // Converting this to a meaningful value
  //voltage = voltageRaw * (voltageReference / NUMBER_OF_LEVELS);

  Serial.println(MSB, 5);
  Serial.println(LSB, 5);

  delay(500);
     
  // Arduino ADC Code 
  //voltageRaw = analogRead(A0);
 // voltage = voltageRaw * (voltageReference / 1023); 
  // PLot the lower limit
  //Serial.println(voltage, 6);
  

}

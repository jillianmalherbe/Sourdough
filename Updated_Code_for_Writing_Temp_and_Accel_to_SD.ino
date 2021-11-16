#include <SPI.h>

#include <SD.h>

/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  This example code is in the public domain.

*/

File myFile;

// analog pin
int sensor_pin = 0;

//Code for temperature sensor 
int analog_data;
float voltage_reading;
float temperatureC; 

//Code for accelerometer
#include<Wire.h>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only

  //This code sets up the transmition of data from the accelerometer
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened properly, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    //myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // nothing happens after setup
  // read the analog input
  analog_data = analogRead(sensor_pin);
  
  // convert analog input to a voltage value 
  // change 5.0 to 3.3 if using 3.3 volt supply
  voltage_reading = analog_data * (5.0/1024.0); 
  
  // convert voltage value to a temperature value
  temperatureC = (voltage_reading - 0.5) * 100; 
  
  // print out our values 
  Serial.print("Analog Value: ");
  Serial.println(analog_data);
  myFile.print("Analog Value: ");
  myFile.println(analog_data);
  
  Serial.print("Voltage Value: ");
  Serial.print(voltage_reading);
  Serial.println(" V");
  myFile.print("Voltage Value: ");
  myFile.print(voltage_reading);
  myFile.println(" V")
  
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" C");
  Serial.println("");
  myFile.print("Temperature: ");
  myFile.print(temperatureC);
  myFile.print(" C");
  myFile.println("");

  //Delay between temperature sensor and accelerometer data
  delay(1000); 

  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
  
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.println(AcZ);
  myFile.print("Accelerometer: ");
  myFile.print("X = "); Serial.print(AcX);
  myFile.print(" | Y = "); Serial.print(AcY);
  myFile.print(" | Z = "); Serial.println(AcZ); 
  
  myFile.print("Gyroscope: ");
  myFile.print("X = "); Serial.print(GyX);
  myFile.print(" | Y = "); Serial.print(GyY);
  myFile.print(" | Z = "); Serial.println(GyZ);
  myFile.println(" ");
  
  //Delay between accelerometer and temperature sensor code
  delay(1000);

  
}

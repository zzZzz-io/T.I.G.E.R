#include <Wire.h>
#include <Servo.h> 
#include <math.h>
#include <SPI.h>
#include <SD.h>
#define PI 3.1415926535
#define PIN_INPUT 0
#define PIN_OUTPUT 3

const int MPU = 0x68; // MPU6050 I2C address
float value1[10], value2[10], value3[10];
float avgvalue1, avgvalue2, avgvalue3;
double kP = .2, kI = 5, kD = 1;
float XDifference, YDifference;
int sensorPinX = A0; 
int sensorPinY = A1;
int sensorValueX = 0;
int sensorValueY = 0;
int servoPinX = 5; 
int servoPinY = 6;
int count;
float ChangeX;
float ChangeY;
int c = 0;

Servo ServoXaxis; 
Servo ServoYaxis;

void setup() {
   Serial.begin(19200);

   ServoXaxis.attach(servoPinX); // to start servo control
   ServoYaxis.attach(servoPinY); // to start servo control

   Wire.begin();                      // Initialize comunication
   Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
   Wire.write(0x6B);                  // Talk to the register 6B
   Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
   Wire.endTransmission(true);        //end the transmission
   delay(20);

   Serial.println("Starting servo check.");
    //Servo Check Code-------------------------------------------- *
   ServoYaxis.write(41);
   delay(1000);
   ServoYaxis.write(85);
   delay(1000);
   ServoYaxis.write(129);
   delay(1000);
   ServoYaxis.write(85);
   Serial.println("Y servo check complete.");

   delay(2000);

   ServoXaxis.write(41);
   delay(1000);
   ServoXaxis.write(85);
   delay(1000);
   ServoXaxis.write(129);
   delay(1000);
   ServoXaxis.write(85);
   Serial.println("X servo check complete.");
  
   delay(2000);

   ServoXaxis.write(41);
   ServoYaxis.write(41);
   delay(1000);
   ServoXaxis.write(41);
   ServoYaxis.write(129);
   delay(1000);
   ServoXaxis.write(129);
   ServoYaxis.write(129);
   delay(1000);
   ServoXaxis.write(129);
   ServoYaxis.write(41);
   delay(1000);
   ServoXaxis.write(85);
   ServoYaxis.write(85);
   Serial.println("X and Y servo check complete");
   // Servo Check Code----------------------------------------
   delay(1000);
   Serial.println("Initiating Servo Control");
}
void loop() {
  // Read Data from MPU6050 ----------------------------------------
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  c++;
  if (c = 10){
  c = 0;
  }
  value1[c] = (Wire.read() << 8 | Wire.read()) / 16384.0;
  value1[c] = 10000*value1[c];
  value1[c] = map(value1[c], -10000,10000,0,36000);
  for (int i = 0; i < 10; i++) {
    avgvalue1 += value1[i];
  }
  Serial.print(avgvalue1/100);
  Serial.print("/");
  
  value2[c] = (Wire.read() << 8 | Wire.read()) / 16384.0;
  value2[c] = 10000*value2[c];
  value2[c] = map(value2[c], -10000,10000,0,36000);
  for (int i = 0; i < 10; i++) {
    avgvalue2 += value2[i];
  }
    Serial.print(avgvalue2/100);
    // end MPU6050 reading ---------------------------------------------------
   sensorValueX = analogRead(sensorPinX);
   sensorValueY = analogRead(sensorPinY);
   XDifference = (180 - (avgvalue1/100));

   YDifference = 180 - (avgvalue2/100);
   ChangeX = (kP * (XDifference));// + (kI * ()) + (kD * ());
   ChangeY = (kP * (YDifference));// + (kI * ()) + (kD * ());
   //ChangeX = (sensorValueX-512)*44/512;
   //ChangeY = (sensorValueY-512)*44/512;
   //Printing Servo Values----------------------------------------------------
   Serial.print("        X Change Equals");
   Serial.print(ChangeX);
   Serial.print(" / Y Change Equals");
   Serial.println(ChangeY);
   //Printing Servo Values-----------------------------------------------------
   ServoXaxis.write(85+ChangeX);
   ServoYaxis.write(85+ChangeY);

   //Reset Variables----------------------------------------------------------
   avgvalue1=0;
   avgvalue2=0;
  }
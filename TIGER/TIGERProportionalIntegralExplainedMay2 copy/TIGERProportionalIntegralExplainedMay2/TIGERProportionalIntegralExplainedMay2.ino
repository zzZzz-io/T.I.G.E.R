//adding important libraries
#include <Wire.h>
#include <Servo.h> 
#include <math.h>
#include <SPI.h>
#include <SD.h>
//defining set variables (because that's what the princeton university people did)
#define PI 3.1415926535
#define PIN_INPUT 0
#define PIN_OUTPUT 3
//defining variables
//I don't understand this one
const int MPU = 0x68; // MPU6050 I2C address
//used to store the values from the gyroscope
float value1[10], value2[10], value3[10];
float avgvalue1, avgvalue2, avgvalue3;
//constants that need to be tuned
double kP = .2, kI = 5, kD = 2;
//variables that are used to calculate the error on each axis; used in P of PID
float XDifference, YDifference;
//used in the code imbedded to accept potentiometer input
int PotPinX = A0;
int PotPinY = A1; 
//declaring the pins that send outputs to the TVC mount
int servoPinX = 5, servoPinY = 6; 
//extra value that is used in various parts of the code
int count;
//final values that are sent to servos
float ChangeX , ChangeY;
//other values used randomly
int c = 0, a = 0;
//time variables used in I of PID
double StartTime, CurrentTime, ElapsedTime;
//integral sums used in I of PID
double SumX, SumY;
double AverageErrorX, AverageErrorY;
//declaring servos
Servo ServoXaxis; 
Servo ServoYaxis;

void setup() {
   //starting communication to serial monitor
   Serial.begin(19200);
   
   ServoXaxis.attach(servoPinX); // to start servo control
   ServoYaxis.attach(servoPinY); // to start servo control

   Wire.begin();                      // Initialize comunication
   Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
   Wire.write(0x6B);                  // Talk to the register 6B
   Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
   Wire.endTransmission(true);        //end the transmission
   delay(20);
   /*Servo Check Code-------------------------------------------- *
   Serial.println("Starting servo check.");
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
   //End of Servo Check Code---------------------------------------- *
   delay(1000);
   Serial.println("Initiating Servo Control");
   */
   // begin the time measuring code
   StartTime = millis();
}
void loop() {
  //to continue time measurement
  CurrentTime = millis();
  ElapsedTime = CurrentTime - StartTime;
  // Read Data from MPU6050 (i think this might be redundant)----------------------------------------
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //End value reading from MPU6050----------------------------------
  // 2 values that are used to calculate average values over the last 10 readings and to smooth the readings
  a++;
  c++;
  if (c = 10){
  c = 0;
  }
  // reading x values -----------------------------------------
  value1[c] = (Wire.read() << 8 | Wire.read()) / 16384.0;
  value1[c] = 10000*value1[c];
  value1[c] = map(value1[c], -10000,10000,0,36000);
  for (int i = 0; i < 10; i++) {
    avgvalue1 += value1[i];
  }
  Serial.print(avgvalue1/100);
  Serial.print("/");
  // reading y values--------------------------------------------
  value2[c] = (Wire.read() << 8 | Wire.read()) / 16384.0;
  value2[c] = 10000*value2[c];
  value2[c] = map(value2[c], -10000,10000,0,36000);
  for (int i = 0; i < 10; i++) {
    avgvalue2 += value2[i];
  }
    Serial.print(avgvalue2/100);
   // end MPU6050 reading ---------------------------------------------------
   //adding the newest value to the sum (integral)
   SumX = SumX + ((value1[c]/100)-180);
   SumY = SumY + ((value2[c]/100)-180);
   // calculating the value that will be applied to the total change as the integral
   AverageErrorX = SumX / (ElapsedTime);
   AverageErrorY = SumY / (ElapsedTime);
   // calculating the value that will be applied to the total change as proportional
   XDifference = 180 - (avgvalue1/100);
   YDifference = 180 - (avgvalue2/100);
   //calculating the final value using the three individual parts and their tuned constants
   ChangeX = (kP * (XDifference)) - (kI * (AverageErrorX));// + (kD * ());
   ChangeY = (kP * (YDifference)) - (kI * (AverageErrorY));// + (kD * ());
   //Printing Servo Values----------------------------------------------------
   Serial.print("        X Change Equals ");
   Serial.print(ChangeX);
   Serial.print(" / Y Change Equals ");
   Serial.print(ChangeY);
   Serial.print(".    Time is");
   Serial.println(ElapsedTime);
   //Setting Servo Positions-----------------------------------------------------
   ServoXaxis.write(85+ChangeX);
   ServoYaxis.write(85+ChangeY);
   //Reset Variables----------------------------------------------------------
   avgvalue1=0;
   avgvalue2=0;
  }
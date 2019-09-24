/*************************************************************************
 * Color Sensor TCS34725 Whisker Tutorial:
 * This program will print the TCS34725 RGB, lux, and clearness values to 
 * the Serial Monitor and a TinyScreen+
 * 
 * Hardware by: TinyCircuits
 * Code by: Laverena Wienclaw for TinyCircuits
 *
 * Initiated: Mon. 11/29/2017 
 * Updated: Tue. 07/03/2018
 ************************************************************************/

#include <Wire.h>         // For I2C communication with sensor
#include <TinyScreen.h>   // For interfacing with the TinyScreen+
#include "Adafruit_TCS34725.h"  // The library used for the Color Sensor

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

// TinyScreen+ variables
TinyScreen display = TinyScreen(TinyScreenPlus);
int background = TS_8b_Black;

// Variables to hold the values the sensor reads
uint16_t r, g, b, c, colorTemp, lux;

void setup(void) {
  SerialUSB.begin(9600);
  Wire.begin();

  while(!SerialUSB);

  // Setup and style for TinyScreen+
  display.begin();
  display.setFlip(true);
  display.setBrightness(15);
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, background);

  //The port is the number on the Adapter board where the sensor is attached
  selectPort(0);

  if (tcs.begin()) {
    SerialUSB.println("Found sensor");
  } else {
    SerialUSB.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop(void) {
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  SerialUSB.print("Color Temp: "); SerialUSB.print(colorTemp); SerialUSB.print(" K, ");
  SerialUSB.print("Lux: "); SerialUSB.print(lux, DEC); SerialUSB.print(", ");
  SerialUSB.print("R: "); SerialUSB.print(r, DEC); SerialUSB.print(", ");
  SerialUSB.print("G: "); SerialUSB.print(g, DEC); SerialUSB.print(", ");
  SerialUSB.print("B: "); SerialUSB.print(b); SerialUSB.print(", ");
  SerialUSB.print("Clr: "); SerialUSB.print(c, DEC);
  SerialUSB.println(" ");

  printScreen();

  delay(500);
}


// Prints the color sensor values to the TinyScreen
void printScreen(void){
  // This will make the screen look a little unsteady but is needed in order
  // to clear old values 
  display.clearScreen();
  
  display.fontColor(TS_8b_DarkRed, background);
  display.setCursor(0, 0);
  display.print("ColorTemp = ");
  display.print(colorTemp);

  display.fontColor(TS_8b_Yellow, background);
  display.setCursor(0, 11);
  display.print("Lux = ");
  display.print(lux);

  display.fontColor(TS_8b_Red, background);
  display.setCursor(0, 22);
  display.print("R = ");
  display.print(r);

  display.fontColor(TS_8b_Green, background);
  display.setCursor(0, 33);
  display.print("G = ");
  display.print(g);

  display.fontColor(TS_8b_Blue, background);
  display.setCursor(0, 44);
  display.print("B = ");
  display.print(b);

  display.fontColor(TS_8b_White, background);
  display.setCursor(0, 55);
  display.print("Clr = ");
  display.print(c);
}

// **This function is necessary for all Whisker boards attached through an Adapter board**
// Selects the correct address of the port being used in the Adapter board
void selectPort(int port) {
  Wire.beginTransmission(0x70);
  Wire.write(0x04 + port);
  Wire.endTransmission();
}

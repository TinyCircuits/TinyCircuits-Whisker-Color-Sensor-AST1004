/*************************************************************************
 * Color Sensor TCS34725 Wireling Tutorial:
 * This program will print the TCS34725 RGB, lux, and clearness values to 
 * the Serial Monitor and a TinyScreen+
 * 
 * Note: I²C Address for Wireling board is different than the Adafruit 
 * component, this address change is reflected in the library that
 * should be included with this sketch 
 * 
 * Hardware by: TinyCircuits
 * Code by: Laveréna Wienclaw for TinyCircuits
 *
 * Initiated: 11/29/2017 
 * Updated: 12/18/2019
 ************************************************************************/

#include <Wire.h>         // For I2C communication with sensor
#include <TinyScreen.h>   // For interfacing with the TinyScreen+
#include "Adafruit_TCS34725.h"  // The library used for the Color Sensor
#include <Wireling.h>     // For interfacing with Wirelings

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

// Make compatible with all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
  #define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#endif

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

// TinyScreen+ variables
TinyScreen display = TinyScreen(TinyScreenPlus);
int background = TS_8b_Black;

// Variables to hold the values the sensor reads
uint16_t r, g, b, c, colorTemp, lux;

void setup(void) {
  SerialMonitorInterface.begin(9600);
  Wire.begin();
  Wireling.begin();

  // Setup and style for TinyScreen+
  display.begin();
  display.setFlip(true);
  display.setBrightness(15);
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, background);

  //The port is the number on the Adapter board where the sensor is attached
  Wireling.selectPort(0);

  if (tcs.begin()) {
    SerialMonitorInterface.println("Found sensor");
  } else {
    SerialMonitorInterface.println("No TCS34725 found ... check your connections");
    while (1);
  }

  // Turn Wireling LEDs on 
  LEDon();
}

void loop(void) {
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  SerialMonitorInterface.print("Color Temp: "); SerialMonitorInterface.print(colorTemp); SerialMonitorInterface.print(" K, ");
  SerialMonitorInterface.print("Lux: "); SerialMonitorInterface.print(lux, DEC); SerialMonitorInterface.print(", ");
  SerialMonitorInterface.print("R: "); SerialMonitorInterface.print(r, DEC); SerialMonitorInterface.print(", ");
  SerialMonitorInterface.print("G: "); SerialMonitorInterface.print(g, DEC); SerialMonitorInterface.print(", ");
  SerialMonitorInterface.print("B: "); SerialMonitorInterface.print(b); SerialMonitorInterface.print(", ");
  SerialMonitorInterface.print("Clr: "); SerialMonitorInterface.print(c, DEC);
  SerialMonitorInterface.println(" ");

  printScreen();

  delay(500);
}

// Turn Wireling LEDs on
void LEDon() {
  tcs.setInterrupt(true);
}

// Turn Wireling LEDs off
void LEDoff() {
  tcs.setInterrupt(false);
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

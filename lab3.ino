#include <Arduino.h>
#include <MD_TCS230.h>
#include "LedControl.h"
#include "symbols.h"

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5
MD_TCS230 ColorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);

#define R_OUT 6
#define G_OUT 7
#define B_OUT 8

const int displaysCount = 1;
const int dataPin = 12;
const int clkPin = 10;
const int csPin = 11;
LedControl lc = LedControl(dataPin, clkPin, csPin, displaysCount);

void setup()
{   
    Serial.begin(115200);
    Serial.println("Started!");

    lc.shutdown(0, false);
    lc.setIntensity(0, 16);
    lc.clearDisplay(0);

    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 120060;
    whiteCalibration.value[TCS230_RGB_G] = 117520;
    whiteCalibration.value[TCS230_RGB_B] = 157590;
    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 11280;
    blackCalibration.value[TCS230_RGB_G] = 10270;
    blackCalibration.value[TCS230_RGB_B] = 13230;

    ColorSensor.begin();
    ColorSensor.setDarkCal(&blackCalibration);
    ColorSensor.setWhiteCal(&whiteCalibration);

    pinMode(R_OUT, OUTPUT);
    pinMode(G_OUT, OUTPUT);
    pinMode(B_OUT, OUTPUT);
}

void loop() 
{
    colorData rgb;
    ColorSensor.read();
    while (!ColorSensor.available());
    ColorSensor.getRGB(&rgb);
    print_result(rgb);
}

void print_result(colorData rgb)
{
    int red = round(rgb.value[TCS230_RGB_R] / 32);
    int green = round(rgb.value[TCS230_RGB_G] / 32);
    int blue = round(rgb.value[TCS230_RGB_B] / 32);

//    Serial.print(rgb.value[TCS230_RGB_R]);
//    Serial.print(" ");
//    Serial.print(rgb.value[TCS230_RGB_G]);
//    Serial.print(" ");
//    Serial.print(rgb.value[TCS230_RGB_B]);
//    Serial.println(" ");

    lc.clearDisplay(0);
    for (int i = 0; i < red; i++)
    {
        lc.setLed(0, 0, i, true);
    }
    for (int i = 0; i < green; i++)
    {
        lc.setLed(0, 1, i, true);
    }
    for (int i = 0; i < blue; i++)
    {
        lc.setLed(0, 2, i, true);
    }
}

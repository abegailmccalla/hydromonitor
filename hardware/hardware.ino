// ##################################################################################################################
// ##                                      ELET2415 DATA ACQUISITION SYSTEM CODE                                   ##
// ##                                                                                                              ##
// ##################################################################################################################

// LIBRARY IMPORTS
#include "Arduino.h"
#include <rom/rtc.h>
#include <math.h> // https://www.tutorialspoint.com/c_standard_library/math_h.htm
#include <ctype.h>
#include "DHT.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h" //library for TFT screen
#include <Wire.h>
#include <SPI.h>             //library for SPI connection for TFT screen
#include <Adafruit_Sensor.h> //general library for sensors
#include <PubSubClient.h>
#include "time.h"
#include "sntp.h"
#include <Adafruit_BMP280.h>
// IMPORT FONTS FOR TFT DISPLAY
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <FastLED.h>
#include <GyverBME280.h> //library for additional functions for bmp/bme sensor

#ifndef _WIFI_H
#include <WiFi.h>
#endif

#ifndef STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef ARDUINO_H
#include <Arduino.h>
#endif

#ifndef ARDUINOJSON_H
#include <ArduinoJson.h>
#endif

// Define pins for TFT LCD screen
#define TFT_DC 17
#define TFT_CS 5
#define TFT_RST 16
#define TFT_CLK 18
#define TFT_MOSI 23
#define TFT_MISO 19

// DEFINE VARIABLES
#define espTimeout_ms 300
#define ARDUINOJSON_USE_DOUBLE 1
#define BTN1 27
#define BTN2 26
#define BTN3 25

// DEFINE THE CONTROL PINS FOR THE DHT22
#define DHTPIN 32
#define DHTTYPE DHT22
double dhtt = 0;
double dhth = 0;
double dhthi = 0;

// DEFINE THE CONTROL PINS FOR THE Soil Sensor
#define MOISTURE_PIN 33

// Pressure Sensor
#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)
double bmpt = 0;
double bmpp = 0;
double bmpa = 0;

const int AirValue = 3520;   // Analogue Reading when sensor is dry
const int WaterValue = 1600; // Analogue reading when sensor is wet
int moisture = 0;
int soilMoistPerc = 000;

// MQTT CLIENT CONFIG
static const char *pubtopic = "620157646";                      // Add your ID number here
static const char *subtopic[] = {"620157646_sub", "/elet2415"}; // Array of Topics(Strings) to subscribe to
static const char *mqtt_server = "dbs.msjrealtms.com";          // Broker IP address or Domain name as a String (www.yanacreations.com/dbs.msjrealtms.com)
static uint16_t mqtt_port = 1883;

// WIFI CREDENTIALS
const char *ssid = "MonaConnect"; // Add your Wi-Fi ssid (MonaConnect/Digicel_WiFi_JHTR)
const char *password = "";      // Add your Wi-Fi password (/zu72Kg7p)

// TASK HANDLES
TaskHandle_t xMQTT_Connect = NULL;
TaskHandle_t xNTPHandle = NULL;
TaskHandle_t xLOOPHandle = NULL;
TaskHandle_t xUpdateHandle = NULL;
TaskHandle_t xButtonCheckeHandle = NULL;

// FUNCTION DECLARATION
void checkHEAP(const char *Name); // RETURN REMAINING HEAP SIZE FOR A TASK
void initMQTT(void);              // CONFIG AND INITIALIZE MQTT PROTOCOL
unsigned long getTimeStamp(void); // GET 10 DIGIT TIMESTAMP FOR CURRENT TIME
void callback(char *topic, byte *payload, unsigned int length);
void initialize(void);
bool publish(const char *topic, const char *payload); // PUBLISH MQTT MESSAGE(PAYLOAD) TO A TOPIC
void vButtonCheck(void *pvParameters);
void vUpdate(void *pvParameters);

/* Declare your functions below */
double convert_Celsius_to_fahrenheit(double c);
double convert_fahrenheit_to_Celsius(double f);
double calcHeatIndex(double Temp, double Humid);
double round2(double value);
bool isNumber(double number);

/* Init class Instances for the DHT22 etcc */
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp; // use I2C interface

// Define TFT LCD screen object
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// ############### IMPORT HEADER FILES ##################
#ifndef NTP_H
#include "NTP.h"
#endif

#ifndef MQTT_H
#include "mqtt.h"
#endif

void setup()
{
  pinMode(BTN1,INPUT_PULLUP);
  pinMode(BTN2,INPUT_PULLUP);
  pinMode(BTN3,INPUT_PULLUP);

  Serial.begin(115200); // INIT SERIAL
  tft.begin();
  tft.setRotation(3);
  // tft.setFont(&FreeSansBold18pt7b);
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_PURPLE);
  tft.setTextSize(2);

  // INITIALIZE ALL SENSORS AND DEVICES
  dht.begin();
  bmp.begin(0x76);
  // pinMode(MOISTURE_PIN, INPUT);
  /* Default settings from datasheet. */
  // bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
  //                 Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
  //                 Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
  //                 Adafruit_BMP280::FILTER_X16,      /* Filtering. */
  //                 Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  initialize(); // INIT WIFI, MQTT & NTP
  //vButtonCheckFunction(); // UNCOMMENT IF USING BUTTONS INT THIS LAB, THEN ADD LOGIC FOR INTERFACING WITH BUTTONS IN THE vButtonCheck FUNCTION
}

void loop()
{
  // put your main code here, to run repeatedly:
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.println("**************************");
  tft.println(" ");

  tft.setTextColor(ILI9341_PURPLE);
  tft.print("   A");
  tft.setTextColor(ILI9341_RED);
  tft.print("T");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("M");
  tft.setTextColor(ILI9341_RED);
  tft.print("'");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("S");
  tft.print(" ");
  tft.setTextColor(ILI9341_BLUE);
  tft.print("W");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("E");
  tft.setTextColor(ILI9341_BLUE);
  tft.print("A");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("T");
  tft.setTextColor(ILI9341_BLUE);
  tft.print("H");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("E");
  tft.setTextColor(ILI9341_BLUE);
  tft.print("R");
  tft.print(" ");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("S");
  tft.setTextColor(ILI9341_RED);
  tft.print("T");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("A");
  tft.setTextColor(ILI9341_RED);
  tft.print("T");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("I");
  tft.setTextColor(ILI9341_RED);
  tft.print("O");
  tft.setTextColor(ILI9341_PURPLE);
  tft.println("N");
  tft.println(" ");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("DHT Temperature:   ");
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
  tft.print(int(dhtt));
  tft.println(" *C");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("BMP Temperature:   ");
  tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);
  tft.print(int(bmpt));
  tft.println(" *C");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("DHT Humidity:      ");
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
  tft.print(int(dhth));
  tft.println(" %");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("DHT Heat Index:    ");
  tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);
  tft.print(int(dhthi));
  tft.println(" *C");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("BMP Pressure:   ");
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
  tft.print(int(bmpp));
  tft.println(" Pa");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("BMP Altitude:      ");
  tft.setTextColor(ILI9341_BLUE, ILI9341_WHITE);
  tft.print(int(bmpa));
  tft.println(" m");
  tft.setTextColor(ILI9341_PURPLE);
  tft.print("Soil Moisture:    ");
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
  tft.printf("%03d", soilMoistPerc);
  tft.println(" %");

  tft.println(" ");
  tft.setTextColor(ILI9341_YELLOW);
  tft.println("**************************");

  if(digitalRead(BTN1) == LOW){
      delay(200);
      tft.fillScreen(ILI9341_BLUE);
      delay(500);
      tft.fillScreen(ILI9341_PURPLE);
      delay(500);
      tft.fillScreen(ILI9341_RED);
      delay(500);
      tft.fillScreen(ILI9341_WHITE);
      delay(200);

      tft.setCursor(0, 0);
      tft.setTextColor(ILI9341_YELLOW);
      tft.println("**************************");
      tft.println(" ");
      tft.setTextColor(ILI9341_PURPLE);
      tft.print(" ACCURACY IS OUR POLICY!!!  ");
      tft.println(" ");
      tft.setTextColor(ILI9341_YELLOW);
      tft.println("**************************");
      delay(2000);

      tft.fillScreen(ILI9341_RED);
      delay(500);
      tft.fillScreen(ILI9341_PURPLE);
      delay(500);
      tft.fillScreen(ILI9341_BLUE);
      delay(500);
      tft.fillScreen(ILI9341_WHITE);
      delay(200);
  }

  if(digitalRead(BTN2) == LOW){
    delay(200);
  }

  if(digitalRead(BTN3) == LOW){
    delay(200);
  }
}

// ####################################################################
// #                          UTIL FUNCTIONS                          #
// ####################################################################
void vButtonCheck(void *pvParameters)
{
  configASSERT(((uint32_t)pvParameters) == 1);

  for (;;)
  {
    // Add code here to check if a button(S) is pressed
    // then execute appropriate function if a button is pressed
    // Display sensor values on TFT LCD screen
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void vUpdate(void *pvParameters)
{
  configASSERT(((uint32_t)pvParameters) == 1);

  for (;;)
  {
    // Task code goes here.
    // PUBLISH to topic every second.

    // DHT22 Sensor
    dhtt = dht.readTemperature();
    dhth = dht.readHumidity();
    dhthi = calcHeatIndex(convert_Celsius_to_fahrenheit(dhtt), dhth);
    Serial.println("DHT Temperature: ");
    Serial.print(dhtt);
    Serial.print(" *C");
    Serial.println(" ");
    Serial.println("DHT Humidity: ");
    Serial.print(dhth);
    Serial.print(" %");
    Serial.println(" ");
    Serial.println("DHT heatindex: ");
    Serial.print(dhthi);
    Serial.print(" *C");
    Serial.println(" ");

    // SOIL SENSOR
    moisture = analogRead(MOISTURE_PIN);
    soilMoistPerc = map(moisture, AirValue, WaterValue, 0, 100);
    // if statement to account for errors/slight deviation in sensor readings
    if (soilMoistPerc >= 100)
    {
      soilMoistPerc = 100;
    }
    else if (soilMoistPerc <= 0)
    {
      soilMoistPerc = 0;
    }
    Serial.println("Soil Moisture Level: ");
    // Serial.printf("%02d", soilMoistPerc);
    Serial.print(soilMoistPerc);
    Serial.print(" %");
    Serial.println(" ");
    Serial.println(moisture);

    // BMP Sensor
    bmpt = bmp.readTemperature();
    bmpp = bmp.readPressure();
    bmpa = bmp.readAltitude(1013.25); // 1013.25
    Serial.println("BMP Temperature: ");
    Serial.print(bmpt);
    Serial.print(" *C");
    Serial.println(" ");
    Serial.println("BMP Pressure: ");
    Serial.print(bmpp);
    Serial.print(" Pa");
    Serial.println(" ");
    Serial.println("BMP Altitude: ");
    Serial.print(bmpa);
    Serial.print(" m");
    Serial.println(" ");

    if (isNumber(dhtt))
    {
      // ##Publish update according to {"Type":"Sensor", "ID":620157646, "DHT_Temperature":0, "BMP_Temperature":0, "DHT_Humidity":0, "DHT_HeatIndex":0, "BMP_Pressure":0, "BMP_Altitude":0, "Soil_Moisture":0}

      // 1. Create JSon objects
      StaticJsonDocument<1000> doc; // Create JSon object
      char message[1100] = {0};

      // 2. Create message buffer/array to store serialized JSON object

      // 3. Add key:value pairs to JSon object based on above schema
      doc["Type"] = "Sensor";
      doc["ID"] = "620157646";
      doc["Timestamp"] = getTimeStamp();
      doc["DHT_Temperature"] = dhtt;
      doc["BMP_Temperature"] = bmpt;
      doc["DHT_Humidity"] = dhth;
      doc["DHT_HeatIndex"] = dhthi; // dht.computeHeatIndex(t,h);
      doc["BMP_Pressure"] = bmpp;
      doc["BMP_Altitude"] = bmpa;
      doc["Soil_Moisture"] = soilMoistPerc; // soilMoistPerc;

      // 4. Seralize / Covert JSon object to JSon string and store in message array

      serializeJson(doc, message); // Seralize / Covert JSon object to JSon string and store in char* array

      // 5. Publish message to a topic sobscribed to by both backend and frontend
      if (mqtt.connected())
      {
        publish(pubtopic, message);
      }
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

unsigned long getTimeStamp(void)
{
  // RETURNS 10 DIGIT TIMESTAMP REPRESENTING CURRENT TIME
  time_t now;
  time(&now); // Retrieve time[Timestamp] from system and save to &now variable
  return now;
}

void callback(char *topic, byte *payload, unsigned int length)
{
  // ############## MQTT CALLBACK  ######################################
  // RUNS WHENEVER A MESSAGE IS RECEIVED ON A TOPIC SUBSCRIBED TO

  Serial.printf("\nMessage received : ( topic: %s ) \n", topic);
  char *received = new char[length + 1]{0};

  for (int i = 0; i < length; i++)
  {
    received[i] = (char)payload[i];
  }

  // PRINT RECEIVED MESSAGE
  Serial.printf("Payload : %s \n", received);

  // CONVERT MESSAGE TO JSON
  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, received);

  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  // PROCESS MESSAGE
  const char *type = doc["type"];

  if (strcmp(type, "Sensor") == 0)
  {
    // 1. EXTRACT ALL PARAMETERS: NODES, RED,GREEN, BLUE, AND BRIGHTNESS FROM JSON OBJECT
    const char *message = doc["message"];
  }
}

bool publish(const char *topic, const char *payload)
{
  bool res = false;
  try
  {
    res = mqtt.publish(topic, payload);
    // Serial.printf("\nres : %d\n",res);
    if (!res)
    {
      res = false;
      throw false;
    }
  }
  catch (...)
  {
    Serial.printf("\nError (%d) >> Unable to publish message\n", res);
  }
  return res;
}

//***** Complete the util functions below ******

// Celsius to Fahrenheit Conversion Formula: Multiply the °C temperature by 1.8. Add 32 to this number. This is the answer in °F.
double convert_Celsius_to_fahrenheit(double c)
{
  // CONVERTS INPUT FROM °C TO °F. RETURN RESULTS
  return (c * 1.8) + 32;
}

// Fahrenheit to Celsius Conversion Formula: Subtract 32 from the °F temperature. Multiply this number by 5/9. This is the answer in °C.
double convert_fahrenheit_to_Celsius(double f)
{
  // CONVERTS INPUT FROM °F TO °C. RETURN RESULT
  return (f - 32) * (5.0 / 9.0);
}

double calcHeatIndex(double Temp, double Humid)
{
  // CALCULATE AND RETURN HEAT INDEX USING EQUATION FOUND AT https://byjus.com/heat-index-formula/#:~:text=The%20heat%20index%20formula%20is,an%20implied%20humidity%20of%2020%25
  return convert_fahrenheit_to_Celsius(-42.379 + (2.04901523 * Temp) + (10.14333127 * Humid) + (-0.22475541 * Temp * Humid) + (-(6.83783 * pow(10, -3)) * Temp * Temp) + (-(5.481717 * pow(10, -2)) * Humid * Humid) + ((1.22874 * pow(10, -3)) * Temp * Temp * Humid) + ((8.5282 * pow(10, -4)) * Temp * Humid * Humid) + (-(1.99 * pow(10, -6)) * Temp * Temp * Humid * Humid));
}

double round2(double value)
{
  return (int)(value * 100 + 0.5) / 100.0;
}

bool isNumber(double number)
{
  char item[20];
  snprintf(item, sizeof(item), "%f\n", number);
  if (isdigit(item[0]))
    return true;
  return false;
}

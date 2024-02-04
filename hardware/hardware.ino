//##################################################################################################################
//##                                      ELET2415 DATA ACQUISITION SYSTEM CODE                                   ##
//##                                                                                                              ##
//##################################################################################################################

// LIBRARY IMPORTS
#include <rom/rtc.h> 
#include <math.h>  // https://www.tutorialspoint.com/c_standard_library/math_h.htm 
#include <ctype.h>

// ADD YOUR IMPORTS HERE
#include "DHT.h"
#include <FastLED.h>


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

 

// DEFINE VARIABLES
#define ARDUINOJSON_USE_DOUBLE      1 

// DEFINE THE CONTROL PINS FOR THE DHT22 
#define DHTPIN 22
#define DHTTYPE DHT22

#define DATA_PIN 2
#define NUM_LEDS 7



// MQTT CLIENT CONFIG  
static const char* pubtopic      = "620157646";                    // Add your ID number here
static const char* subtopic[]    = {"620157646_sub","/elet2415"};  // Array of Topics(Strings) to subscribe to
static const char* mqtt_server   = "dbs.msjrealtms.com";         // Broker IP address or Domain name as a String 
static uint16_t mqtt_port        = 1883;

// WIFI CREDENTIALS
const char* ssid       = "Digicel_WiFi_JHTR";     // Add your Wi-Fi ssid
const char* password   = "zu72Kg7p"; // Add your Wi-Fi password 




// TASK HANDLES 
TaskHandle_t xMQTT_Connect          = NULL; 
TaskHandle_t xNTPHandle             = NULL;  
TaskHandle_t xLOOPHandle            = NULL;  
TaskHandle_t xUpdateHandle          = NULL;
TaskHandle_t xButtonCheckeHandle    = NULL;  

// FUNCTION DECLARATION   
void checkHEAP(const char* Name);   // RETURN REMAINING HEAP SIZE FOR A TASK
void initMQTT(void);                // CONFIG AND INITIALIZE MQTT PROTOCOL
unsigned long getTimeStamp(void);   // GET 10 DIGIT TIMESTAMP FOR CURRENT TIME
void callback(char* topic, byte* payload, unsigned int length);
void initialize(void);
bool publish(const char *topic, const char *payload); // PUBLISH MQTT MESSAGE(PAYLOAD) TO A TOPIC
void vButtonCheck( void * pvParameters );
void vUpdate( void * pvParameters );  
bool isNumber(double number);
 

/* Declare your functions below */ 
double convert_Celsius_to_fahrenheit(double c);
double convert_fahrenheit_to_Celsius(double f);
double calcHeatIndex(double Temp, double Humid);


/* Init class Instances for the DHT22 etcc */
DHT dht(DHTPIN, DHTTYPE);
CRGB  leds[NUM_LEDS];
 
  

//############### IMPORT HEADER FILES ##################
#ifndef NTP_H
#include "NTP.h"
#endif

#ifndef MQTT_H
#include "mqtt.h"
#endif

// Temporary Variables 


void setup() {
  Serial.begin(115200);  // INIT SERIAL  

  // INITIALIZE ALL SENSORS AND DEVICES
  
  /* Add all other necessary sensor Initializations and Configurations here */
  LEDS.addLeds<WS2812,DATA_PIN, GRB> (leds, NUM_LEDS);       // Compensate for GRB ordering  
  dht.begin();


  initialize();     // INIT WIFI, MQTT & NTP 
  // vButtonCheckFunction(); // UNCOMMENT IF USING BUTTONS INT THIS LAB, THEN ADD LOGIC FOR INTERFACING WITH BUTTONS IN THE vButtonCheck FUNCTION
 }
  


void loop() {
    // put your main code here, to run repeatedly:       
    vTaskDelay(1000 / portTICK_PERIOD_MS);    
}




  
//####################################################################
//#                          UTIL FUNCTIONS                          #       
//####################################################################
void vButtonCheck( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );     
      
    for( ;; ) {
        // Add code here to check if a button(S) is pressed
        // then execute appropriate function if a button is pressed  

        vTaskDelay(200 / portTICK_PERIOD_MS);  
    }
}

void vUpdate( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );    
           
    for( ;; ) {
          // #######################################################
          // ## This function must PUBLISH to topic every second. ##
          // #######################################################
   
          // 1. Read Humidity and save in variable below
          double h = 0;
          h = dht.readHumidity();
           
          // 2. Read temperature as Celsius   and save in variable below
          double t = 0;    
          t = dht.readTemperature();
 

          if(isNumber(t)){
              // ##Publish update according to ‘{"id": "student_id", "timestamp": 1702212234, "temperature": 30, "humidity":90, "heatindex": 30}’

              // 1. Create JSon object
            StaticJsonDocument<1000> doc; // Create JSon object
            char message[1100]  = {0};         
              
              // 2. Create message buffer/array to store serialized JSON object
              
              // 3. Add key:value pairs to JSon object based on above schema
            doc["id"]           = "620157646";
            doc["timestamp"]    = getTimeStamp();
            doc["temperature"]  = t;
            doc["humidity"]     = h;
            doc["heatindex"]   = calcHeatIndex(convert_Celsius_to_fahrenheit(t), h); //dht.computeHeatIndex(t,h); //

              // 4. Seralize / Covert JSon object to JSon string and store in message array

            serializeJson(doc, message);  // Seralize / Covert JSon object to JSon string and store in char* array

               
              // 5. Publish message to a topic sobscribed to by both backend and frontend     
            if(mqtt.connected() ){
            publish(pubtopic, message);
          }          

          }

          
            
        vTaskDelay(1000 / portTICK_PERIOD_MS);  
    }
}

 

unsigned long getTimeStamp(void) {
          // RETURNS 10 DIGIT TIMESTAMP REPRESENTING CURRENT TIME
          time_t now;         
          time(&now); // Retrieve time[Timestamp] from system and save to &now variable
          return now;
}


void callback(char* topic, byte* payload, unsigned int length) {
  // ############## MQTT CALLBACK  ######################################
  // RUNS WHENEVER A MESSAGE IS RECEIVED ON A TOPIC SUBSCRIBED TO
  
  Serial.printf("\nMessage received : ( topic: %s ) \n",topic ); 
  char *received = new char[length + 1] {0}; 
  
  for (int i = 0; i < length; i++) { 
    received[i] = (char)payload[i];    
  }

  // PRINT RECEIVED MESSAGE
  Serial.printf("Payload : %s \n",received);

 
  // CONVERT MESSAGE TO JSON
  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, received);  

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }


  // PROCESS MESSAGE
  const char* type = doc["type"]; 

  if (strcmp(type, "controls") == 0){
    // 1. EXTRACT ALL PARAMETERS: NODES, RED,GREEN, BLUE, AND BRIGHTNESS FROM JSON OBJECT
    const int brightness = doc["brightness"];
    const int nodes = doc["leds"];
    const int red = doc["color"]["r"];
    const int green = doc["color"]["g"];
    const int blue = doc["color"]["b"];
    const int alpha = doc["color"]["a"];

    // 2. ITERATIVELY, TURN ON LED(s) BASED ON THE VALUE OF NODES. Ex IF NODES = 2, TURN ON 2 LED(s)
    for(int x=0; x<nodes; x++){
      leds[x] = CRGB( red, green, blue); 
      FastLED.setBrightness( brightness ); 
      FastLED.show(); 
      delay(50); 
    }
    
    // 3. ITERATIVELY, TURN OFF ALL REMAINING LED(s).
    for(int x=nodes; x<NUM_LEDS; x++){ 
      leds[x] = CRGB::Black; 
      FastLED.setBrightness( brightness );
      FastLED.show(); 
      delay(50);
    }
  }
}

bool publish(const char *topic, const char *payload){   
     bool res = false;
     try{
        res = mqtt.publish(topic,payload);
        // Serial.printf("\nres : %d\n",res);
        if(!res){
          res = false;
          throw false;
        }
     }
     catch(...){
      Serial.printf("\nError (%d) >> Unable to publish message\n", res);
     }
  return res;
}



//***** Complete the util functions below ******

// Celsius to Fahrenheit Conversion Formula: Multiply the °C temperature by 1.8. Add 32 to this number. This is the answer in °F.
double convert_Celsius_to_fahrenheit(double c){    
    // CONVERTS INPUT FROM °C TO °F. RETURN RESULTS 
    return (c * 1.8) + 32; 
}

// Fahrenheit to Celsius Conversion Formula: Subtract 32 from the °F temperature. Multiply this number by 5/9. This is the answer in °C.
double convert_fahrenheit_to_Celsius(double f){    
    // CONVERTS INPUT FROM °F TO °C. RETURN RESULT 
    return (f - 32) * (5/9);
}

// HI= c1+c2T+c3R+c4TR+c5T^2+c6R^2+c7T^2R+c8TR^2+c9T^2R^2
// In this formula,
// HI = heat index in degrees Fahrenheit
// R = Relative humidity
// T = Temperature in ∘F
// c1 = -42.379
// c2= -2.04901523
// c3 = -10.14333127
// c4 = -0.22475541
// c5 = -6.83783 x 10−3
// c6 = -5.481717 x 10−2
// c7 = -1.22874 x 10−3
// c8 = 8.5282 x 10−4
// c9 = -1.99 x 10−6
double calcHeatIndex(double Temp, double Humid){
    // CALCULATE AND RETURN HEAT INDEX USING EQUATION FOUND AT https://byjus.com/heat-index-formula/#:~:text=The%20heat%20index%20formula%20is,an%20implied%20humidity%20of%2020%25
    //dht.computeHeatIndex(Temp, Humid, true); //The last argument, 'true,' changes the output to Fahrenheit.
    return -42.379 + (-2.04901523 * Temp) + (-10.14333127 * Humid) + (-0.22475541 * Temp * Humid) + (-0.00683783 * Temp * Temp) + (-0.05481717 * Humid * Humid) + (-0.00122874 * Temp * Temp * Humid) + (0.00085282 * Temp * Humid * Humid) + (-0.00000199 * Temp * Temp * Humid * Humid);
}
 

bool isNumber(double number){       
        char item[20];
        snprintf(item, sizeof(item), "%f\n", number);
        if( isdigit(item[0]) )
          return true;
        return false;
}

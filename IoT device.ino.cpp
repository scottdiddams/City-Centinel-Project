#include <Arduino.h>
#line 1 "c:\\Users\\ASUS\\MSSA2021\\Project\\ESP32-Elec\\Device\\device.ino"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <gfxfont.h>

#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include "time.h"
#include "EmonLib.h"

#include <WiFi.h>
#include "Esp32MQTTClient.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

EnergyMonitor emon1;                   // Create an instance
#define STA_SSID "NETGEAR85"
#define STA_PASS "helpfulraven289"
#define AP_SSID  "esp32-v6"

static volatile bool wifi_connected = false;

WiFiUDP ntpClient;

#define INTERVAL 10000
#define MESSAGE_MAX_LEN 256
// SSID and password of WiFi
 	const char* ssid     = "NETGEAR85";
 	const char* password = "**********";

/*String containing Hostname, Device Id & Device Key in the format:                         */
#define HostName = "myCitySentinelHub.azure-devices.net";
#define DeviceId = "ESP32-Elec";
#define SharedAccessKey = "FxV36Cvv4VdWnq9og0MHCkjxIgNGOJnwCenE/B+kysQ=";
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
static const char* connectionString = "HostName=myCitySentinelHub.azure-devices.net;DeviceId=ESP32-Elec;SharedAccessKey=FxV36Cvv4VdWnq9og0MHCkjxIgNGOJnwCenE/B+kysQ=";
const char *messageData = "{\"messageId\":%d, \"Current\":%f, \"Power\":%f}";
static bool hasIoTHub = false;
static bool hasWifi = false;
int messageCount = 1;
static bool messageSending = true;
static uint64_t send_interval_ms;

#line 50 "c:\\Users\\ASUS\\MSSA2021\\Project\\ESP32-Elec\\Device\\device.ino"
static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result);
#line 58 "c:\\Users\\ASUS\\MSSA2021\\Project\\ESP32-Elec\\Device\\device.ino"
static void MessageCallback(const char* payLoad, int size);
#line 64 "c:\\Users\\ASUS\\MSSA2021\\Project\\ESP32-Elec\\Device\\device.ino"
static void DeviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char *payLoad, int size);
#line 78 "c:\\Users\\ASUS\\MSSA2021\\Project\\ESP32-Elec\\Device\\device.ino"
static int DeviceMethodCallback(const char *methodName, const unsigned char *payload, int size, unsigned char **response, int *response_size);
#line 108 "c:\\Users\\ASUS\\MSSA2021\\Project\\ESP32-Elec\\Device\\device.ino"
void wifiConnectedD2CLoop();
#line 135 "c:\\Users\\ASUS\\MSSA2021\\Project\\ESP32-Elec\\Device\\device.ino"
void wifiConnectedD2COffLoop();
#line 162 "c:\\Users\\ASUS\\MSSA2021\\Project\\ESP32-Elec\\Device\\device.ino"
void wifiConnectedC2DStopLoop();
#line 191 "c:\\Users\\ASUS\\MSSA2021\\Project\\ESP32-Elec\\Device\\device.ino"
void setup();
#line 245 "c:\\Users\\ASUS\\MSSA2021\\Project\\ESP32-Elec\\Device\\device.ino"
void loop();
#line 50 "c:\\Users\\ASUS\\MSSA2021\\Project\\ESP32-Elec\\Device\\device.ino"
static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result)
{
  if (result == IOTHUB_CLIENT_CONFIRMATION_OK)
  {
    Serial.println("Send Confirmation Callback finished.");
  }
}

static void MessageCallback(const char* payLoad, int size)
{
  Serial.println("Message callback:");
  Serial.println(payLoad);
}

static void DeviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char *payLoad, int size)
{
  char *temp = (char *)malloc(size + 1);
  if (temp == NULL)
  {
    return;
  }
  memcpy(temp, payLoad, size);
  temp[size] = '\0';
  // Display Twin message.
  Serial.println(temp);
  free(temp);
}

static int  DeviceMethodCallback(const char *methodName, const unsigned char *payload, int size, unsigned char **response, int *response_size)
{
  LogInfo("Try to invoke method %s", methodName);
  const char *responseMessage = "\"Successfully invoke device method\"";
  int result = 200;

  if (strcmp(methodName, "start") == 0)
  {
    LogInfo("Start sending data");
    messageSending = true;
  }
  else if (strcmp(methodName, "stop") == 0)
  {
    LogInfo("Stop sending data");
    messageSending = false;
    wifiConnectedC2DStopLoop();
  }
  else
  {
    LogInfo("No method %s found", methodName);
    responseMessage = "\"No method found\"";
    result = 404;
  }

  *response_size = strlen(responseMessage) + 1;
  *response = (unsigned char *)strdup(responseMessage);

  return result;
}

void wifiConnectedD2CLoop(){
  
    configTime(-18000, -18000, "time.nist.gov");
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    
            display.clearDisplay();
            display.setTextSize(1);             // Normal 1:1 pixel scale
            display.setTextColor(SSD1306_WHITE);        
            display.setCursor(0,0);             //First row        
            display.println("Connected:Sending D2C");
            display.print("IPv4:");
            display.println(WiFi.localIP());
            display.setCursor(0,17);             //Third row
            display.println(&timeinfo, "%H%M:%SR %d %b %Y");
            
            double Irms = emon1.calcIrms(1480);  //  Irms
            display.setCursor(0, 25);             
            display.print(Irms);
            display.write("A ");
            display.print(Irms*120);
            display.write("W RMS");
            display.display();
  
  delay(1000);
}
void wifiConnectedD2COffLoop(){
  
    configTime(-18000, -18000, "time.nist.gov");
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    
            display.clearDisplay();
            display.setTextSize(1);             // Normal 1:1 pixel scale
            display.setTextColor(SSD1306_WHITE);        
            display.setCursor(0,0);             //First row        
            display.println("Connected:Waiting...");
            display.print("IPv4:");
            display.print(WiFi.localIP());
            display.setCursor(0,17);             //Third row
            display.println(&timeinfo, "%H%M:%SR %d %b %Y");
            
            double Irms = emon1.calcIrms(1480);  //  Irms
            display.setCursor(0, 25);             
            display.print(Irms);
            display.write("A ");
            display.print(Irms*120);
            display.write("W RMS");
            display.display();
  
  delay(1000);
}
void wifiConnectedC2DStopLoop(){
  
    configTime(-18000, -18000, "time.nist.gov");
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    
            display.clearDisplay();
            display.setTextSize(1);             // Normal 1:1 pixel scale
            display.setTextColor(SSD1306_WHITE);        
            display.setCursor(0,0);             //First row        
            display.println("Connected:Stopped");
            display.print("IPv4:");
            display.print(WiFi.localIP());
            display.setCursor(0,17);             //Third row
            display.println(&timeinfo, "%H%M:%SR %d %b %Y");
            
            double Irms = emon1.calcIrms(1480);  //  Irms
            display.setCursor(0, 25);             
            display.print(Irms);
            display.write("A ");
            display.print(Irms*120);
            display.write("W RMS");
            display.display();
  
  delay(1000);
}


void setup() 
{
  Serial.begin(9600);
  emon1.current(34, 60.6);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  Serial.begin(115200);
  Serial.println("ESP32 Device");
  Serial.println("Initializing...");
  Serial.println(" > WiFi");
  Serial.println("Starting connecting WiFi.");
  
  display.clearDisplay();
  display.println("Connecting to WiFi");
  display.setCursor(0, 8);
  
  delay(10);
  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.print(".");
    hasWifi = false;
  }
  hasWifi = true;
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(" > IoT Hub");
  if (!Esp32MQTTClient_Init((const uint8_t*)connectionString, true))
  {
    hasIoTHub = false;
    Serial.println("Initializing IoT hub failed.");
    return;
  }
  hasIoTHub = true;
  Esp32MQTTClient_SetSendConfirmationCallback(SendConfirmationCallback);
  Esp32MQTTClient_SetMessageCallback(MessageCallback);
  Esp32MQTTClient_SetDeviceTwinCallback(DeviceTwinCallback);
  Esp32MQTTClient_SetDeviceMethodCallback(DeviceMethodCallback);
  Serial.println("Start sending events.");
  randomSeed(analogRead(0));
  send_interval_ms = millis();

  

}

void loop() {
if (hasWifi && hasIoTHub)
  {
    if (messageSending && 
        (int)(millis() - send_interval_ms) >= INTERVAL)
    {
      configTime(-18000, -18000, "time.nist.gov");
      struct tm timeinfo;
      getLocalTime(&timeinfo);
      Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      wifiConnectedD2CLoop();


      // Send data
      char messagePayload[MESSAGE_MAX_LEN];
      double Current = emon1.calcIrms(1480);
      double Power = Current*120;
      snprintf(messagePayload, MESSAGE_MAX_LEN, messageData, messageCount++, Current, Power);
      Serial.println(messagePayload);
      EVENT_INSTANCE* message = Esp32MQTTClient_Event_Generate(messagePayload, MESSAGE);
      
      Esp32MQTTClient_SendEventInstance(message);
      send_interval_ms = millis();     
    }
    else
    {
      Esp32MQTTClient_Check();
      wifiConnectedD2COffLoop();
    }
  }
  delay(10);
  
}

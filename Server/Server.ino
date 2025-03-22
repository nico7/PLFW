//#include "back_end.h"
#include "adc.h"
#include "dac.h"
#include "gpios.h"
#include "laser.h"
#include "oled.h"
#include "tec.h"

#include "driver/adc.h"

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <string.h>
#include <mDNS.h>
#include "menu.h"

unsigned long myTime = millis();
const int output18 = 18;
uint16_t adc_val = 0;


int len;
char adc_data[50];
char screen_data[50] = "\n\rSSID: FN530\n\rPW: holograms\n\rIP: 192.168.4.1";
bool wifi_connection = false;
  AsyncWebServer server(80);

const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "gateway";

String ssid;
String pass;
String ip;
String gateway;

const char* ssidPath    = "/ssid.txt";
const char* passPath    = "/pass.txt";
const char* ipPath      = "/ip.txt";
const char* gatewayPath = "/gateway.txt";

IPAddress localIP(192,168,4,1);
IPAddress localGateway;
IPAddress subnet(255, 255, 0, 0);

unsigned long previousMillis = 0;
const long interval = 10000;  // Interval to wait for Wi-Fi connection (ms)
bool wifi_ing = false;

String readFile(fs::FS &fs, const char * path)
{
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    return String();
  }

  String fileContent;
  while(file.available())
  {
    fileContent = file.readStringUntil('\n');
    break;
  }

  return fileContent;
}

// Write file to LittleFS
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}

// Initialize WiFi
bool initWiFi() {
  if(ssid=="" || ip==""){
    Serial.println("Undefined SSID or IP address.");
    return false;
  }

  WiFi.mode(WIFI_STA);
  localIP.fromString(ip.c_str());
  localGateway.fromString(gateway.c_str());


  if (!WiFi.config(localIP, localGateway, subnet)){
    Serial.println("STA Failed to configure");
    return false;
  }
  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println("Connecting to WiFi...");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      Serial.println("Failed to connect.");
      oled_print((uint8_t *) "Failed to connect\n\r", 19);
      return false;
    }
  }

  return true;
}

static char ip_address[16];
void setup()
{
  // Initialize the output variables as outputs
  
  gpios_init();
  ll_oled_init();
  dac_init();
  tec_init();
  laser_init();
  delay(500);
   
  Serial.begin(115200);

  if(!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  // Load values saved in LittleFS
  ssid = readFile(LittleFS, ssidPath);
  pass = readFile(LittleFS, passPath);
  ip = readFile(LittleFS, ipPath);
  gateway = readFile (LittleFS, gatewayPath);
  Serial.println(ssid);
  Serial.println(pass);
  Serial.println(ip);
  Serial.println(gateway);

  if(initWiFi())
  {
    // Route for root / web page
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html");
      });
      server.on("/laserdac", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", String(dac_get_currentmA(LASER)).c_str());
      });
      server.on("/lasercurrent", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", String(adc_get(ADC_LSR)).c_str());
      });
      server.on("/tecdac", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", String(dac_get_currentmA(TEC)).c_str());
      });
      server.on("/teccurrent", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", String(adc_get(ADC_TEC)).c_str());
      });
      server.on("/tectemperature", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", String(adc_get(ADC_THR)).c_str());
      });
      server.on("/htrcurrent", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", String(adc_get(ADC_HTR)).c_str());
      });
 
    server.begin();
    wifi_connection = true;
  }
  else
  {
    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("FN530", "holograms");
    
    oled_print((uint8_t *) screen_data, strlen(screen_data));
    
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/wifimanager.html", "text/html");
    });
    
    server.serveStatic("/", LittleFS, "/");
    
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
      int params = request->params();
      for(int i=0;i<params;i++)
      {
        const AsyncWebParameter* p = request->getParam(i);
        if(p->isPost())
        {
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) 
          {
            ssid = p->value().c_str();
            Serial.print("SSID set to: ");
            Serial.println(ssid);
            // Write file to save value
            writeFile(LittleFS, ssidPath, ssid.c_str());
          }
          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) 
          {
            pass = p->value().c_str();
            Serial.print("Password set to: ");
            Serial.println(pass);
            // Write file to save value
            writeFile(LittleFS, passPath, pass.c_str());
          }
          // HTTP POST ip value
          if (p->name() == PARAM_INPUT_3) 
          {
            ip = p->value().c_str();
            Serial.print("IP Address set to: ");
            Serial.println(ip);
            // Write file to save value
            writeFile(LittleFS, ipPath, ip.c_str());
          }
          // HTTP POST gateway value
          if (p->name() == PARAM_INPUT_4) 
          {
            gateway = p->value().c_str();
            Serial.print("Gateway set to: ");
            Serial.println(gateway);
            // Write file to save value
            writeFile(LittleFS, gatewayPath, gateway.c_str());
          }
          //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
      }
      request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + ip);
      delay(3000);
      ESP.restart();
    });
    server.begin();
  }
}
  

        
     

      
 // Get ADC and DAC readings
//  String get_devices_readings()
//  {
//    readings["Laser DAC"]       = String(dac_get_currentmA(LASER));      // DAC value set
//    readings["Laser Current"]   = String(adc_get(ADC_LSR));              // ADC value
//    readings["TEC DAC"]         = String(dac_get_currentmA(TEC));        // DAC value set
//    readings["TEC Current"]     = String(adc_get(ADC_TEC));              // ADC value
//    readings["TEC Temperature"] = String(adc_get(ADC_THR));              // ADC value
//    readings["Heater Current"]  = String(adc_get(ADC_HTR));              // ADC value
//    String jsonString = JSON.stringify(readings);
//    return jsonString;
//  }
      
  




void loop()
{
 if(wifi_connection)
 {


    uint8_t buttons = NONE_PRESSED;
    int left_state = digitalRead(BACK_BTN);
    int right_state = digitalRead(SELECT_BTN);
    int up_state = digitalRead(UP_BTN);
    int down_state = digitalRead(DOWN_BTN);

    if(!left_state)
    {
      buttons = LEFT_PRESSED;
    }
    else if(!right_state)
    {
      buttons = RIGHT_PRESSED;
    }
    else if(!up_state)
    {
      buttons = UP_PRESSED;
    }
    else if(!down_state)
    {
      buttons = DOWN_PRESSED;
    }

    menu_sm(buttons);
  }

  // if(LOW == up_state)
  // {
  //   Serial.println("UP_pressed!\n");
  //   delay(500);
  //   dac_set_current(TEC, 500);
  //   dac_set_current(LASER, 25);
  //   tec_enable(true);
  //   laser_enable(true);
  // }
  // else if(LOW == down_state)
  // {
  //   Serial.println("DOWN pressed\n");
  //   delay(500);
    
  //   dac_set_current(TEC, 0);
  //   dac_set_current(LASER, 0);
  //   tec_enable(false);
  //   laser_enable(false);
    
  // }

 


  
  
  
  
  
}



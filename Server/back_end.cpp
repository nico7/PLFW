#include "back_end.h"
#include "laser.h"
#include "tec.h"

#include <Arduino.h>
#include <WiFi.h>
#include <mDNS.h>

#include "gpios.h"
#include "oled.h"

#define ON  2
#define OFF 1

static WifiStatus_E wifi_state = BE_DISCONNECTED;

static char ip_address[16];

WiFiServer server(80);

String header;

const  String off_on[3] = {"?", "off", "on"};

static uint8_t shutter_state  = OFF;
static uint8_t laser_state    = OFF;
static uint8_t tec_state      = OFF;


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void back_end_sm(void) {
  switch (wifi_state) {
    case BE_DISCONNECTED:

  
      wifi_state = BE_CONNECTED;

      Serial.println("");
      Serial.println("WiFi connected.");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      snprintf(ip_address, sizeof(ip_address), "%d.%d.%d.%d",
             WiFi.localIP()[0], WiFi.localIP()[1],
             WiFi.localIP()[2], WiFi.localIP()[3]);

      oled_print((uint8_t *) ip_address, sizeof(ip_address));

      server.begin();

      break;
    case BE_CONNECTED:
      // Print local IP address and start web server
      if (WiFi.status() != WL_CONNECTED) {
        wifi_state = BE_DISCONNECTED;
      } else {
        WiFiClient client = server.available();  // Listen for incoming clients

        if (client) {  // If a new client connects,
          currentTime = millis();
          previousTime = currentTime;
          Serial.println("New Client.");                                             // print a message out in the // Serial port
          String currentLine = "";                                                   // make a String to hold incoming data from the client
          while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
            currentTime = millis();
            if (client.available()) {  // if there's bytes to read from the client,
              char c = client.read();  // read a byte, then
              Serial.write(c);         // print it out the // Serial monitor
              header += c;
              if (c == '\n') {  // if the byte is a newline character
                // if the current line is blank, you got two newline characters in a row.
                // that's the end of the client HTTP request, so send a response:
                if (currentLine.length() == 0) {
                  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                  // and a content-type so the client knows what's coming, then a blank line:
                  client.println("HTTP/1.1 200 OK");
                  client.println("Content-type:text/html");
                  client.println("Connection: close");
                  client.println();

                  // turns the GPIOs on and off
                  if (header.indexOf("GET / ") >= 0)
                  {
                    shutter_state = OFF;
                    laser_state   = OFF;
                    tec_state     = OFF;
                  }
                  else if (header.indexOf("GET /SHTR/on") >= 0) 
                  {
                    // Serial.println("GPIO SHUTTER on");
                    shutter_state = ON;
                    digitalWrite(SHUTTER, HIGH);
                  } 
                  else if (header.indexOf("GET /SHTR/off") >= 0) 
                  {
                    // Serial.println("GPIO SHUTTER off");
                    shutter_state = OFF;
                    digitalWrite(SHUTTER, LOW);
                  }

                  if(header.indexOf("GET /LSR/on") >= 0)
                  {
                    laser_state = ON;
                    laser_enable(true);
                  }
                  else if(header.indexOf("GET /LSR/off") >= 0)
                  {
                    laser_state = OFF;
                    laser_enable(false);
                  }
                   
                   if(header.indexOf("GET /TEC/on") >= 0)
                  {
                    tec_state = ON;
                    tec_enable(true);
                  }
                  else if(header.indexOf("GET /TEC/off") >= 0)
                  {
                    tec_state = OFF;
                    tec_enable(false);
                  }
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".gray {background-color: #555555;}");
            client.println(".blue {background-color: #2291E9;}");
            client.println(".red {background-color: #AA0000;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>FN530 CTRL PANEL</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>SHUTTER is " + off_on[shutter_state] + "</p>");
            // If the SHUTTERState is off, it displays the ON button       
            if (shutter_state == ON) {
              client.println("<p><a href=\"/SHTR/off\"><button class=\"button gray\">TURN OFF</button></a></p>");
            } else
            {
              client.println("<p><a href=\"/SHTR/on\"><button class=\"button\">TURN ON</button></a></p>");
            } 

            client.println("<p>LASER is " + off_on[laser_state] + "</p>");
            if(laser_state == ON)
            {
              client.println("<p><a href=\"/LSR/off\"><button class=\"button gray\">TURN OFF</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/LSR/on\"><button class=\"button red\">TURN ON</button></a></p>");
            }
            
            client.println("<p>TEC is " + off_on[tec_state] + "</p>");
            if(tec_state == ON)
            {
              client.println("<p><a href=\"/TEC/off\"><button class=\"button gray\">TURN OFF</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/TEC/on\"><button class=\"button blue\">TURN ON</button></a></p>");
            }

            client.println("</body></html>");
            
                  // The HTTP response ends with another blank line
                  //client.println();
                  // Break out of the while loop
                  break;
                } else {  // if you got a newline, then clear currentLine
                  currentLine = "";
                }
              } else if (c != '\r') {  // if you got anything else but a carriage return character,
                currentLine += c;      // add it to the end of the currentLine
              }
            }
          }
          // Clear the header variable
          header = "";
          // Close the connection
          client.stop();
          // Serial.println("Client disconnected.");
          // Serial.println("");
        }
      }

      break;
    default:
      break;
  }
}

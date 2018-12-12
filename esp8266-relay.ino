// Import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>

//Server listening port
#define LPORT 80

// AP parameters
const char* ssid = "SSID";
const char* password = "SSID PASSWORD";

// relay state and number
int relayControl(String command);
int relayInput;

// Create an instance of the server
WiFiServer server(LPORT);

// Create aREST instance
aREST_UI rest = aREST_UI();
  
void setup(void) 
{
  IPAddress ip(192,168,86,105);
  IPAddress gateway(192,168,86,1);
  IPAddress subnet(255,255,255,0);
  
  // Start Serial
  Serial.begin(115200);

  // Title of control page
  rest.title("xmas lights");

  // define buttons to control gpio
  rest.button(2); //ESP8266 - D2 = GPIO4
  rest.button(1); //ESP8266 D1 = GPIO5
  
  // exposed API
  rest.function("relay", relayControl);

  // declare name and id for device
  rest.set_id("1");
  rest.set_name("esp8266");

  // Connect to AP
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  server.begin();
}

void loop() 
{
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
  while (!client.available()) 
  {
    delay(1);
  }
  rest.handle(client);
}

int relayControl(String command) 
{
  int state = command.toInt();
  digitalWrite(relayInput, state);
  return 1;
}

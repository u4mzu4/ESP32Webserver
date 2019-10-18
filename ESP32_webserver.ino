// Import required libraries
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "credentials.h" // Replace with your network credentials

//Global variables
float waterTemperature = 27.34;
float actualTemperature = 23.123;
float actualHumidity = 47.123;
float actualPressure = 1003.123;
float bmeTemperature = 22.321;
float transData[2];
float setValue = 22.0;
float setFloorTemp = 22.0;
float kitchenTemp = 21.932;
int setControlBase = 2;
bool boilerON = 0;
bool floorON = 1;
bool radiatorON = 0;
bool heatingON = 1;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String processor(const String& var)
{
  transData[1] = 23.12;
  String tempvar = var;
  tempvar.remove(2);
  int numberofPH = tempvar.toInt();
  switch (numberofPH) {
    case 1:
      return (String(actualHumidity, 0) + " %%");
      break;
    case 2:
      return (String(actualPressure, 0) + " mbar");
      break;
    case 3:
      return (String(actualTemperature, 1) + " °C");
      break;
    case 4:
      if (radiatorON)
      {
        return String("on");
      }
      else
      {
        return String("off");
      }
      break;
    case 5:
      if (boilerON)
      {
        return String("on");
      }
      else
      {
        return String("off");
      }
      break;
    case 6:
      if (floorON)
      {
        return String("on");
      }
      else
      {
        return String("off");
      }
      break;
    case 7:
      return (String(bmeTemperature, 1) + " °C");
      break;
    case 8:
      return (String(transData[1], 1) + " °C");
      break;
    case 9:
      return (String(kitchenTemp, 1) + " °C");
      break;
    case 10:
      return (String(waterTemperature, 1) + " °C");
      break;
    case 11:
      return (String(setValue, 1));
      break;
    case 12:
      return (String(setFloorTemp, 1));
      break;
    case 13:
      if (setControlBase == 1)
      {
        return String("checked");
      }
    case 14:
      if (setControlBase == 2)
      {
        return String("checked");
      }
    case 15:
      if (heatingON)
      {
        return String("checked");
      }
    default:
      return String();
      break;
  }
  return String();
}

void SetUpWebServer ()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    int paramsNr = request->params();
    if (paramsNr == 4)
    {
      for (int j = 0; j < paramsNr; j++)
      {
        AsyncWebParameter* p = request->getParam(j);
        if (p->name() == "p1")
        {
          setValue = p->value().toFloat();
          //Blynk.virtualWrite(V5, setValue);
        }
        if (p->name() == "p2")
        {
          heatingON = (bool)(p->value().toInt());
          //Blynk.virtualWrite(V2, heatingON);
        }
        if (p->name() == "p3")
        {
          setFloorTemp  = p->value().toFloat();
          //Blynk.virtualWrite(V6, setFloorTemp);
        }
        if (p->name() == "p4")
        {
          setControlBase  = p->value().toInt();
          //Blynk.virtualWrite(V12, setControlBase);
        }
      }
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to load circle.css file
  server.on("/circle.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/circle.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });
  server.begin();
}


void setup() {
  // Initialize SPIFFS
  SPIFFS.begin(true);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  // Setup Webserver
  SetUpWebServer();
}

void loop() {

}

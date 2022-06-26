#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FastLED.h>
#include <config.h>

int lastDetectorState = LOW;
int val = 0;
const unsigned long delayTime = 5e3;
unsigned long triggerTime = 0;

int lastSerialTime = 0;

CRGB leds[NUM_LEDS];

void triggerStart()
{
  Serial.println("Turn LED ON");

  fill_solid(leds, NUM_LEDS, CRGB::White);
  leds[0] = CRGB::Black;

  lastDetectorState = HIGH;
  triggerTime = millis();
}

void triggerEnd()
{
  Serial.println("Turn LED OFF");

  fill_solid(leds, NUM_LEDS, CRGB::Black);

  lastDetectorState = LOW;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");
  Serial.println("\n\n--- Running project: \"RGB_strip\" ---");
  Serial.println("https://github.com/SpiZeak/RGB_strip\n\n");

  pinMode(SENSOR_PIN, INPUT);

  FastLED.addLeds<LED_TYPE, DATA_PIN, BRG>(leds, NUM_LEDS);
  FastLED.setMaxRefreshRate(400);
  FastLED.setBrightness(100);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  Serial.print("Connecting to wifi... ");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PW);

  while (WiFi.status() != WL_CONNECTED)
  {
    fill_solid(leds, NUM_LEDS, CRGB(0, 136, 255));
    FastLED.show();
    delay(500);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay(500);
  }

  FastLED.setBrightness(255);
  Serial.println("Done.");

  ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA.onStart(
      []()
      {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
        {
          type = "sketch";
        }
        else
        { // U_FS
          type = "filesystem";
        }

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        Serial.println("Start updating " + type);
      });

  ArduinoOTA.onEnd(
      []()
      {
        Serial.println("\nEnd");
      });

  ArduinoOTA.onProgress(
      [](unsigned int progress, unsigned int total)
      {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      });

  ArduinoOTA.onError(
      [](ota_error_t error)
      {
        Serial.printf("Error[%u]: ", error);

        if (error == OTA_AUTH_ERROR)
        {
          Serial.println("Auth Failed");
        }
        else if (error == OTA_BEGIN_ERROR)
        {
          Serial.println("Begin Failed");
        }
        else if (error == OTA_CONNECT_ERROR)
        {
          Serial.println("Connect Failed");
        }
        else if (error == OTA_RECEIVE_ERROR)
        {
          Serial.println("Receive Failed");
        }
        else if (error == OTA_END_ERROR)
        {
          Serial.println("End Failed");
        }
      });

  ArduinoOTA.begin();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  ArduinoOTA.handle();

  val = digitalRead(SENSOR_PIN);

  if (millis() - lastSerialTime > 1000)
  {
    Serial.println(val);
    lastSerialTime = millis();
  }

  if (val == HIGH)
  {
    triggerStart();
  }

  if (millis() - triggerTime > delayTime)
  {
    if (val == LOW && lastDetectorState == HIGH)
    {
      triggerEnd();
    }
  }

  FastLED.show();
}

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FastLED.h>
#include <config.h>

CRGB leds[NUM_LEDS];

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");
  Serial.println("\n\n--- Running project: \"RGB_strip\" ---");
  Serial.println("https://github.com/SpiZeak/RGB_strip\n\n");

  Serial.print("Connecting to wifi... ");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PW);

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("\nConnection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

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

  FastLED.addLeds<TM1829, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop()
{
  ArduinoOTA.handle();
  leds[0] = CRGB(0, 0, 255);
  leds[1] = CRGB(0, 0, 255);
  leds[2] = CRGB(0, 0, 255);
  leds[3] = CRGB(0, 0, 255);
  leds[4] = CRGB(0, 0, 255);
  leds[5] = CRGB(0, 0, 255);
  leds[6] = CRGB(0, 0, 255);
  leds[7] = CRGB(0, 0, 255);
  FastLED.show();
}

// Blue Red Green

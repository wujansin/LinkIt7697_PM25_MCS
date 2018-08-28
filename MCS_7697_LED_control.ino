/* Edit by master */
#include <LWiFi.h>
#include <TLSClient.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <MCS.h>
#include <MCS_debug.h>
#include <WiFiClient.h>
#include "DHT.h"

#define DHTTYPE DHT22   // DHT 22  (AM2302)

// Assign AP ssid / password here _SSID "mifare2"   //iPhone7p
char ssid[] = " ";   //allan1129
char key[] = " ";

// Assign device id / key of your test device
MCSDevice mcs("D2IzGtGW", "40cyB94fxHV2N6GG");

// Assign channel id
// The test device should have 2 channel
// the first channel should be "Controller" - "On/Off"
// the secord channel should be "Display" - "On/Off"
MCSControllerOnOff led("led1");
MCSDisplayOnOff    led_disp("led1_disp");
MCSControllerOnOff relay1("relay1");
MCSDisplayOnOff    relay1_disp("disp_relay1");
MCSControllerOnOff relay2("relay2");
MCSDisplayOnOff    relay2_disp("disp_relay2");
MCSControllerOnOff relay3("relay3");
MCSDisplayOnOff    relay3_disp("disp_relay3");

MCSDisplayFloat    temp_disp("Temperature");
MCSDisplayFloat    humi_disp("Humidity");

#define LED_PIN       7
#define DHTPIN        3     // what pin we're connected to
#define RELAY1_PIN    4
#define RELAY2_PIN    12
#define RELAY3_PIN    13


DHT dht(DHTPIN, DHTTYPE);

int count = 0;
int status = WL_IDLE_STATUS;

void setup() {
  // setup Serial output at 9600
  Serial.begin(9600);

  // setup LED/Button pin
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  Serial.println("DHT22 test!");
  dht.begin();

  // setup Wifi connection
  //while(WL_CONNECTED != WiFi.status())
  while(status != WL_CONNECTED)
  {
    Serial.print("WiFi.begin(");
    Serial.print(ssid);
    Serial.print(",");
    Serial.print(key);
    Serial.println(")...");
    status = WiFi.begin(ssid,key);
    delay(5000);
  }
  Serial.println("WiFi connected !!");
  IPAddress myAddress = WiFi.localIP();
  Serial.println(myAddress);


  // setup MCS connection
  mcs.addChannel(led);
  mcs.addChannel(led_disp);
  mcs.addChannel(relay1);
  mcs.addChannel(relay1_disp);
  mcs.addChannel(relay2);
  mcs.addChannel(relay2_disp);
  mcs.addChannel(relay3);
  mcs.addChannel(relay3_disp);

  mcs.addChannel(temp_disp);
  mcs.addChannel(humi_disp);

  while(!mcs.connected())
  {
    Serial.println("MCS.connect()...");
    mcs.connect();
  }
  Serial.println("MCS connected !!");

  // read LED value from MCS server
  while(!led.valid())
  {
    Serial.println("read LED value from MCS...");
    led.value();
  }
  Serial.print("done, LED value = ");
  Serial.println(led.value());
  digitalWrite(LED_PIN, led.value() ? HIGH : LOW);

  // read Relay value from MCS server
  while(!relay1.valid())
  {
    Serial.println("read Relay 1 value from MCS...");
    relay1.value();
  }
  Serial.print("done, Relay 1 value = ");
  Serial.println(relay1.value());
  digitalWrite(RELAY1_PIN, relay1.value() ? LOW : HIGH);

  while(!relay2.valid())
  {
    Serial.println("read Relay 2 value from MCS...");
    relay2.value();
  }
  Serial.print("done, Relay2 value = ");
  Serial.println(relay2.value());
  digitalWrite(RELAY2_PIN, relay2.value() ? LOW : HIGH);

  while(!relay3.valid())
  {
    Serial.println("read Relay3 value from MCS...");
    relay3.value();
  }
  Serial.print("done, Relay3 value = ");
  Serial.println(relay3.value());
  digitalWrite(RELAY3_PIN, relay3.value() ? LOW : HIGH);


}

void loop() {
  // call process() to allow background processing, add timeout to avoid high cpu usage
  //Serial.print("process(");
  //Serial.print(millis());
  //Serial.println(")");
  mcs.process(100);
  count++;
  if(count >20)
  {
    count = 0;
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h))
    {
        Serial.println("Failed to read from DHT");
    }
    else
    {
        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.println(" *C");
        temp_disp.set(t);
        humi_disp.set(h);
    }
  }

  // updated flag will be cleared in process(), user must check it after process() call.
  if(led.updated())
  {
    Serial.print("LED updated, new value = ");
    Serial.println(led.value());
    digitalWrite(LED_PIN, led.value() ? HIGH : LOW);
    if(!led_disp.set(led.value()))
    {
      Serial.print("Failed to update led_disp");
      Serial.println(led_disp.value());
    }
  }

 // updated flag will be cleared in process(), user must check it after process() call.
  if(relay1.updated())
  {
    Serial.print("Relay 1 updated, new value = ");
    Serial.println(relay1.value());
    digitalWrite(RELAY1_PIN, relay1.value() ? LOW : HIGH);
    if(!relay1_disp.set(relay1.value()))
    {
      Serial.print("Failed to update relay1_disp");
      Serial.println(relay1_disp.value());
    }
  }

  if(relay2.updated())
  {
    Serial.print("Relay 2 updated, new value = ");
    Serial.println(relay2.value());
    digitalWrite(RELAY2_PIN, relay2.value() ? LOW : HIGH);
    if(!relay2_disp.set(relay2.value()))
    {
      Serial.print("Failed to update relay2_disp");
      Serial.println(relay2_disp.value());
    }
  }

  if(relay3.updated())
  {
    Serial.print("Relay 3 updated, new value = ");
    Serial.println(relay3.value());
    digitalWrite(RELAY3_PIN, relay3.value() ? LOW : HIGH);
    if(!relay3_disp.set(relay3.value()))
    {
      Serial.print("Failed to update relay3_disp");
      Serial.println(relay3_disp.value());
    }
  }


  // check if need to re-connect
  while(!mcs.connected())
  {
    Serial.println("re-connect to MCS...");
    mcs.connect();
    if(mcs.connected())
      Serial.println("MCS connected !!");
  }
}

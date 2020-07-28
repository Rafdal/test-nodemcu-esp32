#define DEBUG(String) Serial.println(F(String)); // Using debug print (You don't need the ';' at the end)
#ifndef DEBUG
#define DEBUG(String) // Debug print isn't used
#endif

#include <Arduino.h>
#include <EEPROM_emulator.h>
#include <eeprom_cleaning.h>
#include <LoRa.h>
#include <crc16.h>
#include <FastCRC.h>
#include <private_data.h> // Aqui tengo passwords de red

#include <WiFi.h>
#include <HTTPClient.h>
const char *host = "192.168.1.114"; 
const int port = 3000;
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;
// #include <google-get-request.h>

#define USE_SERIAL Serial


#include <OneButton.h>
OneButton btn(4, true);


#include <master/master.h>
Master device;

#include <mainWebSocket.h>

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	for (uint8_t i = 0; i < 10; i++)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(20);
		digitalWrite(LED_BUILTIN, LOW);
		delay(50);
	}
	Serial.begin(576000);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	Serial.println(F("Connecting"));
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.print(F("\nConnected to WiFi network with IP Address: "));
	Serial.println(WiFi.localIP());

	lastTime = millis();

	// @ WebSockets
	webSocketSetup();

	// @ Master
	device.begin();
	btn.setPressTicks(2000);

	Make_Sure_EEPROM_is_Clear(158, 76);

	// Loop per second
	ctrl.setLoopPerSec([](){
		device.runPerSec();
	});

	// Button Events
	btn.attachLongPressStart([](){
		DEBUG("LongPress")
		device.LongPress();
	});

	btn.attachDoubleClick([](){
		DEBUG("DoubleClick")
		device.DoubleClick();

		device.network.printClients();
	});

	btn.attachClick([](){
		DEBUG("Click")
		device.Click();

		packet_t p;
		p.set(3, type.data.setState, 2);
		lora.send(p);
	});
}

void loop()
{
	device.run();
	ctrl.run();
	btn.tick();
}
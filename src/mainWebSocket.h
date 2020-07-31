#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#include <SocketIoClient.h>
SocketIoClient webSocket;

TaskHandle_t TaskWebSocket;

void webSocketSetup()
{
    // % WebSockets
	webSocket.on("module:control", [](const char * payload, size_t length){

		device.network.setState(3, 2);

	});
	webSocket.on("connect", [](const char * payload, size_t length){
        DEBUG("CONNECTED")
        StaticJsonDocument<64> json;
        String jsonStr;
        jsonStr.reserve(64);

        json["mac"] = WiFi.macAddress();
        json["sdk"] = ESP.getSdkVersion();
        json["chip"] = ESP.getChipRevision();

        serializeJson(json, jsonStr);
        jsonStr.replace("\"", "\\\"");
        jsonStr = '"' + jsonStr + '"';

        webSocket.emit("module:connect", jsonStr.c_str());
    });

    webSocket.on("disconnect", [](const char * payload, size_t length){
        DEBUG("HEY DISCONECTADOU")
    });

    webSocket.on("chat:mensaje", [](const char * payload, size_t length){
        // ! Expresiones lambda en C++
        Serial.printf("Recibido payload: %s\n", payload);
    });

    // $ WebSocket Begin
    webSocket.begin(host, port);


    xTaskCreatePinnedToCore(
        [](void * parameter){
            for(;;)
            {
                webSocket.loop();
                // disableCore1WDT();
                // enableCore1WDT();
                disableCore0WDT();
                enableCore0WDT();
            }
        }, /* Function to implement the task */
        "WebSocketTask", /* Name of the task */
        10000,  /* Stack size in words */
        NULL,  /* Task input parameter */
        1,  /* Priority of the task */
        &TaskWebSocket  /* Task handle. */
        ,0  /* Core where the task should run */
    ); 
}
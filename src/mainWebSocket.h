#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#include <SocketIoClient.h>
SocketIoClient webSocket;

TaskHandle_t TaskWebSocket;

void webSocketSetup()
{
    xTaskCreatePinnedToCore(
        [](void * parameter){

            webSocket.on("connect", [](const char * payload, size_t length){
                DEBUG("CONECTADOUUUU")
            });

            webSocket.on("disconnect", [](const char * payload, size_t length){
                DEBUG("HEY DISCONECTADOU")
            });

            webSocket.on("chat:escribiendo", [](const char * payload, size_t length){
                // ! Expresiones lambda en C++
                Serial.printf("Recibido payload: %s\n", payload);
            });

            webSocket.on("module:control", [](const char * payload, size_t length){
                DEBUG("CONTROL")
                device.Click();
                packet_t p;
                p.set(3, type.data.setState, 2);
                lora.send(p);
                DEBUG("SENT")
            });

            // $ WebSocket Begin
            webSocket.begin(host, port);

            for(;;)
            {
                webSocket.loop();
                disableCore0WDT();
                enableCore0WDT();
            }
        }, /* Function to implement the task */
        "WebSocketTask", /* Name of the task */
        10000,  /* Stack size in words */
        NULL,  /* Task input parameter */
        0,  /* Priority of the task */
        &TaskWebSocket,  /* Task handle. */
        0  /* Core where the task should run */
    ); 
}
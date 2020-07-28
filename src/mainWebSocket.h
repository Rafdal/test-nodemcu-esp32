#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#include <SocketIoClient.h>
SocketIoClient webSocket;

TaskHandle_t TaskWebSocket;

void webSocketSetup()
{
    // xTaskCreate()
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
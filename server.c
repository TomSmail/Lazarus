
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rpi_sensor_control.h"
#include "set_message.h"
#include "socket.h"
#include "ws.h"

int main(void) {

    // Update message in case any changes to data happenned since last strike
    convert_strikes_csv_to_json();

    // Register websocket handlers
    struct ws_events evs;
    evs.onopen = &onopen;
    evs.onclose = &onclose;
    evs.onmessage = &onmessage;

    // Prepare for sensing
    puts("Starting up RPi Thread.");
    pthread_t rpi_thread;
    pthread_create(&rpi_thread, NULL, rpi_poll, NULL);

    // This will block until the end of the server.
    puts("Starting the server.");
    ws_socket(&evs, 5678, 0, 1000);

    puts("Cleaning Up.");
    pthread_join(rpi_thread, NULL);

    return 0;
}

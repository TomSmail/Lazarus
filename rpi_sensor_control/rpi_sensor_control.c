#include "rpi_sensor_control.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <wiringPi.h>

// For south kensington demo: expect 6 second light sound delay.

// ASSUMPTION 1: no consecutive lightning strikes (within
// MAX_SOUND_DELAY_AFTER_LIGHT). otherwise, would use a stack of sound and light
// times, and would massively increase complexity of trilateration process.

// ASSUMPTION 2: speed of light is >> speed of sound. Not exact answer.

#include "sensor_poll.h"  // would be a radio library in practice.

#define MAX_DIST_FROM_LIGHTNING 10000  // metres
#define SAMPLE_DELAY 5                 // in milliseconds.
#define MAX_SOUND_DELAY_AFTER_LIGHT \
  (MAX_DIST_FROM_LIGHTNING /						\
   0.33) / 1000  // for 5000 metres, expect max. 15,150 millisecond delay.
// Divide by 1000 since now in seconds.
#define QTY_SENSOR_PAIRS 3
#define LIGHT_FLASH_THRESHOLD 7000

#define light_sensor_one 0
#define sound_sensor_one 1
#define light_sensor_two 2
#define sound_sensor_two 4
#define light_sensor_three 3
#define sound_sensor_three 5

#define UNUSED(x) (void)x;

// Use wiringpi to empty the charge on the capacitor
static int flatten_capacitor(int pin_to_circuit) {
    pinMode(pin_to_circuit, OUTPUT);
    digitalWrite(pin_to_circuit, LOW);
    return 1;
}

// returns true iff sound received.
// if received, sets time_in_milliseconds to clock when received.
static bool sound_received(time_t* clock_when_received,
                           int pin) {  // need some pin info here... ) {
    bool received = digitalRead(pin);

    if (received) {
        *clock_when_received = time(0);
    }

    return received;
}

// Note pins are hard-coded
// Polls the sensors for light and sound detection for the demo.
void* rpi_poll(void* args) {
    UNUSED(args);

    // Index = sensor pair number. Pairs 0 to 2.
    int index_to_sound_map[] = {sound_sensor_one, sound_sensor_two,
                                sound_sensor_three};

    // setup wiring pi library.
    wiringPiSetup();

    // set input pins to input mode.
    pinMode(sound_sensor_one, INPUT);
    pinMode(sound_sensor_two, INPUT);
    pinMode(sound_sensor_three, INPUT);

    // main loop
    // clock times at which light received.
    time_t light_cell_activation_time[3] = {0, 0, 0};

    // done is never set to true.
    bool done = false;
    while (!done) {
        // Update cell activation times for ALL CELLS.

        // flatten capacitors.
        flatten_capacitor(light_sensor_one);
        flatten_capacitor(light_sensor_two);
        flatten_capacitor(light_sensor_three);

        // wait for 0.1 seconds to flatten.
        delay(100);  // = 0.1 seconds

        // set all input light pins to read.
        pinMode(light_sensor_one, INPUT);
        pinMode(light_sensor_two, INPUT);
        pinMode(light_sensor_three, INPUT);

        // read time to charge for each.
        int count[QTY_SENSOR_PAIRS] = {0, 0, 0};
        bool all_capacitors_charged = false;
        while (!all_capacitors_charged) {
            all_capacitors_charged = digitalRead(light_sensor_one) == HIGH &&
                                     digitalRead(light_sensor_two) == HIGH &&
                                     digitalRead(light_sensor_three) == HIGH;
            if (digitalRead(light_sensor_one) == LOW) {
                count[0]++;
            }
            if (digitalRead(light_sensor_two) == LOW) {
                count[1]++;
            }
            if (digitalRead(light_sensor_three) == LOW) {
                count[2]++;
            }
        }

        // check the three pin pairs in order.
        for (int i = 0; i < QTY_SENSOR_PAIRS; i++) {
            // use time counts to charge to determine whether light is incident
            // (LDR activated)
            if (count[i] < LIGHT_FLASH_THRESHOLD) {
                // lightning detected in this loop.
                light_cell_activation_time[i] = time(0);
                printf("Light detected on %d\n", i);
            }
            // If: waiting for sound from a recent strike's light (or has just
            // detected light)...
            bool within_max_delay_after_light_received =
                time(0) <
	        light_cell_activation_time[i] + MAX_SOUND_DELAY_AFTER_LIGHT;
            // takes most recent lightning strike.

            if (within_max_delay_after_light_received) {  // max time not
                                                          // exceeded.

                // Poll for sound.
                int sound_pin = index_to_sound_map[i];
                time_t sound_time = 0;

                if (sound_received(&sound_time, sound_pin)) {
                    // report to server.
		    double time_delay = ( (double) sound_time ) - ( (double) light_cell_activation_time[i] );
                    printf(
                        "SOUND RECEIVED FOR LIGHT on sensor %d, reporting. Sound time: %ld, Light time: %ld, DELAY: %lf \n",
                        i, sound_time, light_cell_activation_time[i], time_delay);
                    report_sound_to_server(
                        i, time_delay);
                }
            }
        }
        // wait at least SAMPLE_DELAY milliseconds before repolling.
        delay(SAMPLE_DELAY);
    }
    return NULL;
}

#include "sensor_poll.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "strike.h"

#define MAX_DELTA_BETWEEN_STATIONS 20  // in seconds

double lat_array[] = {51.51741, 51.49016, 51.48834};
double lon_array[] = {-0.17487, -0.20087, -0.14814};
double time_deltas[] = {0, 0, 0};
int times_occur[] = {0, 0, 0};

// Processes data from the raspberry pi and sends it through to the 
//    server code that will alert the frontend.
void report_sound_to_server(int sensor_idx, double delay) {
    times_occur[sensor_idx] = time(0);
    time_deltas[sensor_idx] = delay;

    if (time(0) - times_occur[0] < MAX_DELTA_BETWEEN_STATIONS &&
        time(0) - times_occur[1] < MAX_DELTA_BETWEEN_STATIONS &&
        times_occur[2] < MAX_DELTA_BETWEEN_STATIONS) {
        // found a strike detected by all
        times_occur[0] = 0;
        times_occur[1] = 0;
        times_occur[2] = 0;

        StationRaw station1 = {
            .station = {.lat = lat_array[0], .lon = lon_array[0]},
            .delay = time_deltas[0]};

        StationRaw station2 = {
            .station = {.lat = lat_array[1], .lon = lon_array[1]},
            .delay = time_deltas[1]};
        StationRaw station3 = {
            .station = {.lat = lat_array[2], .lon = lon_array[2]},
            .delay = time_deltas[2]};

        printf("Delay: %lf", delay);
        process_strike(&station1, &station2, &station3,
                       time(0));  // get that sent off
    }
}

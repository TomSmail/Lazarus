#include "sensor_poll.h"

#include <stdio.h>
#include <stdbool.h>
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
    time_t time_at_report = time(0);
    times_occur[sensor_idx] = time(0);
    time_deltas[sensor_idx] = delay;
    bool stations_all_received_signal_within_max
      = time_at_report - times_occur[0] < MAX_DELTA_BETWEEN_STATIONS &&
        time_at_report - times_occur[1] < MAX_DELTA_BETWEEN_STATIONS &&
        time_at_report - times_occur[2] < MAX_DELTA_BETWEEN_STATIONS;

    if (stations_all_received_signal_within_max) {
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

	// ERROR IS HERE. Calls process_strike without all 3 sensors having procced.
	// Probably a separate error causing the seg fault though...
	
        printf("Reporting with time_deltas[0]: %lf, time_deltas[1]: %lf, time_deltas[2]: %lf\n", time_deltas[0], time_deltas[1], time_deltas[2]);
        process_strike(&station1, &station2, &station3,
                       time(0));  // get that sent off
    }
}

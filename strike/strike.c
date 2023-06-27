#include "strike.h"

#include <stdio.h>
#include <string.h>

#include "calculate.h"
#include "messenger.h"
#include "set_message.h"
#include "socket.h"
#include "types.h"

static const unsigned int MAX_LINE_LENGTH = 1000;
static unsigned int max_id = 0;

// Backend processing to move a strike to the archive when it has been checked
// by the fire brigade.
void checkoff_strike(const char* strike_id) {
    FILE* strikes_file = fopen("strike/strikes.csv", "r");
    FILE* new_strikes_file = fopen("strike/strikes_temp.csv", "w");
    FILE* archive_file = fopen("strike/archive.csv", "a");

    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), strikes_file)) {
        char* id = strtok(line, ",");
        if (strike_id == id) {  // need to archive
            fputs(line, archive_file);
        } else {  // need to keep
            fputs(line, new_strikes_file);
        }
    }

    fclose(archive_file);
    fclose(strikes_file);
    fclose(new_strikes_file);

    rename("strikes_temp.csv", "strike/strikes.csv");

    // update json to csv
    convert_strikes_csv_to_json();
}

// Updates CSV file to add strike data
void add_strike_to_csv(StrikeData* strike_data) {
    FILE* strikes_file = fopen("strike/strikes.csv", "a");

    fprintf(strikes_file, "%d,", strike_data->strike_id);
    fprintf(strikes_file, "%lld,", strike_data->timestamp);

    fprintf(strikes_file, "%lf,", strike_data->strike_location.lat);
    fprintf(strikes_file, "%lf,", strike_data->strike_location.lon);

    fprintf(strikes_file, "%lf,", strike_data->c1.centre.lat);
    fprintf(strikes_file, "%lf,", strike_data->c1.centre.lon);
    fprintf(strikes_file, "%lf,", strike_data->c1.radius);

    fprintf(strikes_file, "%lf,", strike_data->c2.centre.lat);
    fprintf(strikes_file, "%lf,", strike_data->c2.centre.lon);
    fprintf(strikes_file, "%lf,", strike_data->c2.radius);

    fprintf(strikes_file, "%lf,", strike_data->c3.centre.lat);
    fprintf(strikes_file, "%lf,", strike_data->c3.centre.lon);
    fprintf(strikes_file, "%lf\n", strike_data->c3.radius);

    fclose(strikes_file);
}

// Everything the server needs to do to process a lightning strike
void process_strike(StationRaw* station1, StationRaw* station2,
                    StationRaw* station3, time_t time_received) {
    // Compute distances
    StrikeData strike_data;
    strike_data.strike_id = max_id++;
    strike_data.timestamp =
        (long long)time_received;  // TODO: convert time_t to long long

    strike_data.c1.centre = station1->station;
    strike_data.c1.radius = distance_to_lightning(station1->delay);
    strike_data.c2.centre = station2->station;
    strike_data.c2.radius = distance_to_lightning(station2->delay);
    strike_data.c3.centre = station3->station;
    strike_data.c3.radius = distance_to_lightning(station3->delay);

    // Compute intersection via tralaturation
    get_strike_location(&strike_data);

    // Write to CSV
    add_strike_to_csv(&strike_data);

    // sets prev_strikes.json
    convert_strikes_csv_to_json();

    // Write to single strike JSON
    set_single_strike(&strike_data);

    // Inform by text of the strike
    char msg_buffer[300];
    sprintf(msg_buffer, "Alert: Strike Reported at Lat: %lf, Lon: %lf ",
            strike_data.strike_location.lat, strike_data.strike_location.lon);
    send_text_message(msg_buffer);

    // Inform all listeners of the strike
    broadcast_by_name("single_strike.json");
}

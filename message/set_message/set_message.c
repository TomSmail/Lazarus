
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

const int MAX_LINE_SIZE = 1000;

// Convert from the concise archiveable backend format of CSV for strike
// data to JSON that can be passed to the frontend  
void convert_strikes_csv_to_json(void) {
    FILE *strikes_csv = fopen("extension/strike/strikes.csv", "r");
    FILE *strikes_json = fopen("extension/api_messages/prev_strikes.json", "w");
    char *strike_data_str = malloc(MAX_LINE_SIZE);

    fgets(strike_data_str, MAX_LINE_SIZE, strikes_csv);  // skips titles

    fputs("{", strikes_json);
    fputs("\n", strikes_json);
    fputs("\"command\": \"prev-strikes\",", strikes_json);
    fputs("\n", strikes_json);
    fputs("\"strikes\": [", strikes_json);
    fputs("\n", strikes_json);
    bool not_EOF = true;

    not_EOF = fgets(strike_data_str, MAX_LINE_SIZE, strikes_csv);
    while (not_EOF) {
        fputs("{", strikes_json);
        fputs("\n", strikes_json);

        fprintf(strikes_json, "\"strike_id\": %s,\n",
                strtok(strike_data_str, ","));
        fprintf(strikes_json, "\"timestamp\": %s,\n", strtok(NULL, ","));

        fprintf(strikes_json, "\"lat\": %s,\n", strtok(NULL, ","));
        fprintf(strikes_json, "\"lon\": %s,\n", strtok(NULL, ","));

        fprintf(strikes_json, "\"centre_lat_1\": %s,\n", strtok(NULL, ","));
        fprintf(strikes_json, "\"centre_lon_1\": %s,\n", strtok(NULL, ","));
        fprintf(strikes_json, "\"radius_1\": %s,\n", strtok(NULL, ","));

        fprintf(strikes_json, "\"centre_lat_2\": %s,\n", strtok(NULL, ","));
        fprintf(strikes_json, "\"centre_lon_2\": %s,\n", strtok(NULL, ","));
        fprintf(strikes_json, "\"radius_2\": %s,\n", strtok(NULL, ","));

        fprintf(strikes_json, "\"centre_lat_3\": %s,\n", strtok(NULL, ","));
        fprintf(strikes_json, "\"centre_lon_3\": %s,\n", strtok(NULL, ","));
        fprintf(strikes_json, "\"radius_3\": %s\n", strtok(NULL, ","));

        not_EOF = fgets(strike_data_str, MAX_LINE_SIZE, strikes_csv);
        // end of file, so no comma
        if (!not_EOF) {
            fputs("}", strikes_json);
            fputs("\n", strikes_json);
        } else {
            fputs("},", strikes_json);
            fputs("\n", strikes_json);
        }
    }
    fputs("]", strikes_json);
    fputs("\n", strikes_json);
    fputs("}", strikes_json);
    fclose(strikes_csv);
    fclose(strikes_json);
}

// Given a new strike, write it to the JSON file.
void write_strike_data_to_file(FILE *file_name, StrikeData *strike_data) {
    fprintf(file_name, "\"strike_id\": %d,\n", strike_data->strike_id);
    fprintf(file_name, "\"timestamp\": %lld,\n", strike_data->timestamp);

    fprintf(file_name, "\"lat\": %lf,\n", strike_data->strike_location.lat);
    fprintf(file_name, "\"lon\": %lf,\n", strike_data->strike_location.lon);

    fprintf(file_name, "\"radius_1\": %lf,\n", strike_data->c1.radius);
    fprintf(file_name, "\"centre_lat_2\": %lf,\n", strike_data->c1.centre.lat);
    fprintf(file_name, "\"centre_lon_2\": %lf,\n", strike_data->c1.centre.lon);

    fprintf(file_name, "\"radius_2\": %lf,\n", strike_data->c2.radius);
    fprintf(file_name, "\"centre_lat_3\": %lf,\n", strike_data->c2.centre.lat);
    fprintf(file_name, "\"centre_lon_3\": %lf,\n", strike_data->c2.centre.lon);

    fprintf(file_name, "\"radius_3\": %lf,\n", strike_data->c3.radius);
    fprintf(file_name, "\"centre_lat_3\": %lf,\n", strike_data->c3.centre.lat);
    fprintf(file_name, "\"centre_lon_3\": %lf\n", strike_data->c3.centre.lon);
}

// Set up the single strike json file for data output from write_strike_data_to_file  
void set_single_strike(StrikeData *strike_data) {
    FILE *single_strike_file =
        fopen("extension/api_messages/single_strike.json", "w");

    fputs("{", single_strike_file);
    fputs("\n", single_strike_file);
    fputs("\"command\": \"add\",", single_strike_file);
    fputs("\n", single_strike_file);
    fputs("\"strike\": {", single_strike_file);
    fputs("\n", single_strike_file);

    write_strike_data_to_file(single_strike_file, strike_data);

    fputs("}", single_strike_file);
    fputs("\n", single_strike_file);
    fputs("}", single_strike_file);
    fclose(single_strike_file);
}

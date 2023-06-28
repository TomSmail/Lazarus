#include "types.h"

#include <math.h>
#include <stdio.h>

double lat_lon_to_dist(double x) {
    return (111139.0 * x);
}

double dist_to_lat_lon(double x) {
    return (x / 111139.0);
}

const double LIGHT_SPEED = 299792458;  // ms^-1
const double SOUND_SPEED = 343;        // ms^-1
const double RADIUS_EARTH = 6378139;

const double ORIGIN_LAT = 51.471557;
const double ORIGIN_LON = -0.243153;

// Uses the difference in speed between sound and light to find the distance
// to a lightning strike from lightning and thunder data.
double distance_to_lightning(double time_delta) {
    return ((time_delta) / (1.0 / SOUND_SPEED - 1.0 / LIGHT_SPEED));
}

// Uses the algorithm detailled here:
// https://www.101computing.net/cell-phone-trilateration-algorithm/
//   to deduce the location of the thunderclap given the distances to each of
//   the
void get_strike_location(StrikeData *strike_data) {
    double x1 = lat_lon_to_dist((strike_data->c1).centre.lon - ORIGIN_LON);
    double y1 = lat_lon_to_dist((strike_data->c1).centre.lat - ORIGIN_LAT);
    double r1 = (strike_data->c1).radius;

    double x2 = lat_lon_to_dist((strike_data->c2).centre.lon - ORIGIN_LON);
    double y2 = lat_lon_to_dist((strike_data->c2).centre.lat - ORIGIN_LAT);
    double r2 = (strike_data->c2).radius;

    double x3 = lat_lon_to_dist((strike_data->c3).centre.lon - ORIGIN_LON);
    double y3 = lat_lon_to_dist((strike_data->c3).centre.lat - ORIGIN_LAT);
    double r3 = (strike_data->c3).radius;

    double A = -2 * x1 + 2 * x2;
    double B = -2 * y1 + 2 * y2;
    double C = r1 * r1 - r2 * r2 - x1 * x1 + x2 * x2 - y1 * y1 + y2 * y2;
    double D = -2 * x2 + 2 * x3;
    double E = -2 * y2 + 2 * y3;
    double F = r2 * r2 - r3 * r3 - x2 * x2 + x3 * x3 - y2 * y2 + y3 * y3;

    double xd =  (C*E - F*B) / (E*A - B*D); // (C * D - A * F) / (B * D - A * E); 
    double yd =  (C*D - A*F) / (B*D - A*E); // (C * E - F * B) / (E * A - B * D);

    printf("%lf", (strike_data->c1).centre.lon);
    printf("%lf, %lf, %lf, %lf, %lf, %lf", x1, x2, x3, y1, y2, y3);
    printf("xd: %lf, yd: %lf", xd, yd);

    (strike_data->strike_location).lat = ORIGIN_LAT + dist_to_lat_lon(yd);
    (strike_data->strike_location).lon = ORIGIN_LON + dist_to_lat_lon(xd);
}

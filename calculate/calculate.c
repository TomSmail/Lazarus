#include "types.h"

const double LIGHT_SPEED = 299792458;  // ms^-1
const double SOUND_SPEED = 343;        // ms^-1

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
    double x1 = (strike_data->c1).centre.lat;
    double y1 = (strike_data->c1).centre.lon;
    double r1 = (strike_data->c1).radius;

    double x2 = (strike_data->c2).centre.lat;
    double y2 = (strike_data->c2).centre.lon;
    double r2 = (strike_data->c2).radius;

    double x3 = (strike_data->c3).centre.lat;
    double y3 = (strike_data->c3).centre.lon;
    double r3 = (strike_data->c3).radius;

    double A = -2 * x1 + 2 * x2;
    double B = -2 * y1 + 2 * y2;
    double C = r1 * r1 - r2 * r2 - x1 * x1 + x2 * x2 - y1 * y1 + y2 * y2;
    double D = -2 * x2 + 2 * x3;
    double E = -2 * y2 + 2 * y3;
    double F = r2 * r2 - r3 * r3 - x2 * x2 + x3 * x3 - y2 * y2 + y3 * y3;
    (strike_data->strike_location).lat = (C * E - F * B) / (E * A - B * D);
    (strike_data->strike_location).lon = (C * D - A * F) / (B * D - A * E);
}

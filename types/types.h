#ifndef TYPES_H
#define TYPES_H

typedef struct {
    double lat;
    double lon;
} Point;

typedef struct {
    Point centre;
    double radius;
} Circle;

typedef struct {
    Point station;
    double delay;
} StationRaw;

typedef struct {
    int strike_id;
    long long timestamp;
    Point strike_location;
    Circle c1;
    Circle c2;
    Circle c3;
} StrikeData;

#endif
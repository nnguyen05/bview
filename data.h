#ifndef __DATA_H__
#define __DATA_H__

#include "csv.h"

typedef struct {
	int number; // stop_code
	char name[256]; // stop_name
} stop_t;

typedef struct {
	int number; // trip_id
	char arrival[32]; // arrival_time
	char departure[32]; // departure_time
	int stop; // stop_id
} stoptime_t;

typedef struct {
	int route; // route_id
	char headsign[32]; // trip_headsign
	int stop; // stop_id
} trip_t;

void get_stops(csv_t);

#endif

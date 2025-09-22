#include "data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void get_stops(csv_t csv){
	row_t header = csv.rows[0];
	int iCode = -1, iName = -1;
	for(int i = header.cColumn-1; i >= 0; --i){
		char* field = header.columns[i];
		if(iCode == -1 && !strcmp(field, "stop_code")){
			iCode = i;
			continue;
		}
		if(iName == -1 && !strcmp(field, "stop_name")){
			iName = i;
			continue;
		}
	}
	for(size_t i = 1; i < csv.cRow; i++){
		// TODO: move this to heap?
		stop_t stop;
		stop.number = strtol(csv.rows[i].columns[iCode], NULL, 0);
		memset(stop.name, 0, sizeof(stop.name));
		strncpy(stop.name, csv.rows[i].columns[iName], sizeof(stop.name));


		printf("%d: %s\n", stop.number, stop.name);
	}
}

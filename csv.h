#ifndef __CSV_H__
#define __CSV_H__

#include <stddef.h>

typedef struct {
	char** lines;
	size_t cLine;
} lines_t;

typedef struct {
	char** columns;
	size_t cColumn;
} row_t;

typedef struct {
	lines_t data;
	row_t* rows;
	size_t cRow;
} csv_t;

int read_csv(csv_t*, const char*);
void free_csv(csv_t*);

#endif

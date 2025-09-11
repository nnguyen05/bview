#include "csv.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 256

#define RESIZE(cursor, start, size, type) \
if(cursor >= (start + size)){ \
	ptrdiff_t diff = cursor - start; \
	size += BUFFER_SIZE; \
	start = realloc(start, size*sizeof(type)); \
	cursor = start + diff; \
} \

typedef struct {
	char** lines;
	size_t cLine;
} lines_t;

typedef struct {
	char** columns;
	size_t cColumn;
} row_t;

typedef struct {
	row_t* rows;
	size_t cRow;
} csv_t;

size_t getlines(const char* path, lines_t* lines){
	FILE* fFile = fopen(path, "rb");
	int iChar = 0;
	if(fFile == NULL) return -1;
	
	size_t cLines = BUFFER_SIZE;
	lines->cLine = 0;
	lines->lines = (char**)calloc(cLines, sizeof(char*));
	char** ppLines = lines->lines;
	
	size_t cLine = BUFFER_SIZE;
	char* szLine = (char*)calloc(cLine, sizeof(char));
	char* pchLine = szLine;
	size_t cbBuffer = 0;

	int bOopsie = 0; // catch extraneous allocation
	for(;;){
		iChar = fgetc(fFile);
		if(iChar == EOF) break;
		// ignore marks of the beast
		if(iChar == '\r') continue;

		RESIZE(pchLine, szLine, cLine, char);
		*pchLine = iChar;

		if(iChar == '\n'){
			szLine = realloc(szLine, cbBuffer + 1);
			szLine[cbBuffer] = 0;
			
			RESIZE(ppLines, lines->lines, cLines, char*);
			*(ppLines++) = szLine;

			lines->cLine++;

			cLine = BUFFER_SIZE;
			szLine = (char*)calloc(cLine, sizeof(char*));
			pchLine = szLine;
			cbBuffer = 0;
			bOopsie = 1;
		} else {
			bOopsie = 0;
			pchLine++;
			cbBuffer++;
		}
	}
	lines->lines = realloc(lines->lines, lines->cLine*sizeof(char*));
	fclose(fFile);
	if(bOopsie) free(szLine);

	return lines->cLine;
}

void freelines(lines_t* lines){
	for(size_t i = 0; i < lines->cLine; i++){
		free(lines->lines[i]);
		lines->lines[i] = NULL;
	}
	free(lines->lines);
	lines->lines = NULL;
}

size_t linestocsv(lines_t* lines, csv_t* csv){
	csv->cRow = lines->cLine;
	csv->rows = (row_t*)calloc(csv->cRow, sizeof(row_t));
	row_t* pRow = csv->rows;
	for(size_t i=0; i<lines->cLine; i++){
		char* line = lines->lines[i];
		char* start = line;
		size_t length = strlen(line) + 1;

		size_t cBufColumn = BUFFER_SIZE;
		pRow->cColumn = 0;
		pRow->columns = (char**)calloc(cBufColumn, sizeof(char*));

		char** pColumn = pRow->columns;

		for(int i=0; i<(int)length; i++){
			switch(line[i]){
				case ',':
				case '\0': {
					line[i] = '\0';
					RESIZE(pColumn, pRow->columns, cBufColumn, char*);
					
					*(pColumn++) = start;
					pRow->cColumn++;
					
					start = line + i + 1;
					break;
				}
			}
		}
		pRow->columns = realloc(pRow->columns, pRow->cColumn*sizeof(char*));

		pRow++;
	}
	return csv->cRow;
}

void freecsv(csv_t* csv){
	for(size_t i=0; i<csv->cRow; i++){
		free(csv->rows[i].columns);
		csv->rows[i].columns = NULL;
	}
	free(csv->rows);
	csv->rows = NULL;
}

int do_csv(const char* path){
	lines_t lines = {0};
	csv_t csv = {0};
	getlines(path, &lines);
	linestocsv(&lines, &csv);

	for(size_t i=0; i<csv.cRow; i++){
		row_t row = csv.rows[i];
		for(size_t j=0; j < row.cColumn; j++){
			printf("(%s), ", row.columns[j]);
		}
		printf("\n");
	}

	freelines(&lines);
	freecsv(&csv);
	return 0;
}


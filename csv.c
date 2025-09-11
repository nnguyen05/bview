#include "csv.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 256

typedef struct {
	char** lines;
	size_t count;
} lines_t;

size_t getlines(const char* path, lines_t* lines){
	FILE* fFile = fopen(path, "rb");
	int iChar = 0;
	if(fFile == NULL) return -1;
	
	size_t cLines = BUFFER_SIZE;
	lines->count = 0;
	lines->lines = (char**)calloc(cLines, sizeof(char*));
	char** ppLines = lines->lines;
	
	size_t cLine = BUFFER_SIZE;
	char* szLine = (char*)calloc(cLine, sizeof(char));
	char* pchLine = szLine;
	size_t cbBuffer = 0;
	for(;;){
		iChar = fgetc(fFile);
		if(iChar == EOF) break;
		// ignore marks of the beast
		if(iChar == '\r') continue;

		if(pchLine >= (szLine + cLine)){
			ptrdiff_t bLine = pchLine - szLine;
			cLine += BUFFER_SIZE;
			szLine = realloc(szLine, cLine*sizeof(char));
			pchLine = szLine + bLine;
		}
		*pchLine = iChar;

		if(iChar == '\n'){
			szLine = realloc(szLine, cbBuffer + 1);
			szLine[cbBuffer] = 0;
			
			if(ppLines >= (lines->lines + cLines)){
				ptrdiff_t bLines = ppLines - lines->lines;
				cLines += BUFFER_SIZE;
				lines->lines = realloc(lines->lines, cLines*sizeof(char*));
				ppLines = lines->lines + bLines;
			}
			*(ppLines++) = szLine;
			lines->count++;

			cLine = BUFFER_SIZE;
			szLine = (char*)calloc(cLine, sizeof(char*));
			pchLine = szLine;
			cbBuffer = 0;
		} else {
			pchLine++;
			cbBuffer++;
		}
	}
	lines->lines = realloc(lines->lines, lines->count*sizeof(char*));

	return lines->count;
}

void freelines(lines_t* lines){
	for(int i = 0; i < (int)lines->count; i++){
		free(lines->lines[i]);
		lines->lines[i] = NULL;
	}
	free(lines->lines);
	lines->lines = NULL;
}

int do_csv(const char* path){
	lines_t lines = {0};
	getlines(path, &lines);
	
	for(int i = 0; i < (int)lines.count; i++){
		printf("%s\n", lines.lines[i]);
	}

	freelines(&lines);
	return 0;
}


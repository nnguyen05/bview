#include "csv.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 256

int do_csv(const char* path){
	FILE* fFile = fopen(path, "rb");
	int iChar = 0;
	if(fFile == NULL) return -1;
	
	size_t cbLines = BUFFER_SIZE;
	char** pLines = (char**)malloc(cbLines);
	char** ppLines = pLines;
	
	size_t cbLine = BUFFER_SIZE;
	char* szLine = (char*)malloc(cbLine);
	char* pchLine = szLine;
	size_t cbBuffer = 0;
	for(;;){
		iChar = fgetc(fFile);
		if(iChar == EOF) break;
		// ignore marks of the beast
		if(iChar == '\r') continue;

		if(pchLine >= (szLine + cbLine)){
			ptrdiff_t bLine = pchLine - szLine;
			cbLine += BUFFER_SIZE;
			szLine = realloc(szLine, cbLine);
			pchLine = szLine + bLine;
		}

		*pchLine = iChar;
		if(iChar == '\n'){
			szLine = realloc(szLine, cbBuffer + 1);
			szLine[cbBuffer] = 0;
			
			if(ppLines >= (pLines + cbLines)){
				ptrdiff_t bLines = ppLines - pLines;
				cbLines += BUFFER_SIZE;
				pLines = realloc(pLines, cbLines);
				ppLines = pLines + bLines;
			}
			*(ppLines++) = szLine;

			// free(szLine);
			cbLine = BUFFER_SIZE;
			szLine = (char*)malloc(cbLine);
			pchLine = szLine;
			cbBuffer = 0;
		} else {
			pchLine++;
			cbBuffer++;
		}
	}

	return 0;
}

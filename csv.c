#include "csv.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 256

int do_csv(const char* path){
	FILE* fFile = fopen(path, "rb");
	int iChar = 0;
	if(fFile == NULL) return -1;

	
	size_t cbLine = BUFFER_SIZE;
	char* szLine = (char*)malloc(BUFFER_SIZE);
	char* pchLine = szLine;
	size_t cbBuffer = 0;
	for(;;){
		iChar = fgetc(fFile);
		if(iChar == EOF) break;
		if(pchLine >= (szLine + cbLine)){
			ptrdiff_t bLine = pchLine - szLine;
			cbLine = cbLine + BUFFER_SIZE;
			szLine = realloc(szLine, cbLine);
			pchLine = szLine + bLine;
		}
		*pchLine = iChar;
		if(iChar == '\n'){
			*pchLine = 0;
			szLine = realloc(szLine, cbBuffer + 1);
			printf("%s", szLine);

			free(szLine);
			cbLine = BUFFER_SIZE;
			szLine = (char*)malloc(BUFFER_SIZE);
			pchLine = szLine;
		}
		pchLine++;
		cbBuffer++;
	}

	return 0;
}

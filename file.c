#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/file.h>
#include <sys/wait.h>

#include <curl/curl.h>

int file_exists(const char* path){
	struct stat st = {0};
	stat(path, &st);
	return !(errno == ENOENT);
}

size_t write_gtfs(void* buffer, size_t size, size_t nmemb, void* userp){
	FILE* hFile = (FILE*)userp;
	fwrite(buffer, size * nmemb, 1, hFile);
	printf(".");
	return size * nmemb;
}
void download_gtfs(const char* url, const char* path){
	printf("Downloading");

	CURL* cCurl = curl_easy_init();
	FILE* fGtfs = fopen(path, "wb");
	curl_easy_setopt(cCurl, CURLOPT_URL, url);
	curl_easy_setopt(cCurl, CURLOPT_WRITEFUNCTION, write_gtfs);
	curl_easy_setopt(cCurl, CURLOPT_WRITEDATA, (void*)fGtfs);
	CURLcode ccRes = curl_easy_perform(cCurl);
	fclose(fGtfs);
	curl_easy_cleanup(cCurl);

	if(ccRes == CURLE_OK)
		printf("\n...done.\n");
	else
		printf("\n...error (CURLcode %d.)\n", ccRes);
}

int unzip_gtfs(const char* zip, const char* path){
	printf("Unpacking %s to %s...\n", zip, path);
	pid_t pid = fork();
	int iResult = 0;
	int exit_status = 0;

	if(pid == 0){
		// iResult = execlp("unzip", "unzip", "-o", zip, "-d", path, NULL);
		const char* 
		iResult = execve("unzip", , NULL);
	} else if(pid == -1){
		printf("I can't fork.\n");
		iResult = -1;
		return iResult;
	}
	waitpid(pid, &exit_status, 0);

	fflush(stdout);

	return iResult;
}

#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#include <curl/curl.h>

int file_exists(const char* root, const char* path){
	size_t cbPath = snprintf(NULL, 0, "%s/%s", root, path) + 1;
	char szPath[cbPath];
	snprintf(szPath, cbPath, "%s/%s", root, path);

	struct stat st = {0};
	stat(szPath, &st);
	return !(errno == ENOENT);
}

size_t write_gtfs(void* buffer, size_t size, size_t nmemb, void* userp){
	FILE* hFile = (FILE*)userp;
	fwrite(buffer, size * nmemb, 1, hFile);
	printf(".");
	return size * nmemb;
}

// dest, source
void download_gtfs(const char* path, const char* url, const char* root){
	size_t cbPath = snprintf(NULL, 0, "%s/%s", root, path) + 1;
	char szPath[cbPath];
	snprintf(szPath, cbPath, "%s/%s", root, path);

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

int unzip_gtfs(const char* path, const char* zip, const char* root){
	size_t cbPath = snprintf(NULL, 0, "%s/%s", root, path) + 1;
	size_t cbZip = snprintf(NULL, 0, "%s/%s", root, zip) + 1;
	char szPath[cbPath];
	char szZip[cbZip];
	snprintf(szPath, cbPath, "%s/%s", root, path);
	snprintf(szZip, cbZip, "%s/%s", root, zip);
	printf("Unpacking %s to %s...\n", szZip, szPath);
	pid_t pid = fork();
	switch(pid){
		case 0:
			execlp("unzip", "unzip", "-o", szZip, "-d", szPath, NULL);
			perror("unzip");
			return -1; // if execlp returns, it's broken
		case -1:
			perror("Can't fork");
			return -1;
		default: {
			int status = 0;
			waitpid(pid, &status, 0);
			fflush(stdout);
			return 0;
		}
	}
}

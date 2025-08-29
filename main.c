#include <stdio.h>

#include <curl/curl.h>

#include "main.h"
#include "file.h"
#include "csv.h"

int main(){
	// init curl
	curl_version_info_data* cvVersion = curl_version_info(CURLVERSION_NOW);
	printf("Using libcurl %s\n", cvVersion->version);
	curl_global_init(CURL_GLOBAL_DEFAULT);

	if(!file_exists(GTFS_FILE)){
		printf("GTFS data does not exist.\n");
		download_gtfs(GTFS_FILE, GTFS_URL);
	}
	if(unzip_gtfs(GTFS_FILE, GTFS_DIR) == -1){
		printf("unzip is not executable; is it installed?\n");
		goto cleanup;
	}
	printf("...done.\n");
	
	// TODO: format this to use GTFS_DIR
	do_csv("./data/stops.txt");

cleanup:
	// cleanup curl
	curl_global_cleanup();
	return 0;
}

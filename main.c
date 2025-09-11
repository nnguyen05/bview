#include <stdio.h>

#include <curl/curl.h>
#include <unistd.h>
#include <libgen.h>

#include "main.h"
#include "file.h"
#include "csv.h"

int main(int argc, char** argv){
	const char* szRoot = dirname(argv[0]);

	// init curl
	curl_version_info_data* cvVersion = curl_version_info(CURLVERSION_NOW);
	printf("Using libcurl %s\n", cvVersion->version);
	curl_global_init(CURL_GLOBAL_DEFAULT);

	if(!file_exists(szRoot, GTFS_FILE)){
		printf("GTFS data does not exist.\n");
		download_gtfs(GTFS_FILE, GTFS_URL, szRoot);
	}
	if(unzip_gtfs(GTFS_DIR, GTFS_FILE, szRoot) == -1){
		printf("unzip is not executable; is it installed?\n");
		goto cleanup;
	}
	printf("...done.\n");
{
	size_t cbPath = snprintf(NULL, 0, "%s/%s/%s", szRoot, GTFS_DIR, "stops.txt") + 1;
	char szPath[cbPath];
	snprintf(szPath, cbPath, "%s/%s/%s", szRoot, GTFS_DIR, "stops.txt");
	
	printf("Reading %s...\n", szPath);

	csv_t csvStops = {0};
	read_csv(&csvStops, szPath);
}
cleanup:
	// cleanup curl
	curl_global_cleanup();
	return 0;
}

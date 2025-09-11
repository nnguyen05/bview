#ifndef __FILE_H__
#define __FILE_H__

// General file/networking management functions

// In Linux, uses POSIX stat to check if a file exists.
int file_exists(const char*, const char*);

// Downloads from url to path.
// Uses libcurl, make sure to initialize it first.
void download_gtfs(const char*, const char*, const char*);

// Unzips zip to path.
// The implementation depends on
// the shell's ability to run the `unzip' command.
int unzip_gtfs(const char*, const char*, const char*);

#endif

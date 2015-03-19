#include "filesystem.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifndef S_ISREG
#ifndef S_IFREG
#ifndef _S_IFREG
#define S_IFREG		(-1)
#else
#define S_IFREG		_S_IFREG
#endif
#endif
#define S_ISREG(m)	(((m)&S_IFREG)==S_IFREG)
#endif

#ifndef S_ISDIR
#ifndef S_IFDIR
#ifndef _S_IFDIR
#define S_IFDIR		(-1)
#else
#define S_IFDIR		_S_IFDIR
#endif
#endif
#define S_ISDIR(m)	(((m)&S_IFDIR)==S_IFDIR)
#endif

// Checks whether a file or directory exists
int file_exists(char *filename)
{
	struct stat st;
	return (stat(filename, &st) == 0);
}

// Tells whether the filename is a directory
int is_dir(char *filename)
{
	struct stat st;
	if (stat(filename, &st) == 0) {
		return S_ISDIR(st.st_mode);
	}
	return 0;
}

// Tells whether the filename is a regular file
int is_file(char *filename)
{
	struct stat st;
	if (stat(filename, &st) == 0) {
		return S_ISREG(st.st_mode);
	}
	return 0;
}

#ifdef _WIN32
// Makes directory
int mkdir(const char *filename, unsigned short mode)
{
	errno = 0;

	if (CreateDirectory(filename, NULL) != 0) {
		return 0;
	} else {
		DWORD dw = GetLastError();
		switch (dw)
		{
			case ERROR_ACCESS_DENIED:
				errno = EACCES;
				break;
			case ERROR_ALREADY_EXISTS:
				errno = EEXIST;
				break;
			case ERROR_INVALID_NAME:
				if (strlen(filename) > MAX_PATH) {
					errno = ENAMETOOLONG;
				}
				break;
			case ERROR_PATH_NOT_FOUND:
				errno = ENOENT;
				break;
		}
		
		return -1;
	}
}
#endif
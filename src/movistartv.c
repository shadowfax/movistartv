#include "sockets/socket.h"
#include "appserver/mvtv.h"
#include "sockets/multicast.h"
#include "movistartv.h"
#include "kernel/filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char *movistartv_data_path = NULL;


int main(int argc, char * argv[])
{
	struct mvtv_client_profile *client_profile = NULL;
	struct mvtv_platform_profile *platform_profile = NULL;
#ifdef _WIN32
	WSADATA wsaData;
	
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		/* error */
		fprintf(stderr, "ERROR: WSAStartup failed. Code: %d\n", WSAGetLastError());
		return 1;
	}
#endif

	/* Set the data folder */
	if (movistartv_data_path == NULL) {
#ifdef _WIN32
		char temp_path[MAX_PATH];
		DWORD dwRetVal = GetTempPathA(MAX_PATH, temp_path);
		if (dwRetVal > MAX_PATH || (dwRetVal == 0)) {
			fprintf(stderr, "ERROR: unable to get windows temp path\n");
			WSACleanup();
			exit(1);
		}

		movistartv_data_path = (char *)malloc(strlen(&temp_path) + strlen("movistartv") + 1);
		memset(movistartv_data_path, 0, strlen(&temp_path) + strlen("movistartv") + 1);
		strcpy(movistartv_data_path, &temp_path);
		strcat(movistartv_data_path, "movistartv");
#else
		char *temp_path = NULL;

		temp_path = getenv("TEMP");
		if (temp_path == NULL) {
			temp_path = getenv("TMP");
		}

		if (temp_path == NULL) {
			movistartv_data_path = strdup("/tmp/movistartv");
		} else {
			movistartv_data_path = (char *)malloc(strlen(temp_path) + strlen("movistartv") + 1);
			memset(movistartv_data_path, 0, strlen(temp_path) + strlen("movistartv") + 1);
			strcpy(movistartv_data_path, temp_path);
			strcat(movistartv_data_path, "movistartv");
		}		
#endif
	}

	/* Create the data folder if it does not exist */
	if (!is_dir(movistartv_data_path)) {
		if (mkdir(movistartv_data_path, 0700) != 0) {
			fprintf(stderr, "ERROR: unable to create the directory %s", movistartv_data_path);
			free(movistartv_data_path);
			exit(1);
		}
	}

	/* Get the client profile */
	appserver_mvtv_get_client_profile(&client_profile);
	if (client_profile == NULL) {
		fprintf(stderr, "ERROR: Failed to get client profile\n");
#ifdef _WIN32
		WSACleanup();
#endif
		return 1;
	}

	/* Get the platform profile */
	appserver_mvtv_get_platform_profile(&platform_profile);
	if (platform_profile == NULL) {
		fprintf(stderr, "ERROR: Failed to get platform profile\n");
		mvtv_client_profile_free(client_profile);
#ifdef WIN32
		WSACleanup();
#endif
		return 1;
	}
	
	/* TODO: Do some stuff! */
	char *dvb_entry_point_addr = strtok(platform_profile->dvb_config->dvb_entry_point, ":");
	int dvb_entry_point_port = strtol(strtok(NULL, ":"), NULL, 10);

	char *xml_data = NULL;
	multicast_service_discovery(dvb_entry_point_addr, dvb_entry_point_port, 1, &xml_data);
	printf("%s", xml_data);

	if (xml_data) {
		free(xml_data);
	}
	
	/* Free memory */
	if (client_profile) {
		mvtv_client_profile_free(client_profile);
	}
	if (platform_profile) {
		mvtv_platform_profile_free(platform_profile);
	}

	if (movistartv_data_path) {
		free(movistartv_data_path);
	}
#ifdef WIN32
	WSACleanup();
#endif

#if defined(WIN32) && defined(_DEBUG)
	// Just so debugger doesn't exit right away
	_sleep(5000);
#endif
}
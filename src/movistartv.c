#include "sockets/socket.h"
#include "appserver/mvtv.h"
#include "sockets/multicast.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	struct mvtv_client_profile *client_profile = NULL;
	struct mvtv_platform_profile *platform_profile = NULL;
#ifdef WIN32
	WSADATA wsaData;
	
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		/* error */
		fprintf(stderr, "ERROR: WSAStartup failed. Code: %d\n", WSAGetLastError());
		return 1;
	}
#endif

	/* Get the client profile */
	appserver_mvtv_get_client_profile(&client_profile);
	if (client_profile == NULL) {
		fprintf(stderr, "ERROR: Failed to get client profile\n");
#ifdef WIN32
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

	multicast_service_discovery(dvb_entry_point_addr, dvb_entry_point_port);
	
	/* Free memory */
	if (client_profile) {
		mvtv_client_profile_free(client_profile);
	}
	if (platform_profile) {
		mvtv_platform_profile_free(platform_profile);
	}

#ifdef WIN32
	WSACleanup();
#endif

#if defined(WIN32) && defined(_DEBUG)
	// Just so debugger doesn't exit right away
	_sleep(5000);
#endif
}
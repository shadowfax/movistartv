#include "sockets/socket.h"
#include "appserver/mvtv.h"
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
	client_profile = appserver_mvtv_get_client_profile();
	if (client_profile == NULL) {
		fprintf(stderr, "ERROR: Failed to get client profile\n");
#ifdef WIN32
		WSACleanup();
#endif
		return 1;
	}

	/* Get the platform profile */
	platform_profile = appserver_mvtv_get_platform_profile();
	if (client_profile == NULL) {
		fprintf(stderr, "ERROR: Failed to get platform profile\n");
		mvtv_client_profile_free(client_profile);
#ifdef WIN32
		WSACleanup();
#endif
		return 1;
	}
	
	/* TODO: Do some stuff! */

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
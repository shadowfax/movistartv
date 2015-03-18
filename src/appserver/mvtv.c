#include "mvtv.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

struct mvtv_client_profile * appserver_mvtv_get_client_profile(void)
{
	struct mvtv_client_profile *client_profile;

	client_profile = malloc(sizeof(client_profile));

	mvtv_client_profile_free(client_profile);
	return NULL;
}

struct mvtv_platform_profile * appserver_mvtv_get_platform_profile(void)
{
	return NULL;
}

void mvtv_dvb_config_free(mvtv_dvb_config *dvb_config)
{
	if (dvb_config->dvb_service_provider) {
		free(dvb_config->dvb_service_provider);
	}

	if (dvb_config->dvb_entry_point) {
		free(dvb_config->dvb_entry_point);
	}

	free(dvb_config);
}

void mvtv_client_profile_free(mvtv_client_profile *client_profile)
{

	free(client_profile);
}

void mvtv_platform_profile_free(mvtv_platform_profile *platform_profile)
{
	if (platform_profile->dvb_config) {
		mvtv_dvb_config_free(platform_profile->dvb_config);
	}

	free(platform_profile);
}
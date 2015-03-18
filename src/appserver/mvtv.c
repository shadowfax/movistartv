#include "mvtv.h"
#include "http.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void appserver_mvtv_get_client_profile(struct mvtv_client_profile **profile)
{
	struct json_object *json_client_profile;
	struct json_object *json_result_data;
	struct json_object *t_obj;

	json_client_profile = http_fetch_json("http://172.26.22.23:2001/appserver/mvtv.do?action=getClientProfile");
	if (json_client_profile == NULL) {
		/* error */
		fprintf(stderr, "ERROR: Failed to get the client profile\n");
		return;
	}

	json_result_data = json_object_object_get(json_client_profile, "resultData");
	if (json_result_data == NULL) {
		/* error */
		fprintf(stderr, "ERROR: Failed to get the client profile\n");
		json_object_put(json_client_profile);
		return;
	}

	/* Allocate memory for the client profile */
	*profile = (struct mvtv_client_profile *)malloc(sizeof(struct mvtv_client_profile));

	t_obj = json_object_object_get(json_result_data, "version");
	(*profile)->version = json_object_get_int(t_obj);

	t_obj = json_object_object_get(json_result_data, "demarcation");
	(*profile)->demarcation = json_object_get_int(t_obj);

	json_object_put(json_client_profile);
}

void appserver_mvtv_get_platform_profile(struct mvtv_platform_profile **profile)
{
	struct json_object *json_platform_profile;
	struct json_object *json_result_data;
	struct json_object *t_obj;

	json_platform_profile = http_fetch_json("http://172.26.22.23:2001/appserver/mvtv.do?action=getPlatformProfile");
	if (json_platform_profile == NULL) {
		/* error */
		fprintf(stderr, "ERROR: Failed to get the platform profile\n");
		return;
	}

	json_result_data = json_object_object_get(json_platform_profile, "resultData");
	if (json_result_data == NULL) {
		/* error */
		fprintf(stderr, "ERROR: Failed to get the platform profile\n");
		json_object_put(json_platform_profile);
		return;
	}

	/* Allocate memory for the client profile */
	*profile = (struct mvtv_platform_profile *)malloc(sizeof(struct mvtv_platform_profile));

	t_obj = json_object_object_get(json_result_data, "dvbConfig");
	if (t_obj == NULL) {
		/* error */
		fprintf(stderr, "ERROR: missing dvbConfig\n");
		json_object_put(json_platform_profile);
		return;
	}
	
	(*profile)->dvb_config = (struct mvtv_dvb_config *)malloc(sizeof(struct mvtv_dvb_config));
	struct json_object *tt_obj = json_object_object_get(t_obj, "dvbServiceProvider");
	(*profile)->dvb_config->dvb_service_provider = strdup(json_object_get_string(tt_obj));
	tt_obj = json_object_object_get(t_obj, "dvbEntryPoint");
	(*profile)->dvb_config->dvb_entry_point = strdup(json_object_get_string(tt_obj));

	json_object_put(json_platform_profile);
}

void mvtv_dvb_config_free(struct mvtv_dvb_config *dvb_config)
{
	if (dvb_config->dvb_service_provider) {
		free(dvb_config->dvb_service_provider);
	}

	if (dvb_config->dvb_entry_point) {
		free(dvb_config->dvb_entry_point);
	}

	free(dvb_config);
}

void mvtv_client_profile_free(struct mvtv_client_profile* client_profile)
{
	free((void *)client_profile);
}

void mvtv_platform_profile_free(struct mvtv_platform_profile* platform_profile)
{
	if (platform_profile->dvb_config) {
		mvtv_dvb_config_free(platform_profile->dvb_config);
	}

	free(platform_profile);
}
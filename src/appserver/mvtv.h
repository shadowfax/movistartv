#ifndef MOVISTARTV_APPSERVER_MVTV_H
#define MOVISTARTV_APPSERVER_MVTV_H

struct mvtv_client_profile
{
	int version;
	int demarcation;
};

struct mvtv_dvb_config
{
	char * dvb_service_provider;
	char * dvb_entry_point;
};

struct mvtv_platform_profile
{
	struct mvtv_dvb_config *dvb_config;
};

/* Functions */
void appserver_mvtv_get_client_profile(struct mvtv_client_profile **profile);
void appserver_mvtv_get_platform_profile(struct mvtv_platform_profile **profile);

void mvtv_client_profile_free(struct mvtv_client_profile * client_profile);
void mvtv_dvb_config_free(struct mvtv_dvb_config *dvb_config);
void mvtv_platform_profile_free(struct mvtv_platform_profile * platform_profile);

#endif
#ifndef MOVISTARTV_APPSERVER_MVTV_H
#define MOVISTARTV_APPSERVER_MVTV_H

typedef struct
{
	int version;
	int demarcation;
} mvtv_client_profile;

typedef struct
{
	char * dvb_service_provider;
	char * dvb_entry_point;
} mvtv_dvb_config;

typedef struct
{
	struct mvtv_dvb_config *dvb_config;
	int version;
	int demarcation;
} mvtv_platform_profile;

/* Functions */
struct mvtv_client_profile * appserver_mvtv_get_client_profile(void);
struct mvtv_platform_profile * appserver_mvtv_get_platform_profile(void);

void mvtv_client_profile_free(mvtv_client_profile * client_profile);
void mvtv_dvb_config_free(mvtv_dvb_config *dvb_config);
void mvtv_platform_profile_free(mvtv_client_profile * client_profile);

#endif
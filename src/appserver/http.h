#ifndef MOVISTARTV_APPSERVER_HTTP_H
#define MOVISTARTV_APPSERVER_HTTP_H

#include <json/json.h>

json_object * http_fetch_json(const char *url);

#endif
#ifndef MOVISTARTV_SOCKETS_MULTICAST_H
#define MOVISTARTV_SOCKETS_MULTICAST_H

void multicast_service_discovery(char * address, unsigned int port, unsigned int payload_id, char **return_value);

#endif
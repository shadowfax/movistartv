#include "socket.h"
#include "multicast.h"

typedef struct {
	unsigned int version : 2;
	unsigned int resrv : 3;
	unsigned int encryption : 2;
	unsigned int crc_flag : 1;
	unsigned int total_segment_size : 24;
	unsigned int payload_id : 8;
	unsigned int segment_id : 16;
	unsigned int segment_version : 8;
	unsigned int section_number : 12;
	unsigned int last_section_number : 12;
	unsigned int compression : 3;
	unsigned int provider_id_flag : 1;
	unsigned int hdr_len : 4;
} sd_s_hdr_t;

void multicast_service_discovery(char * address, unsigned int port)
{
	int sd, addrlen;
	struct sockaddr_in addr;
	struct ip_mreq mreq;
	int reuse = 1;
	int n;
	char msg[1024];

	/* create socket */
	if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		fprintf(stderr, "ERROR: cannot create socket\n");
		return;
	}

	/* allow multiple sockets to use the same PORT number */
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
		fprintf(stderr, "ERROR: cannot reuse socket address\n");
		closesocket(sd);
		return;
	}

	/* bind port */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	if (bind(sd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		fprintf(stderr, "ERROR: cannot bind port %d\n", port);
		closesocket(sd);
		return;
	}

	/* join multicast group */
	mreq.imr_multiaddr.s_addr = inet_addr(address);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&mreq, sizeof(mreq)) < 0) {
		fprintf(stderr, "ERROR: cannot join multicast group '%s'", address);
		closesocket(sd);
		return;
	}

	/* infinite server loop */
	while (1) {
		addrlen = sizeof(addr);
		n = recvfrom(sd, msg, 1024, 0, (struct sockaddr *) &addr, &addrlen);
		if (n<n) {
			printf("ERROR: cannot receive data\n");
			continue;
		}

		printf("From %s:%d on %s : %s\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), address, msg);
	}

	closesocket(sd);
}
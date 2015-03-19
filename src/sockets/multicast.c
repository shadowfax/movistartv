#include "socket.h"
#include "multicast.h"

struct sd_s_hdr_t {
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
};

void multicast_service_discovery(char * address, unsigned int port)
{
	int sd, addrlen;
	struct sockaddr_in addr;
	struct ip_mreq mreq;
	int reuse = 1;
	int n;
	unsigned char databuf[4096];
#ifdef WIN32
	int timeout = 3000;
#else
	struct timeval timeout;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
#endif
	struct sd_s_hdr_t *header;

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

	/* Set timeout */
#ifdef WIN32
	if (setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(int)) < 0) {
#else
	if (setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval)) < 0) {
#endif
		fprintf(stderr, "ERROR: unable to set the socket receive timeout\n");
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
		n = recvfrom(sd, &databuf, 4096, 0, (struct sockaddr *) &addr, &addrlen);
		if (n <= 0) {
			printf("ERROR: cannot receive data\n");
			continue;
		}

		/* header */
		header = (struct sd_s_hdr*)malloc(sizeof(struct sd_s_hdr_t));
		memset((void *)header, 0, sizeof(header));

		header->version = ((int)databuf[0] >> 6) & 0x03;
		header->encryption = ((int)databuf[0] >> 1) & 0x03;
		header->crc_flag = (int)databuf[0] & 0x03;
		header->total_segment_size = ((unsigned int)databuf[1] << 16) + ((unsigned int)databuf[2] << 8) + ((unsigned int)databuf[3]);
		header->payload_id = (unsigned int)databuf[4];
		header->segment_id = ((unsigned int)databuf[5] << 8) + ((unsigned int)databuf[6]);

		header->section_number = (databuf[8] << 4) | ((databuf[9] >> 4) &0x0F);
		header->last_section_number = ((databuf[9] & 0x0f) << 8) + databuf[10];
		
		header->compression = ((unsigned int)databuf[11] >> 6) & 0x03;
		header->provider_id_flag = ((unsigned int)databuf[11] >> 4) & 0x01;
		header->hdr_len = (unsigned int)databuf[11] & 0x0F;

		printf("Received payload id %d and section number %d\n", header->payload_id, header->section_number );
		

		free(header);
	}

	closesocket(sd);
}
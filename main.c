// main.c - Bitcast 0.1
// Maintained by woodchips <woodchips@i2pmail.org>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "datastruct.c"
#include "net.c"
#include "file.c"
#include "sha/sha1.h"
#include "sha/sha1.c"
#include "crypto.c"

#define FILENAME "/home/user/Videos/test.avi"
#define TARGET "224.0.2.0"
#define DATAPORT "5500"
#define CHUNKSIZE 1024
#define BITRATE 1024000	// Not yet used
#define SENDSLEEP 9400	// 500 = 8mbit, 1150 = 5mbit, 2000 = 3.5mbit
			// 3000 = 2.5mbit, 9400 = 1mbit
			// These values are estimates, and a poor method
			// of bandwidth throttling :)

void sender();
void receiver();

int main()
{
	char c;
	printf("Bitcast 0.1\n");
	printf("woodchips <woodchips@i2pmail.org>\n\n");
	printf("1) Sender mode\n");
	printf("2) Receiver mode\n");
	printf("> ");
	c = getchar();
	if(c == '1')
		sender();
	else if(c == '2')
		receiver();

	return 0;
}

void sender()
{
	int total=0, i, filesize, total_chunks;
	FILE *fp;
	packet p;

	// Open File
	fp = fopen(FILENAME, "r");
	// Determine file size
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	rewind(fp);
	// Determine number of chunks
	total_chunks = filesize/CHUNKSIZE+1;

	// Process chunks
	for(i = 0; i < total_chunks; i++) {
		// Print debug text
		if(i % 100 == 0) {
			system("clear");
			printf("Sending file: %s\n", FILENAME);
			printf("    Chunk (%d/%d) %d MB sent\n", i, total_chunks, total/(1024*1024));
		}
		// Clear packet
		memset(&p, 0, sizeof p);
		// Add metadata to packet
		strcpy(p.filename, FILENAME);
		p.chunk_number = i;
		p.total_chunks = total_chunks;
		if(i < (total_chunks - 1))
			p.datalen = CHUNKSIZE;
		else
			p.datalen = filesize - (total_chunks * CHUNKSIZE);
		// Fetch chunk
		readChunk(fp, p.data, i * CHUNKSIZE, p.datalen);
		// Sign packet
		signPacket(p, p.signature);
		// Send chunk
		total += sendPacket(p, TARGET, DATAPORT);
		// Pause to limit throughput
		usleep(SENDSLEEP);
	}
	printf("  %d MB transmitted (%d packets)\n", total/(1024*1024), total_chunks);

}

void receiver()
{
	int sock, status, socklen;
	char buffer[1600];
	struct sockaddr_in saddr;
	struct ip_mreq imreq;

	memset(&saddr, 0, sizeof(struct sockaddr_in));
	memset(&imreq, 0, sizeof(struct ip_mreq));

	// Open a UDP socket
	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(sock < 0) {
		printf("Could not open receive socket");
		exit(0);
	}
	saddr.sin_family = PF_INET;
	saddr.sin_port = htons(5500);			// Listen on port 5500
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);	// Bind to any if
	status = bind(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));

	imreq.imr_multiaddr.s_addr = inet_addr(TARGET);
	imreq.imr_interface.s_addr = INADDR_ANY;

	// Join multicast group on default interface
	status = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void *)&imreq, sizeof(struct ip_mreq));

	socklen = sizeof(struct sockaddr_in);

	// Receive packet from socket
	status = recvfrom(sock, buffer, 1600, 0, (struct sockaddr *)&saddr, &socklen);
	printf("%s", buffer);

	// Shutdown socket
	shutdown(sock, 2);

	// Close socket
	close(sock);
}

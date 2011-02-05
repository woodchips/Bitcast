// main.c - Bitcast 0.1

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

#define FILENAME "/home/user/truecrypt-7.0a-setup-x86"
#define TARGET "172.16.255.1"
#define DATAPORT "5500"
#define CHUNKSIZE 1024

int main()
{
	int total=0, i, filesize, total_chunks;
	FILE *fp;
	packet p;

	printf("Sending file: %s\n", FILENAME);

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
	}
	printf("  %d bytes transmitted (%d packets)\n", total, total_chunks);

	return 0;
}

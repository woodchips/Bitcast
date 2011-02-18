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
#define TARGET "172.16.0.4"
#define DATAPORT "5500"
#define CHUNKSIZE 1024
#define BITRATE 1024000

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
	}
	printf("  %d MB transmitted (%d packets)\n", total/(1024*1024), total_chunks);

}

void receiver()
{

}

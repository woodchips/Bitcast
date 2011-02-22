// net.c - UDP network functions

// sendPacket() - Send a Bitcast packet using UDP, returns number of bytes sent or -1 for error
int sendPacket(packet p, char *target, char *dataport)
{
	char *data;
	data = (char *) &p;

	return sendUDP(data, sizeof p, target, dataport);
}

// sendUDP() returns -1 if error, otherwise returns number of bytes sent
int sendUDP(char *data, int len, char *target, char *dataport)
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if((rv = getaddrinfo(target, dataport, &hints, &servinfo)) != 0) {
		printf("getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}
		break;
	}

	// Check if we allocated a socket
	if (p == NULL) {
		printf("talker:  failed to bind socket\n");
		return -1;
	}

	// Attempt to send the data
	if ((numbytes = sendto(sockfd, data, len, 0, p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	freeaddrinfo(servinfo);

	//printf("talker: sent %d bytes to %s\n", numbytes, TARGET);

	// Close the socket
	close(sockfd);

	return numbytes;
}

void printpacket(char *received_packet)
{
	// Filename starts at &recv_packet+16
	// Data starts at &recv_packet+83
	recv_packet *p = (recv_packet *)(&received_packet);
	printf("Message type:  %d\n", p->message_type);
	printf("Filename:  %s\n", p->filename);
	printf("Chunk number:  %d\n", p->chunk_number);
	printf("Total chunks:  %d\n", p->total_chunks);
	printf("Data length:  %d\n", p->datalen);
	printf("\n");
}

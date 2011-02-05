// crypto.c - Wrappers for crypto routines

void signPacket(packet p, char *signature)
{
	SHA1Context sha;
	char secret[] = "12345";
	char payload[1500];

	// Copy the packet struct to the buffer (all except the signature var)
	memcpy(payload, &p, 1296);
	// Find the SHA1 hash of that buffer
	SHA1Reset(&sha);
	SHA1Input(&sha, payload, 1296);
	SHA1Result(&sha);
	// sha.Message_Digest[5] is an array of unsigned integers (4 bytes each)
	memcpy( (void *)(signature + 0), &sha.Message_Digest[0], 4);
	memcpy( (void *)(signature + 4), &sha.Message_Digest[1], 4);
	memcpy( (void *)(signature + 8), &sha.Message_Digest[2], 4);
	memcpy( (void *)(signature + 12), &sha.Message_Digest[3], 4);
	memcpy( (void *)(signature + 16), &sha.Message_Digest[4], 4);

}

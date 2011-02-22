// datastruct.c

typedef struct {
	int message_type;
	char filename[256];
	int chunk_number;
	int total_chunks;
	int datalen;
	char data[1024];
	char signature[20];
} packet;

typedef struct {
	char header[60];
	int message_type;
	char filename[256];
	int chunk_number;
	int total_chunks;
	int datalen;
	char data[1024];
	char signature[20];
} recv_packet;

typedef struct {
	char filename[256];
	int sentcount;
	int timeadded;
	struct filenode *next;
} filenode;

typedef struct {
	char filename[256];
	int total_chunks;
	short int chunkmap[1000000];
	struct file *next;
} file;



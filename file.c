// file.c - Filesystem operations

void readChunk(FILE *fp, char *data, int offset, int len)
{
	fseek(fp, offset, 0);
	fread(data, len, 1, fp);
}

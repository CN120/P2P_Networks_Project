/* Capstone project- Application Layer
File Syncer
Keegan Papakipos, Matti Masten,
Justin Cole, Chris Nelson */

#include "header.h"

int main(int argc, char* argv[]) {
	int listenfd = 0, connfd = 0;
	FILE *newFile, *locationPointer;
	struct sockaddr_in serv_addr;
	char buff[BUFFER_SIZE], filename[50];
	unsigned char fileHash[32];
	int bytesRead;
	// set up socket on port defined in header.h (currently 8050)
	memset (&serv_addr, '0', sizeof (serv_addr));
	memset (buff, '0', sizeof (buff));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons (PORT);
	// create socket, bind, and listen
	if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		printf ("socket error\n");
		return 1;
	}
	if (bind (listenfd, (struct sockaddr*)&serv_addr, sizeof (serv_addr)) < 0) {
		printf ("bind error\n");
		return 1;
	}
	if (listen (listenfd, 10) < 0) {
		printf ("listen error\n");
		return 1;
	}

	while (1) {
		// accept connection
		if ((connfd = accept (listenfd, (struct sockaddr*)NULL, NULL)) < 0) {
			printf ("accept error\n");
			return 1;
		}
		// receive filename, open file for writing
		read (connfd, filename, sizeof(filename));
		newFile = fopen(filename, "w");
		// receive file hash, find hash location, update hash
		read(connfd, fileHash, sizeof(fileHash));
		locationPointer = findHashLoc(filename);
		updateHash(&locationPointer, hashFile(filename));
		while ((bytesRead = read (connfd, buff, sizeof (buff))) > 0) {
			// append received data to file TODO
			fwrite(buff, sizeof(char), bytesRead, newFile);
		}
		// close connection
		close (connfd);
	}
	return 0;
}

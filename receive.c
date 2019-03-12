/* Capstone project- Application Layer
Keegan Papakipos, Matti Masten,
Justin Cole, Chris Nelson */

#include "header2.h"

int main(int argc, char* argv[]) {
	int listenfd = 0, connfd = 0;
	FILE *newFile, *locationPointer;
	struct sockaddr_in serv_addr;
	char buff[BUFFER_SIZE], filename[50];
	char fileHash[33], fileHash2[32];
	int bytesRead, name_length;
	char c;
	char response_char = '\0';

	fileHash[32] = '\0';
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
		read(connfd, buff, sizeof(filename));
		//read (connfd, filename, sizeof(filename));
		write (connfd, &response_char, 1);
		printf("\n\nReceived filename = %s\n", buff);

		name_length = 0;
		while((c = buff[name_length]) != '0')
		{
			name_length++;
		}

		memcpy (filename, buff, name_length);
		filename[name_length] = '\0';   /* null character manually added */

		locationPointer = findHashLoc(filename);
		newFile = fopen(filename, "w");

		memset (buff, '0', sizeof (buff));
		// receive file hash, find hash location, update hash
		read(connfd, buff, 32);
		//read(connfd, fileHash, sizeof(fileHash));
		write (connfd, &response_char, 1);
		memcpy (fileHash, buff, 32);
		//fileHash[32] = '\0';
		printf("Received hash = %s\n", fileHash);
		//printf("buffer version: %s\n", buff);

		if (locationPointer == NULL) {
			addHash(filename, fileHash);
		} else {
			//memcpy(fileHash2, fileHash, 32);
			updateHash(locationPointer, fileHash);
		}

		memset (buff, '0', sizeof (buff));
		while ((bytesRead = read (connfd, buff, sizeof (buff))) > 0) {
			printf("Got into reading the file.\n");
			//printf("Buffer contains: %s\n", buff);
			// append received data to file TODO
			fwrite(buff, sizeof(char), bytesRead, newFile);
		}
		// close connection
		fclose(newFile);
		close (connfd);
	}
	return 0;
}

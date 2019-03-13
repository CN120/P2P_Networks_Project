/* Capstone project- Application Layer
Keegan Papakipos, Matti Masten,
Justin Cole, Chris Nelson */

#include "header2.h"

int main(int argc, char* argv[]) {
	int listenfd = 0, connfd = 0, bytesRead;
	FILE *newFile, *locationPointer;
	struct sockaddr_in serv_addr;
	char buff[BUFFER_SIZE], filename[50];
	char fileHash[33], response_char = '\0';

	// Set last byte of hash to \0 to make it a string
	fileHash[32] = '\0';

	// set up TCP socket on port defined in header2.h
	// Accept connections from all IPs
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

		read(connfd, buff, sizeof(filename)); // receive filename

		write (connfd, &response_char, 1);    // send response to peer

		// Copy filename from buffer, find filename in hash.txt, and open file
		memcpy(filename, buff, 50);
		locationPointer = findHashLoc(filename);
		newFile = fopen(filename, "w");

		// Clear buffer
		memset (buff, '0', sizeof (buff));

		read(connfd, buff, 32);				// receive file hash
		write (connfd, &response_char, 1);		// send response to peer
		memcpy (fileHash, buff, 32);			// copy hash from buffer

		if (locationPointer == NULL) {
			addHash(filename, fileHash);		// If file not in hash.txt, add it
		} else {
			updateHash(locationPointer, fileHash);	// If file is present, update hash
		}

		memset (buff, '0', sizeof (buff));		// Clear buffer

		// Receive contents and write them to the file
		while ((bytesRead = read (connfd, buff, sizeof (buff))) > 0) {
			fwrite(buff, sizeof(char), bytesRead, newFile);
		}

		fclose(newFile);	// Close file
		close (connfd);	// Close TCP connection
	}
	return 0;
}

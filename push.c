#include "header.h"

#define SIZE   100

// Send file and hash to a single peer with IP address peerIP
int sendToPeer(char *fileName, char *hash, char *peerIP)
{
     int socketfd, bytes_read;
     char	buffer[SIZE];
     struct sockaddr_in server_address;
     FILE	*src_fp;

     // Initialize buffer and IP address with 0s
    memset (buffer, '0', sizeof (buffer));
    memset (&server_address, '0', sizeof (server_address));

     // Create socket, display error if unsuccessful
    if ((socketfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
     {
         printf("Socket creation error.\n");
         return 1;
     }
     // Set family and port parts of address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

     // Convert string server IP address to the correct format and set
     // Display error if unsuccessful
    if (inet_pton (AF_INET, peerIP, &server_address.sin_addr) <= 0)
    {
         printf("inet_pton error.\n");
          return 1;
     }

     // Connect to server, display error if unsuccessful
    if (connect (socketfd, (struct sockaddr *)&server_address, sizeof (server_address)) < 0)
    {
        printf("connect error.\n");
         return 1;
    }
     // Open the source file for reading, display error if unsuccessful
     if ((src_fp = fopen (fileName, "rb")) == NULL)
     {
         printf("file opening error.\n");
          return 1;
     }

     // Send name of file to server
     write (socketfd, fileName, strlen(fileName));

     // Send hash of file to server
     write(socketfd, hash, strlen(hash));

     // Read from source file and transmit to server SIZE bytes
     // at a time
     while ((bytes_read = fread (buffer, sizeof (char), SIZE, src_fp)) > 0)
          write (socketfd, buffer, bytes_read);

     fclose (src_fp);
     close (socketfd);

     return 0;
}

int main()
{
     return 0;
}

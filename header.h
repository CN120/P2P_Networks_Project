#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 8050
#define SIZE 100

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

/*
 parameters: a string of the filename thats needs its hash updated
 return value: a FILE pointer to the location immediately after the found hash
 */
FILE* findHashLoc(char fileName[]){
    FILE* fp = fopen("hash.txt", "r+");
    char hashVal[32];
    char fn_store[50];  //stores filenames read in from fscanf
    int num;
    do {
        num = fscanf(fp, "%s %s", fn_store, hashVal);
        if(strcmp(fn_store, fileName)==0){
            printf("found %s & %s\n", fn_store, hashVal);
            return fp; //file pointer to ed of line with hash
        }
    } while(num!=EOF);
    printf("failed to find %s\n", fileName);
    return fp;
}

/*
 parameters: poiter to a FILE pointer (or reference in our case)
 and a string of new hash value
 return value: returns non-negative value upon success, EOF on Error
 */
int updateHash(FILE** loc_ptr, char newHash[32]){
    fseek(*loc_ptr, -32, SEEK_CUR);
    int a = fputs(newHash, *loc_ptr);
    //fprintf(*loc_ptr, "%s", newHash);
    fflush(*loc_ptr);
    return a;
}

/* Capstone project- Application Layer
Keegan Papakipos, Matti Masten,
Justin Cole, Chris Nelson */

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
#include <openssl/md5.h>
#include <dirent.h>
#include "getIP.h"

#define PORT 8050
#define BUFFER_SIZE 100

/*
Sends file contents and hash to single peer with IP address peerIP
parameters: file name, file hash, and IP to be sent to
return value: returns 0 on success, 1 on error
 */
int sendToPeer(char fileName[50], char hash[32], char *peerIP)
{
     int socketfd, bytes_read;
     char   buffer[BUFFER_SIZE];
     struct sockaddr_in server_address;
     FILE   *src_fp;

     // Initialize buffer and IP address with 0s
     memset (buffer, '0', sizeof (buffer));
     memset (&server_address, '0', sizeof (server_address));

     // Create socket, display error if unsuccessful
     if ((socketfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
         printf("Socket creation error.\n");
         return 1;
     }

     // Set family and port parts of address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

     // Convert string server IP address to the correct format and set it
     // Display error if unsuccessful
    if (inet_pton (AF_INET, peerIP, &server_address.sin_addr) <= 0) {
         printf("inet_pton error.\n");
          return 1;
     }

     // Connect to server, display error if unsuccessful
    if (connect (socketfd, (struct sockaddr *)&server_address, sizeof (server_address)) < 0) {
        printf("connect error.\n");
         return 1;
    }

     // Open the source file for reading, display error if unsuccessful
     if ((src_fp = fopen (fileName, "rb")) == NULL) {
         printf("file opening error.\n");
          return 1;
     }

     // Send name of file to server and read response
     write (socketfd, fileName, strlen(fileName)+1);
     read (socketfd, buffer, 1);

     // Send hash of file to server and read response
     write(socketfd, hash, 32);
     read (socketfd, buffer, 1);

     // Read from source file and transmit to server
     while ((bytes_read = fread (buffer, sizeof (char), BUFFER_SIZE, src_fp)) > 0) {
          write (socketfd, buffer, bytes_read);
     }

     fclose (src_fp);    // Close file
     close (socketfd);   // Close TCP socket

     return 0;
}

/*
 Sends file contents and hash to all IPs in peers.txt
 parameters: 50 length char filename, 32 length unsigned char hash
*/

void sendToAllPeers(char fileName[50],  char hash[32]) {
    FILE* fp;
    char ip[15];
    char *myIP;
    char termIP[16];
    fp = fopen("Repo/peers.txt", "rb");
    myIP = getIP();
    while (fscanf(fp, "%s", ip) != EOF) {
        memcpy(termIP, ip, 15);
        termIP[16] = '\0';
        printf("the ip is: %s\n", ip);
        if (memcmp(ip, myIP, strlen(termIP)) != 0) {
            printf("sending\n");
            sendToPeer(fileName, hash, ip);
        }
    }
    fclose(fp);
}

/*
 parameters: a string of the filename thats needs its hash updated
 return value: a FILE pointer to the location immediately after the found hash
 */
FILE* findHashLoc(char fileName[50]) {
    FILE* fp = fopen("./hash.txt", "r+");
    char hashVal[32];
    char fn_store[50];  //will store filenames read in from fscanf
    int num;
    do {
        num = fscanf(fp, "%s %s", fn_store, hashVal);
        if(strcmp(fn_store, fileName)==0){
            return fp; //file pointer to ed of line with hash
        }
    } while(num!=EOF);

    fclose(fp);
    return NULL;
}

/*
 parameters: pointer to a FILE pointer (or reference in our case)
 and a string of new hash value
 return value: returns non-negative value upon success, EOF on Error
 */
int updateHash(FILE* loc_ptr, char *newHash){
    fseek(loc_ptr, -1 * 32, SEEK_CUR);
    int a = fwrite(newHash, 1, 32, loc_ptr);
    fclose(loc_ptr);
    return a;
}

/*
 parameters: file name in a length 50 array
 return value: returns the hash in a signed char array
*/
void hashFile(char fileName[50], char newHash[32]) {
    MD5_CTX c;
    char buf[512];
    ssize_t bytes;
    unsigned char newHashMD5[MD5_DIGEST_LENGTH];
    MD5_Init(&c);
    FILE* fp;
    fp = fopen(fileName, "rb");

    // Read file contents and update hash
    while((bytes = fread(buf, sizeof(char), 512, fp)) > 0) {
        MD5_Update(&c, buf, bytes);
    }
    MD5_Final(newHashMD5, &c);

    // Convert hash to hex, viewable as string
    for(int i = 0; i < 16; ++i)
         sprintf(&newHash[i*2], "%02x", (unsigned int)newHashMD5[i]);

    fclose(fp);
}

/*
 Reads a hash and stores it in oldHash
 parameters: pointer to the end of the hash to be read, pointer to hash storage location
 */
void readHash(FILE** fp, char *oldHash) {
    fseek(*fp, -1 * 32, SEEK_CUR);
    fread(oldHash, sizeof(char), 32, *fp);
}

/*
 Adds a hash to hash.txt (for a new file)
 parameters: name of the file being added, hash of the file
 */
void addHash(char *filename, char *newHash) {
    FILE *writefp = fopen("hash.txt", "a");
    fprintf(writefp, "\n%s %s", filename, newHash);
    fclose(writefp);
}

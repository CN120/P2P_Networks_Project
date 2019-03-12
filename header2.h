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


#define PORT 8050
#define BUFFER_SIZE 100

// Send file and hash to a single peer with IP address peerIP
int sendToPeer(char fileName[50], char hash[32], char *peerIP)
{
     int socketfd, bytes_read;
     char	buffer[BUFFER_SIZE];
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
    printf("connected successfully\n");
     // Open the source file for reading, display error if unsuccessful
     if ((src_fp = fopen (fileName, "rb")) == NULL)
     {
         printf("file opening error.\n");
          return 1;
     }

     // Send name of file to server
     printf("About to send filename: %s\n", fileName);
     write (socketfd, fileName, strlen(fileName)+1);
     read (socketfd, buffer, 1);

     // Send hash of file to server
     printf("About to send hash: %s\n", hash);
     //printf("%s\n", hash);
     write(socketfd, hash, 32);
     read (socketfd, buffer, 1);

     // Read from source file and transmit to server SIZE bytes
     // at a time
     while ((bytes_read = fread (buffer, sizeof (char), BUFFER_SIZE, src_fp)) > 0)
          write (socketfd, buffer, bytes_read);

     fclose (src_fp);
     close (socketfd);

     return 0;
}

/*
parameters: 50 length char filename, 32 length unsigned char hash
runs sendToPeer on all peers listed in peers.txt
*/
void sendToAllPeers(char fileName[50],  char hash[32]) {
    FILE* fp;
    char ip[15];
    fp = fopen("peers.txt", "rb");
    while (fscanf(fp, "%s", ip) != EOF) {
        printf("sending to peer\n");
        sendToPeer(fileName, hash, ip);
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
        //printf("Current position is: %ld\n", ftell(fp));
        if(strcmp(fn_store, fileName)==0){
            printf("found %s with hash %s\n", fn_store, hashVal);
            return fp; //file pointer to ed of line with hash
        }
    } while(num!=EOF);
    printf("failed to find %s\n", fileName);
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

    /* previous code */
    //int a = fputs((char *)newHash, *loc_ptr);
    //fprintf(*loc_ptr, "%s", newHash);
    //fflush(*loc_ptr);

}

/*
    parameters: file name in a length 50 array
    return value: returns the hash in an unsigned char array
*/
void hashFile(char fileName[50], char newHash[32]) {
    MD5_CTX c;
    char buf[512];
    ssize_t bytes;
    unsigned char newHashMD5[MD5_DIGEST_LENGTH];
    MD5_Init(&c);
    FILE* fp;
    fp = fopen(fileName, "rb");
    while((bytes = fread(buf, sizeof(char), 512, fp)) > 0) {
        MD5_Update(&c, buf, bytes);
    }
    MD5_Final(newHashMD5, &c);

    for(int i = 0; i < 16; ++i)
         sprintf(&newHash[i*2], "%02x", (unsigned int)newHashMD5[i]);

    fclose(fp);
}
void readHash(FILE** fp, char *oldHash) {
    fseek(*fp, -1 * 32, SEEK_CUR);
    fread(oldHash, sizeof(char), 32, *fp);
}
void addHash(char *filename, char *newHash) {
    FILE *writefp = fopen("hash.txt", "a");
    fprintf(writefp, "\n%s %s", filename, newHash);
    fclose(writefp);
}
/*
void checkFiles(DIR *dr) {
    struct dirent *de;
    while (de = readdir(dr) != NULL) {
        if () {
            newHash = hashFile(filename);
            if (newHash != oldHash) {
                updateHash(&hashfp, newHash);
                sendToAllPeers(filename, newHash);
            }
        }

    }
}
*/

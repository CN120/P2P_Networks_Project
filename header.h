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

#define PORT 8050

void sendToAllPeers(char fileName[50],  unsigned char hash[32]) {
    FILE* fp;
    int n;
    char ip[15];
    fp = fopen("peers.txt", "rb");
    while (fscanf(fp, "%s", ip) != EOF) {
        sendToPeer(fileName, hash, ip);
    }
}

int sendToPeer(char *fileName, unsigned char *hash, char *peerIP);

/*
 parameters: a string of the filename thats needs its hash updated
 return value: a FILE pointer to the location immediately after the found hash
 */
FILE* findHashLoc(char fileName[50]) {
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
 parameters: pointer to a FILE pointer (or reference in our case)
 and a string of new hash value
 return value: returns non-negative value upon success, EOF on Error
 */
int updateHash(FILE** loc_ptr, unsigned char newHash[32]){
    fseek(*loc_ptr, -32, SEEK_CUR);
    int a = fputs(newHash, *loc_ptr);
    //fprintf(*loc_ptr, "%s", newHash);
    fflush(*loc_ptr);
    return a;
}

/*
    parameters: file name in a length 50 array
    return value: returns the hash in an unsigned char array
*/
unsigned char* hashFile(char fileName[50]) {
    int n;
    MD5_CTX c;
    char buf[512];
    ssize_t bytes;
    unsigned char out[MD5_DIGEST_LENGTH];
    MD5_Init(&c);
    FILE* fp;

    fp = fopen(fileName, "rb");
    while(bytes = fread(buf, sizeof(char), 512, fp) > 0) {
        MD5_Update(&c, buf, bytes);
    }

    MD5_Final(out, &c);
    fclose(fp);
    return out;
}

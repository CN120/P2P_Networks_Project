/* Capstone project- Application Layer
Keegan Papakipos, Matti Masten,
Justin Cole, Chris Nelson */

#include "header2.h"

int main(int argc, char* argv[]) {
     FILE *readfp;
     char filename[50];
     char oldHash[33], newHash[32];
     char directPath[50] = "./Repo/";
     struct dirent *de;

     //Open Repo, the synced repository
     DIR *dr = opendir("./Repo");

     // Loop through every file in Repo
     while ((de = readdir(dr)) != NULL) {
          if (de->d_name[0] != '.') {        // Skip hidden files

               // Initialize and construct file name
               memset(filename, 0, 50);
               strcpy(filename, directPath);
               strcat(filename, de->d_name);

               // Hash the file and find its location in hash.txt
               hashFile(filename, newHash);
               readfp = findHashLoc(filename);

               // If file is not new, get its old hash
               if (readfp != NULL) {
                    readHash(&readfp, oldHash);
                    oldHash[32] = '\0';

                    // If file has been changed, update its hash and send to peers
               	if (memcmp(newHash, oldHash, 32) != 0) {
               		printf("\nThe file %s has been updated.\n", filename);
               		updateHash(readfp, newHash);
               		sendToAllPeers(filename, newHash);
                         printf("Synced with peers.\n");
                    }

               // If file is new, add its hash and send to peers
               } else {
                    printf("\nThe file %s is new.\n", filename);
                    addHash(filename, newHash);
                    sendToAllPeers(filename, newHash);
                    printf("Synced with peers.\n");
               }
          }
     }
     return 0;
}

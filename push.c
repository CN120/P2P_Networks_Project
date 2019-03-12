/* Capstone project- Application Layer
Keegan Papakipos, Matti Masten,
Justin Cole, Chris Nelson */

#include "header2.h"

int main(int argc, char* argv[]) {
  FILE *readfp;
  char filename[50];
  char oldHash[32], newHash[32];
  char directPath[50] = "./Repo/";
  struct dirent *de;
  //unsigned char oldHash[MD5_DIGEST_LENGTH], newHash[MD5_DIGEST_LENGTH];
  // open hash.txt for writing
      printf("%s\n", "opened hash");
  //open repo
  DIR *dr = opendir("./Repo");
        printf("%s\n", "opened directory");

  // save old filename and hash
  //fscanf(hashfp, "%s %s\n", filename, oldHash) != EOF
	while ((de = readdir(dr)) != NULL) {
	// new hash
		if (de->d_name[0] != '.') {
			memset(filename, 0, 50);
			strcpy(filename, directPath);
			strcat(filename, de->d_name);
			printf("%s\n", filename);
			hashFile(filename, newHash);
               printf("new hash: %s\n", newHash);
               //for (int i = 0; i < 33; ++i)
               //     printf("%c", newHash[i]);
			readfp = findHashLoc(filename);
			if (readfp != NULL) {
				readHash(&readfp, oldHash);
                    printf("old hash: %s", oldHash);
                    //for(int i = 0; i < 32; ++i)
                    //     printf("%c", oldHash[i]);
				if (memcmp(newHash, oldHash, 32) != 0) {
					printf("FILE UPDATED\n");
					updateHash(&readfp, newHash);
					sendToAllPeers(filename, newHash);
				}
			} else {
				printf("%s\n", newHash);
				addHash(filename, newHash);
				printf("sending\n");
				printf("%s\n", newHash);
				sendToAllPeers(filename, newHash);
			}
		}
	}
}

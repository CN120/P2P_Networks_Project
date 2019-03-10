/* Capstone project- Application Layer
Keegan Papakipos, Matti Masten,
Justin Cole, Chris Nelson */

#include "header.h"

int main(int argc, char* argv[]) {
  FILE *readfp, *writefp;
  char filename[50];
  struct dirent *de;
  unsigned char *oldHash, *newHash;
  // open hash.txt for writing
  writefp = fopen("hash.txt", "wa");
  //open repo
  DIR *dr = opendir("./repo");

  // save old filename and hash
  //fscanf(hashfp, "%s %s\n", filename, oldHash) != EOF
	while ((de = readdir(dr)) != NULL) {
	// new hash
		newHash = hashFile(de->d_name);
		readfp = findHashLoc(de->d_name);
		if (readfp != NULL) {
			oldHash = readHash(&readfp);
			if (strcmp( (char *)newHash, (char *)oldHash) == 0) {
				updateHash(&readfp, newHash);
				sendToAllPeers(filename, newHash);
			}
		} else {
			fwrite( (char *)newHash, sizeof(char), 32, writefp);
			sendToAllPeers(filename, newHash);
		}
		
	}
}

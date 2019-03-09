/* Capstone project- Application Layer
Keegan Papakipos, Matti Masten,
Justin Cole, Chris Nelson */

#include "header.h"

int main(int argc, char* argv[]) {
  FILE* hashfp;
  char filename[50];
  unsigned char oldHash[32], *newHash;

  // open hash.txt for reading
  hashfp = fopen("hash.txt", "r");
  // save old filename and hash
	while (fscanf(hashfp, "%s %s\n", filename, oldHash) != EOF) {
    // new hash
		newHash = hashFile(filename);
		if (newHash != oldHash) {
			updateHash(&hashfp, newHash);
			sendToAllPeers(filename, newHash);
		}
	}
}

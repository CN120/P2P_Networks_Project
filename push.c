#include "header.h"

int main(int argc, char* argv[]) {
  char filename[50];
  unsigned char newHash[32];

  // open hash.txt for reading
	while (fscanf a line =/= EOF, store file name and old hash) {
    // new hash TODO
		newHash = hashFile(filename);
		if (new hash =/= old hash) {
			updateHash(&fp, newHash);
			sendToAllPeers(filename, newHash);
		}
	}
}

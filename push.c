/* Capstone project- Application Layer
Keegan Papakipos, Matti Masten,
Justin Cole, Chris Nelson */

#include "header.h"

int main(int argc, char* argv[]) {
  FILE *readfp, *writefp;
  char filename[50];
  char directPath[50] = "./Repo/";
  struct dirent *de;
  unsigned char *oldHash, *newHash;
  // open hash.txt for writing
  writefp = fopen("hash.txt", "wa");
      printf("%s\n", "opened hash");
  //open repo
  DIR *dr = opendir("./Repo");
        printf("%s\n", "opened directory");

  // save old filename and hash
  //fscanf(hashfp, "%s %s\n", filename, oldHash) != EOF
	while ((de = readdir(dr)) != NULL) {
	// new hash
		if (de->d_name[0] != '.') {
			strcpy(filename, directPath);
			strcat(filename, de->d_name);
			printf("%s\n", filename);
			newHash = hashFile(filename);
			readfp = findHashLoc(filename);
			if (readfp != NULL) {
				oldHash = readHash(&readfp);
				if (strcmp( (char *)newHash, (char *)oldHash) == 0) {
					updateHash(&readfp, newHash);
					sendToAllPeers(filename, newHash);
				}
			} else {
				printf("writing\n");
				fprintf(writefp, "\n%s %s", filename, newHash);
				printf("sending\n");
				sendToAllPeers(filename, newHash);
			}
		}

	}
	fclose(writefp);
}

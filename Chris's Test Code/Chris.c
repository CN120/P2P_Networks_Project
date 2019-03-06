/* Chris's Code */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


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




int main(int argc, char const *argv[]) {
    /* code */
    char fileN[] = "file2.c";
    FILE* hashLoc = findHashLoc(fileN);
    updateHash(&hashLoc, "ChrisNelsonTest2ChrisNelsonTest1");

    return 0;
}

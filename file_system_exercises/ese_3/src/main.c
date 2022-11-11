#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "errExit.h"

#define BUFFER_SZ 1
char buffer[BUFFER_SZ];

int main(int argc, char *argv[])
{

    char *charSearch, *charReplace;
    char *fileSource, *fileDestination;

    // Check command line arguments
    if (argc != 5)
    {
        printf("Usage: %s <sourceFile> <destinationFile> <char search> <char replace>\n", argv[0]);
        return 1;
    }
    else
    {
        fileSource = argv[1];
        fileDestination = argv[2];
        charSearch = argv[3];
        charReplace = argv[4];
    }

    // open the source file in read only mode
    int fileS = open(fileSource, O_RDONLY);
    if (fileS == -1)
    {
        printf("File %s does not exist\n", fileSource);
        errExit("source open failed");
    }

    int fileD = -1;
    // check if the destination file already exists
    if (access(fileDestination, F_OK) == 0)
    {
        printf("Il file %s gia' esiste!\n", fileDestination);
        return 1;
    }
    // create and open the destination file for only writing
    fileD = open(fileDestination, O_CREAT | O_EXCL | O_WRONLY,
                 S_IRUSR | S_IWUSR);
    // if fileD is -1, then something went wrong with the open system call
    if (fileD == -1)
        errExit("open failed");

    // Start copying
    ssize_t bR;
    char c;
    do
    {
        //bR = read(fileS, buffer, sizeof(buffer));
        bR = read(fileS, &c, sizeof(char));
        // check if read completed successfully
        if (bR == -1)
            errExit("read failed");
        
        if(c == *charSearch){
            c = *charReplace;
        }

        // check if write completed successfully
        if (bR > 0 && write(fileD, &c, sizeof(char)) != sizeof(char))
            errExit("write failed");
        // inter. until end-of-file is reached
    } while (bR > 0);

    // close file descriptors
    if (close(fileS) == -1 || close(fileD) == -1)
        errExit("close failed");

    return 0;
}

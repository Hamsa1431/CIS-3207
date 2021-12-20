#include <stdio.h>
#include <stdlib.h>

#define SIZE 64

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("**ERROR: No file named to pass!**\n");
        exit(0);        //exits when no.of arguments is 1 
    }

    char buffer[SIZE];          //string to read from the file

    int n;
    for(n=1; n<argc; n++)
    {
        FILE *ptr = fopen(argv[n], "r");        //file pointer opens file to read

        if(ptr == NULL)
        {
            printf("**wcat cannot open the file!**");
            exit(1);        //exits 1 when it cannot open the file and pointer is null
        }

        while(fgets(buffer, SIZE, ptr) != NULL)     //reads the file to buffer
        {
            printf("%s\n", buffer);       //outputs the content of the file in buffer
        }

        fclose(ptr);        //closes the file
    }

    return(0);

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("**ERROR: No term or file named to pass!**\n");
        exit(1);        //exits with status 1 when no.of commands is 1
    }
    else if(argc == 2)
    {
        char *term = argv[1];   //1st argument becomes search term

        while (1)
        {
            char buffer[SIZE];
            scanf("%s", buffer);        //reads string to buffer
            char *at = strstr(buffer, term);        //searches asked term in buffer
            if(at != NULL)
            {
                printf("%s\n", buffer);       //outputs the found lines, if found
            }
        }   
    }
    else if(argc>2)
    {
        char *term = argv[1];       //1st argument becomes search term
        char buffer[SIZE];
        int n;

        for(n=2; n<argc; n++)
        {
            FILE *ptr = fopen(argv[n], "r");    //file pointer opens the asked file to read
            if(ptr == NULL)
            {
                printf("**wgrep cannot open the file!**");
                exit(1);        //exits with status 1 when ptr is null and cant open the file
            }

            while(getline(buffer, SIZE, ptr) != NULL)   //reads file into buffer
            {
                char *at = strstr(buffer, term);    //checks if line contains the search term
                if(at != NULL)
                {
                    printf("%s\n", buffer);       //if search term is found, outputs the line
                }
            }

            fclose(ptr);        //closes the file
        }
    }

    return(0);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char curr;
    char prev;

    int count = 1;

    if(argc == 1 )
    {
        printf( "wzip: file1 [file2 ...]\n");
        exit(1);
    }
    else
    {
        int i;
        for(i =1; i<argc; i++)
        {
            FILE *file = fopen(argv[i],"rb") ;

            if(file == NULL)
            {
                printf("wzip: cannot open file\n");
                exit(1);
            }
            else
            {
                prev = fgetc(file);

                while((curr = fgetc(file)) != EOF)
                {
                    if(prev == curr)
                    {
                        count++;
                    }
                    else
                    {
                        fwrite(&count, sizeof(int), 1, stdout);
                        fwrite(&prev, sizeof(char), 1, stdout);

                        prev = curr;
                        count = 1;
                    }
                }

                fwrite(&count, sizeof(int), 1, stdout);
                fwrite(&prev, sizeof(char), 1, stdout);
            }

            fclose(file);
            printf("\n");
        }
    }

    return(0);
}
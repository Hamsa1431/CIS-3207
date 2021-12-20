#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SIZE 64

int main(int argc, char *argv[])
{
   char curr = 0;
   char buffer[SIZE];

   if(argc == 1)
   {
        printf("**wunzip: file1 [file2 ...]**\n");
        exit(1);
   }

       int i;
       for(i =1; i< argc;i++)
       {
            FILE* file = fopen(argv[i],"rb");
           
            if(file == NULL)
            {
               printf("**Cannot open the file**\n");
               exit(1);
            }

            int count = 0;
            while(fread(&count, sizeof(int), 1, file))
            {
                fread(&curr, sizeof(char), 1, file);

                int i;
                for(i=0; i<count; i++)
                {
                    printf("%c", curr);
                }
            }
           
           printf("\n");
           fclose(file);
           printf("\n");
        }

    return(0);
}
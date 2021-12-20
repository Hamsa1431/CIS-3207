#include <stdio.h>
#include <stdlib.h>

int randNum(int low, int high)
{
    if((high-low) < 0 || high < 0 || low < 0)
    {
        printf("Invalid input");
    }

    return (rand() % ((high - low) + 1) + low);
}

int diskProb(int QUIT_PROB)     // determines the probability to get into the disk
{
    if(QUIT_PROB < 0 || QUIT_PROB > 100)
    {
        printf("Invalid");
        return -1;
    }

    int prob = randNum(1, 100);

    if(prob <= QUIT_PROB)
    {
        return 0;
    }
    
    return 1;
}
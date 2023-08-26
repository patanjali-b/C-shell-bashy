#include "headers.h"



void pastevents(char* input, char* home)
{
    // store the input in history

    strcpy(history[counter_pastevents], input);
    counter_pastevents++;
    if(counter_pastevents == 15)
    {
        // replace the oldest command

        for(int i = 0; i < 14; i++)
        {
            strcpy(history[i], history[i+1]);
        }
        strcpy(history[14], input);

    }
   
    for(int i = 0; i < 15; i++)
    {

        printf("%d. %s\n", i, history[i]);
    }
    
}


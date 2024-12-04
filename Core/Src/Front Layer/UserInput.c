/*
 * UserInput.c
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */
#include "main.h"
#include "UART.h"
#include "UserInput.h"

void FL_Parse_String(char String_Received[1028])
{
	char stringStop = 0;
    char processed_string[1028]; // Temporary buffer for the current command
    int i = 0, j = 0, spacebarCheck = 0;           // Indexes for input and output buffers

    UART2_SendString("Processing String: \n");

    while (stringStop == 0)
    {
    	if(processed_string <= 999)
    	{
    	if(processed_string >= 100)
    		    {
    		    	UART2_SendString("\n\n");
    		    	UART2_SendString("Warning! String received is larger than 100 characters");
    		    	UART2_SendString("\n\n");
    		    }
    	{
    	if((String_Received[i] == '\0') && (String_Received[i-1] != '|'))
    	        	{
    					UART2_SendString("\n\n");
    	        		UART2_SendString("ERROR! String Received does not meet requirements. Missing '|'");
    	        		UART2_SendString("\n\n");
    	        		stringStop = 1;
    	        	}
    	else if (String_Received[i] == '|' || String_Received[i] == '\n')
        {
            // End of command detected, null-terminate and parse the command
            processed_string[j] = '\0';

            UART2_SendString("Command found: ");
            UART2_SendString(processed_string);
            UART2_SendString("\n");

//            matchesCommand(processed_string); // Process the command
            stringStop = 1;

            // Reset the buffer index for the next command
            j = 0;
        } else
        	{
            // Append character to the current command
        	if(String_Received[i] == ',')
        		{
        		processed_string[j++] = String_Received[i];
        		if(String_Received[i+1] == ' ')
        			{
        	    		i++;
        	    		spacebarCheck++;
        			}

        	    }
        	else
            processed_string[j++] = String_Received[i];
        	}


        i++;
    }
    	}

    	else if(processed_string >= 1000)
    	    {
    		UART2_SendString("\n\n");
    		UART2_SendString("ERROR! String Received is bigger than 1000, can not complete request.");
    		UART2_SendString("\n\n");
    	    }
    }


}

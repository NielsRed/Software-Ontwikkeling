/*
 * DataParsing.h
 *
 *  Created on: Nov 21, 2024
 *      Author: dupon
 */

#ifndef INC_DATAPARSING_H_
#define INC_DATAPARSING_H_

#define ALL_COMMANDS 5
#define MAX_DATA_SIZE 50

char *userCommands[] = {"lijn", "rechthoek", "tekst", "bitmap", "clearscherm"};

void matchesCommand(const char *command);
void handleUnknownCommand();



#endif /* INC_DATAPARSING_H_ */

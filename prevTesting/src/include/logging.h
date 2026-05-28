//STANDALONE MODULE
#ifndef LOGGING_H
#define LOGGING_H

void initializeLogFile(char *sourceLogFilePath, FILE **sourceLogFile, struct tm *tm); // initializing log file
void logToFile(FILE *sourceLogFile, struct tm *tm, char *text); // write info into the log file
void rawLogToFile(FILE *sourceLogFile, char *text); // without timestamp

#endif
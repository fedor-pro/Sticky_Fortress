//STANDALONE MODULE

#pragma once
#include "types.h"

void initializeLogFile(char *sourceLogFilePath, FILE **sourceLogFile, struct tm *tm) // initializing log file
{
    sprintf(sourceLogFilePath, "./logs/latest.log"); 
            /*"../logs/programExecution_%02d_%02d_%02d_%02d_%02d",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min*/

    *sourceLogFile = fopen(sourceLogFilePath, "w");
}

void logToFile(FILE *sourceLogFile, struct tm *tm, char *text) // write info into the log file
{
    fprintf(sourceLogFile, "[%04d-%02d-%02d %02d:%02d:%02d] ",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec);

    fprintf(sourceLogFile, "%s", text);
}

void rawLogToFile(FILE *sourceLogFile, char *text) // without timestamp
{
    fprintf(sourceLogFile, "%s", text);
}
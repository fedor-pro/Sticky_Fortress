#include "logging.h"
#include <time.h>

void initializeLogFile(char *sourceLogFilePath, logDataLord *logData) // Initializing log file
{
    sprintf(sourceLogFilePath, "./logs/latest.log"); 
            /*"../logs/programExecution_%02d_%02d_%02d_%02d_%02d",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min;*/

    logData->sourceLogFile = fopen(sourceLogFilePath, "w");

    fflush(logData->sourceLogFile);
}

void logToFile(logDataLord *logData, char *text) // Write info into the log file
{
    fprintf(logData->sourceLogFile, "[%04d-%02d-%02d %02d:%02d:%02d] ",
            logData->tm->tm_year + 1900, logData->tm->tm_mon + 1, logData->tm->tm_mday,
            logData->tm->tm_hour, logData->tm->tm_min, logData->tm->tm_sec);

    fprintf(logData->sourceLogFile, "%s", text);

    fflush(logData->sourceLogFile);
}

void rawLogToFile(logDataLord *logData, char *text) // Without timestamp
{
    fprintf(logData->sourceLogFile, "%s", text);

    fflush(logData->sourceLogFile);
}
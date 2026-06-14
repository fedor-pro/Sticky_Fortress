#if !defined(LOGGING_H)
#define LOGGING_H

#include "types.h"
#include <time.h>

void initializeLogFile(char *sourceLogFilePath, logDataLord* logData);
void logToFile(logDataLord* logData, char *text);
void rawLogToFile(logDataLord* logData, char* text);

#endif
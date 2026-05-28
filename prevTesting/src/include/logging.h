#if !defined(LOGGING_H)
#define LOGGING_H

#include "types.h"
#include <time.h>

void initializeLogFile(char *sourceLogFilePath, FILE **sourceLogFile, struct tm *tm);
void logToFile(FILE *sourceLogFile, struct tm *tm, char *text);
void rawLogToFile(FILE *sourceLogFile, char *text);

#endif
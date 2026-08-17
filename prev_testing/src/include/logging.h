#if !defined(LOGGING_H)
#define LOGGING_H

#include "types.h"
#include <time.h>

void initialize_log_file(char *source_log_file_path, log_data_lord *log_data);
void log_to_file(log_data_lord *log_data, char *text);
void raw_log_to_file(log_data_lord *log_data, char *text);

#endif
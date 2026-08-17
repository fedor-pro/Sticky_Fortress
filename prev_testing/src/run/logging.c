#include "logging.h"
#include <time.h>

void initialize_log_file(char *source_log_file_path, log_data_lord *log_data) // Initializing log file
{
    sprintf(source_log_file_path, "./logs/latest.log"); 
            /*"../logs/program_execution_%02d_%02d_%02d_%02d_%02d",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min;*/

    log_data->source_log_file = fopen(source_log_file_path, "w");

    fflush(log_data->source_log_file);
}

void log_to_file(log_data_lord *log_data, char *text) // Write info into the log file
{
    fprintf(log_data->source_log_file, "[%04d-%02d-%02d %02d:%02d:%02d] ",
            log_data->tm->tm_year + 1900, log_data->tm->tm_mon + 1, log_data->tm->tm_mday,
            log_data->tm->tm_hour, log_data->tm->tm_min, log_data->tm->tm_sec);

    fprintf(log_data->source_log_file, "%s", text);

    fflush(log_data->source_log_file);
}

void raw_log_to_file(log_data_lord *log_data, char *text) // Without timestamp
{
    fprintf(log_data->source_log_file, "%s", text);

    fflush(log_data->source_log_file);
}
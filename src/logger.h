/*
 * Logging methods by levels
 */
void log_error(char* format, ...);
void log_warning(char* format, ...);
void log_status(char* format, ...);
void log_debug(char* format, ...);

/*
 * Log level configurator
 * Default is LOG_MAX_LEVEL_ERROR_WARNING_STATUS
 */ 

#define LOG_MAX_LEVEL_ERROR 0
#define LOG_MAX_LEVEL_ERROR_WARNING_STATUS 1
#define LOG_MAX_LEVEL_ERROR_WARNING_STATUS_DEBUG 2

void logger_set_log_level(const int level);

/*
 * Set target type
 * Default is syslog
 */
void logger_reset_state(void);
int logger_set_log_file(const char* filename);
void logger_set_out_stdout();
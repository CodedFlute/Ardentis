#pragma once
/**
 * @file ard_log.h
 * @author Roland Neill
 * @brief Logging library header for the Ardentis flight computer.
 * @version 0.1-alpha
 * @date 2025-04-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

/**
 * @brief Logging level enum type.
 * 
 */
enum ard_logLevel_t {
    LOG_DISABLE = 0,
    LOG_ERROR = 1,
    LOG_WARN = 2,
    LOG_INFO = 3,
    LOG_DEBUG = 4,
    LOG_VERBOSE = 5
};

/**
 * @brief Enumeration type for error codes
 * 
 */
enum ard_errorCode_t {
    ERROR_NONE = 0,
    ERROR_GENERIC = -1
};

 class ardLog {
    public:
        /**
         * @brief Construct a new ard Log object
         * 
         * @param tag The tag for this logger instance
         */
        ardLog(const char* tag);

        /**
         * @brief Set the logging level
         * 
         * @param level The level of logging to set
         */
        void logLevel(ard_logLevel_t level);

        /**
         * @brief Log an error message
         * 
         * @param message The error message to log
         */
        void error(const char* message);

        /**
         * @brief Log a warning message
         * 
         * @param message The warning message to log
         */
        void warn(const char* message);
    
        /**
         * @brief Log an informational message
         * 
         * @param message The informational message to log
         */
        void info(const char* message);
    
        /**
         * @brief Log a debug message
         * 
         * @param message The debug message to log
         */
        void debug(const char* message);
        
        /**
         * @brief Log a verbose message
         * 
         * @param message The verbose message to log
         */
        void verb(const char* message);

        private:
            const char* TAG;
};
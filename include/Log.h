#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <freertos/FreeRTOS.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define CR "\r\n"

#ifndef UNPHONE_PRODUCTION_BUILD
#define APP_ASSERT(x) if ( !(x) ) {LOG_ERROR("Assertion failed in %s:%d", __FILE__, __LINE__); for(;;) taskYIELD(); }
#else
#define APP_ASSERT(x)  {}
#endif

/**
* Logging is a helper class to output informations over
* RS232. If you know log4j or log4net, this logging class
* is more or less similar ;-)
* Different loglevels can be used to extend or reduce output
* All methods are able to handle any number of output parameters.
* All methods print out a formated string (like printf).
* To reduce output and program size, reduce loglevel.
*
* Output format string can contain below wildcards. Every wildcard
* must be start with percent sign (\%)
*
* Depending on loglevel, source code is excluded from compile !
*
* Wildcards
*
* \%s	replace with an string (char*)
* \%c	replace with an character
* \%d	replace with an integer value
* \%l	replace with an long value
* \%x	replace and convert integer value into hex
* \%X	like %x but combine with 0x123AB
* \%b	replace and convert integer value into binary
* \%B	like %x but combine with 0b10100011
* \%t	replace and convert boolean value into "t" or "f"
* \%T	like %t but convert into "true" or "false"
* </ul>
* Loglevels
*
* 0    LOG_LEVEL_NOOUTPUT    no output
* 1    LOG_LEVEL_ERROR    only errors
* 2    LOG_LEVEL_INFO    errors and info
* 3    LOG_LEVEL_DEBUG    errors, info and debug
* 4    LOG_LEVEL_VERBOSE    all
*
*/

class Log {
public:

    enum Level { FATAL, ERROR, WARN, INFO, DEBUG, TRACE };
    char const *const LevelNames[6] = { "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE" };

    static Log appLogger;
    /**
        * Initializing, must be called as first.
        * \param void
        * \return void
        *
        */
    void init(Log::Level level, long baud);

    /**
        * Initializing, must be called as first.
        * \param void
        * \return void
        *
        */
    void init(Log::Level level, Stream* stream = nullptr);

    /**
    * Output an log message with a given log level.
    * Output message contains level followed by original msg
    * \param msg format string to output
    * \param ... any number of variables
    * \return void
    */
    void log(Level level, char const * const msg, ...);

private:
    SemaphoreHandle_t logMutex = xSemaphoreCreateMutex();
    int _level = Log::TRACE;
    long _baud = 0;

    Stream* _stream = nullptr;

    void _print(const char *format, va_list args);

    void _detectSerial(long baud);

};

namespace {
Log& getLogger() {
    return Log::appLogger;
};
}

#define LOG_INIT(level, baud_rate) \
    do { \
		getLogger().init(level, baud_rate); \
    } while (false)

#define LOG_TRACE(msg...) \
    do { \
        getLogger().log(Log::TRACE, msg); \
    } while (false)

#define LOG_DEBUG(msg...) \
    { \
        getLogger().log(Log::DEBUG, msg); \
    }

#define LOG_INFO(msg...) \
    do { \
        getLogger().log(Log::INFO, msg); \
    } while (false)

#define LOG_WARN(msg...) \
    do { \
        getLogger().log(Log::WARN, msg); \
    } while (false)

#define LOG_ERROR(msg...) \
    do { \
    getLogger().log(Log::ERROR, msg); \
    } while (false)

#define LOG_FATAL(msg...) \
    do { \
        getLogger().log(Log::FATAL, msg); \
    } while (false)


#endif





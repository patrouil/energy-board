#include "Log.h"
#include <lvgl.h>


Log Log::appLogger;

void Log::init(Level level, long baud)
{
    _level = level;
    _baud = baud;
    _detectSerial(baud);
}

void Log::init(Level level, Stream* stream)
{
    _level = level;
    _stream = stream;
}

void Log::_detectSerial(long baud)
{
    Serial.begin(_baud);
    //SerialUSB.begin(_baud);
    int const interval = 5000;
    unsigned long initialMillis = millis();
    unsigned long currentMillis;
    unsigned long elapsed;
    do
    {
        currentMillis = millis();
        elapsed = currentMillis - initialMillis;
    }
    while (elapsed < interval);

    char usb_port_name[12];
    if (Serial)
    {
        _stream = &Serial;
        strcpy(usb_port_name, "programming");
    }
    this->log(Log::TRACE, "Logger ready, printing on %s port", usb_port_name);
}

void Log::log(Level level, char const* const msg, ...)
{
    if (level <= _level)
    {
        xSemaphoreTake(logMutex, portMAX_DELAY);

        char loglevel[8];
        sprintf(loglevel, "%s:", LevelNames[level]);
        _stream->print(loglevel);
        va_list args;
        va_start(args, msg);
        _print(msg, args);
        _stream->print(CR);
        xSemaphoreGive(logMutex);
    }
}

void Log::_print(const char* format, va_list args)
{
    //
    // loop through format string
    if ( ! Serial.availableForWrite())
        return;
    for (; *format != 0; ++format)
    {
        if (*format == '%')
        {
            ++format;
            if (*format == '\0') break;
            if (*format == '%')
            {
                _stream->print(*format);
                continue;
            }
            if (*format == 's')
            {
                register char* s = (char*)va_arg(args, int);
                _stream->print(s);
                continue;
            }
            if (*format == 'd' || *format == 'i')
            {
                _stream->print(va_arg(args, int),DEC);
                continue;
            }
            if (*format == 'x')
            {
                _stream->print(va_arg(args, int),HEX);
                continue;
            }
            if (*format == 'X')
            {
                _stream->print("0x");
                _stream->print(va_arg(args, int),HEX);
                continue;
            }
            if (*format == 'b')
            {
                _stream->print(va_arg(args, int),BIN);
                continue;
            }
            if (*format == 'B')
            {
                _stream->print("0b");
                _stream->print(va_arg(args, int),BIN);
                continue;
            }
            if (*format == 'l')
            {
                _stream->print(va_arg(args, long),DEC);
                continue;
            }
            if (*format == 'f')
            {
                _stream->print(va_arg(args, double), 6);
                continue;
            }

            if (*format == 'c')
            {
                _stream->print(va_arg(args, int));
                continue;
            }
            if (*format == 't')
            {
                if (va_arg(args, int) == 1)
                {
                    _stream->print("T");
                }
                else
                {
                    _stream->print("F");
                }
                continue;
            }
            if (*format == 'T')
            {
                if (va_arg(args, int) == 1)
                {
                    _stream->print("true");
                }
                else
                {
                    _stream->print("false");
                }
                continue;
            }
        }
        _stream->print(*format);
    }
}

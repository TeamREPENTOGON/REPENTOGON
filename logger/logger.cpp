#include "logger/logger.h"

#include <cstdlib>
#include <ctime>
#include <stdexcept>

static const char* LogLevelToString(LogLevel level) {
    switch (level) {
    case LOG_TRACE:
        return "TRACE";

    case LOG_DEBUG:
        return "DEBUG";

    case LOG_INFO:
        return "INFO";

    case LOG_WARN:
        return "WARN";

    case LOG_ERROR:
        return "ERROR";

    case LOG_FATAL:
        return "FATAL";

    case LOG_ABORT:
        return "ABORT";

    default:
        return "UNK";
    }
}

Logger::Logger(LogLevel level, const char* prefix, std::initializer_list<ILogOutput*> outputs) : _level(level), _prefix(prefix) {
    for (ILogOutput* output: outputs) {
        _outputs.push_back(output);
    }
}

Logger::~Logger() {
    for (ILogOutput* output : _outputs) {
        output->Close();
        delete output;
    }
}

#define LOGGER_IMPLEMENT_FUNCTION(NAME, LEVEL) \
void Logger :: NAME (const char* fmt, ...) {\
    va_list va;\
    va_start(va, fmt);\
    Log(LEVEL, fmt, va);\
    va_end(va);\
}

LOGGER_IMPLEMENT_FUNCTION(Trace, LOG_TRACE)
LOGGER_IMPLEMENT_FUNCTION(Debug, LOG_DEBUG)
LOGGER_IMPLEMENT_FUNCTION(Info, LOG_INFO)
LOGGER_IMPLEMENT_FUNCTION(Warn, LOG_WARN)
LOGGER_IMPLEMENT_FUNCTION(Error, LOG_ERROR)
LOGGER_IMPLEMENT_FUNCTION(Fatal, LOG_FATAL)

void Logger::Abort(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    Log(LOG_ABORT, fmt, va);
    va_end(va);

    std::abort();
}

void Logger::Log(LogLevel level, const char* fmt, va_list va) {
    if (_level <= level) {
        std::time_t t = std::time(nullptr);
        std::tm* asTm = std::localtime(&t);

        size_t n = vsnprintf(nullptr, 0, fmt, va);
        constexpr size_t timeLength = 100;
        size_t prefixAndTime = _prefix.size() + timeLength;

        char* buffer = (char*)malloc(n + 1 + prefixAndTime);
        if (!buffer) {
            std::exit(-1);
        }
        
        char* position = buffer;
        position += strftime(buffer, timeLength, "[%m-%d-%Y %H:%M:%S] ", asTm);
        if (!_prefix.empty()) {
            position += sprintf(position, "[%s] ", _prefix.c_str());
        }
        position += sprintf(position, "[%s] ", LogLevelToString(level));
        vsprintf(position, fmt, va);

        for (ILogOutput* output : _outputs) {
            output->Log(level, buffer);
        }

        free(buffer);
    }
}

void StdoutLogOutput::Log(LogLevel level, const char* text) {
    if (level <= LOG_INFO) {
        printf("%s", text);
    }
}

void StdoutLogOutput::Close() {
    fflush(stdout);
}

void StderrLogOutput::Log(LogLevel level, const char* text) {
    if (level >= LOG_WARN) {
        fprintf(stderr, "%s", text);
    }
}

FileLogOutput::FileLogOutput(const char* file, const char* mode, bool flush) {
    _file = fopen(file, mode);
    _flush = flush;

    if (!_file) {
        _file = stdout;
    }
}

void FileLogOutput::Log(LogLevel, const char* text) {
    fprintf(_file, "%s", text);
}

void FileLogOutput::Close() {
    fflush(_file);
    if (_file != stdout) {
        fclose(_file);
    }
}
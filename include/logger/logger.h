#pragma once

#include <cstdarg>
#include <cstdio>
#include <initializer_list>
#include <string>
#include <vector>

enum LogLevel {
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
    LOG_ABORT
};

class ILogOutput {
public:
    virtual void Log(LogLevel level, const char* text) = 0;
    virtual void Close() = 0;
};

class StdoutLogOutput : public ILogOutput {
public:
    void Log(LogLevel level, const char* text) override;
    void Close() override;
};

class StderrLogOutput : public ILogOutput {
public:
    void Log(LogLevel level, const char* text) override;
    void Close() override { }
};

class FileLogOutput : public ILogOutput {
public:
    FileLogOutput(const char* file, const char* mode, bool flush = false);
    void Log(LogLevel level, const char* text) override;
    void Close() override;

private:
    FILE* _file;
    bool _flush;
};

class Logger {
public:
    Logger(LogLevel level, const char* prefix, std::initializer_list<ILogOutput*> outputs);
    ~Logger();

    void Trace(const char* fmt, ...);
    void Debug(const char* fmt, ...);
    void Info(const char* fmt, ...);
    void Warn(const char* fmt, ...);
    void Error(const char* fmt, ...);
    void Fatal(const char* fmt, ...);
    void Abort(const char* fmt, ...);

private:
    void Log(LogLevel level, const char* fmt, va_list va);

    LogLevel _level = LOG_INFO;
    std::string _prefix = "";
    std::vector<ILogOutput*> _outputs;
};
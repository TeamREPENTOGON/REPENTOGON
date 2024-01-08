#pragma once

#include <cstdarg>
#include <cstdio>

enum LogLevel {
	LOG_TRACE,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_CRITICAL,
	LOG_FATAL
};

class Logger {
private:
	Logger();
	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(Logger const&) = delete;
	Logger& operator=(Logger&&) = delete;

	LogLevel _level = LOG_DEBUG;
	FILE* _file = NULL;
	// Call exit(-1) after a fatal error is logged
	bool _killOnFatal = false;
	// Call fflush on the output file after every log
	bool _flushOnLog = false;
	// Close the log file after a call to SetOutputFile
	bool _closeOnChange = false;

public:
	static Logger* instance() {
		static Logger logger;
		return &logger;
	}

	void SetLogLevel(LogLevel level);
	void SetOutputFile(const char* name, const char* mode, bool closeOnChange);
	void SetOutputFile(FILE* file, bool closeOnChange);
	void SetKillOnFatal(bool on);
	void SetFlushOnLog(bool on);

	void Log(LogLevel level, const char* fmt, va_list va);
	void Log(LogLevel level, const char* fmt, ...);
	void Trace(const char* fmt, ...);
	void Debug(const char* fmt, ...);
	void Info(const char* fmt, ...);
	void Warn(const char* fmt, ...);
	void Error(const char* fmt, ...);
	void Critical(const char* fmt, ...);
	void Fatal(const char* fmt, ...);
};

#define sLogger Logger::instance()
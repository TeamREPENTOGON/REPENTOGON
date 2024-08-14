#include <cstdlib>
#include <ctime>

#include "launcher/Logger.h"

static const char* LogLevelToString(LogLevel level);

Logger::Logger() {

}

void Logger::SynchronizeWithStdout(bool on) {
	_synchronizeWithStdout = on;
}

void Logger::SetLogLevel(LogLevel level) {
	_level = level;
}

void Logger::SetOutputFile(const char* name, const char* mode, bool closeOnChange) {
	FILE* f = fopen(name, mode);
	if (!f) {
		Log(LOG_ERROR, "[Logger] Unable to new log file %s with mode %s, ignoring\n", name, mode);
		return;
	}

	if (_file && _closeOnChange) {
		fclose(_file);
	}

	_file = f;
	_closeOnChange = closeOnChange;
}

void Logger::SetOutputFile(FILE* f, bool closeOnChange) {
	if (!f) {
		Log(LOG_ERROR, "[Logger] SetOutputFile called with NULL output file, ignoring\n");
		return;
	}

	if (_closeOnChange) {
		fclose(_file);
	}

	_file = f;
	_closeOnChange = closeOnChange;
}

void Logger::SetKillOnFatal(bool on) {
	_killOnFatal = on;
}

void Logger::SetFlushOnLog(bool on) {
	_flushOnLog = on;
}

void Logger::Log(LogLevel level, const char* fmt, va_list va) {
	if (_level > level)
		return;

	char buffer[4096];
	time_t now = time(nullptr);
	struct tm* tm = localtime(&now);
	strftime(buffer, 4095, "[%Y-%m-%d %H:%M:%S]", tm);
	fprintf(_file, "%s %s ", LogLevelToString(level), buffer);
	vfprintf(_file, fmt, va);

	if (_synchronizeWithStdout) {
		FILE* output = stdout;
		if (level >= LOG_WARN) {
			output = stderr;
		}

		fprintf(output, "%s %s ", LogLevelToString(level), buffer);
		vfprintf(output, fmt, va);
	}
	va_end(va);

	if (_flushOnLog) {
		fflush(_file);
	}
}

#define LOG(level) va_list va; \
	va_start(va, fmt); \
	Log(level, fmt, va)

void Logger::Log(LogLevel level, const char* fmt, ...) {
	LOG(level);
}

void Logger::Trace(const char* fmt, ...) {
	LOG(LOG_TRACE);
}

void Logger::Debug(const char* fmt, ...) {
	LOG(LOG_DEBUG);
}

void Logger::Info(const char* fmt, ...) {
	LOG(LOG_INFO);
}

void Logger::Warn(const char* fmt, ...) {
	LOG(LOG_WARN);
}

void Logger::Error(const char* fmt, ...) {
	LOG(LOG_ERROR);
}

void Logger::Critical(const char* fmt, ...) {
	LOG(LOG_CRITICAL);
}

void Logger::Fatal(const char* fmt, ...) {
	LOG(LOG_FATAL);

	if (_killOnFatal) {
		exit(EXIT_FAILURE);
	}
}

const char* LogLevelToString(LogLevel level) {
	switch (level) {
	case LOG_TRACE:
		return "[TRACE]";

	case LOG_DEBUG:
		return "[DEBUG]";

	case LOG_INFO:
		return "[INFO]";

	case LOG_WARN:
		return "[WARN]";

	case LOG_ERROR:
		return "[ERROR]";

	case LOG_CRITICAL:
		return "[CRITICAL]";

	case LOG_FATAL:
		return "[FATAL]";

	default:
		return "[UNKNOWN]";
	}
}
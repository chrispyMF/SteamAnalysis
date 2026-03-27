# pragma once

#include <fstream>
#include <mutex>
#include <string>
#include <iostream>
#include <SteamAnalysis/Helper.h>

enum class LogLevel {
	Debug,
	Info,
	Warn,
	Error
};

class Logger {
public:
	explicit Logger(LogLevel minLevel, const std::string& filepath);

	void debug(const std::string& message);
	void info(const std::string& message);
	void warn(const std::string& message);
	void error(const std::string& message);

private:
	LogLevel minLevel_;
	std::ofstream file_;
	std::mutex mutex_;

	bool shouldLog(LogLevel level) const;
	std::string levelToString(LogLevel level) const;
	std::string timestamp() const;
	void log(LogLevel level, const std::string& message);
};
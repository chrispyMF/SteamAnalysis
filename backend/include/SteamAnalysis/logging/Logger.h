# pragma once

#include <fstream>
#include <mutex>
#include <string>
#include <iostream>
#include <SteamAnalysis/Helper.h>

/// Specify how much information you want to be logged.  
enum class LogLevel {
	Debug,
	Info,
	Warn,
	Error
};

/// Class used to log information like debug info, errors, or warnings. 
class Logger {
public:
	explicit Logger(LogLevel minLevel, const std::string& filepath);
	
	/// Log detailed developer info.
	/// @param message Message string that will be added to the log in the log file.
	void debug(const std::string& message);

	/// Log normal successful events.
	/// @param message Message string that will be added to the log in the log file. 
	void info(const std::string& message);

	/// Log unexpected but recoverable events.
	/// @param message Message string that will be added to the log in the log file.
	void warn(const std::string& message);

	/// Log failed operations.
	/// @param message Message string that will be added to the log in the log file.
	void error(const std::string& message);

private:
	LogLevel minLevel_;
	std::ofstream file_;
	std::mutex mutex_;

	bool shouldLog(LogLevel level) const;
	std::string logLevelToString(LogLevel level) const;
	std::string timestamp() const;

	/// Print log info and write to log file.
	/// @param level LogLevel enum to check if we should log based on the minimum log level specified. 
	/// @param message Message string that will be added to the log in the log file 
	void log(LogLevel level, const std::string& message);
};
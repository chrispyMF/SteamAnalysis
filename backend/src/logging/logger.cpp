#include <SteamAnalysis/logging/logger.h>

Logger::Logger(LogLevel minLevel, const std::string& filePath)
	: minLevel_(minLevel) {
	if (!filePath.empty()) {
		file_.open(filePath, std::ios::app);
	}
}

void Logger::debug(const std::string& message) {
	log(LogLevel::Debug, message);
}

void Logger::info(const std::string& message) {
	log(LogLevel::Info, message);
}

void Logger::warn(const std::string& message) {
	log(LogLevel::Warn, message);
}

void Logger::error(const std::string& message) {
	log(LogLevel::Error, message);
}

bool Logger::shouldLog(LogLevel level) const {
	return static_cast<int>(level) >= static_cast<int>(minLevel_);
}

std::string Logger::levelToString(LogLevel level) const {
	switch (level) {
		case LogLevel::Debug:
			return "DEBUG";
		case LogLevel::Error:
			return "ERROR";
		case LogLevel::Info:
			return "INFO";
		case LogLevel::Warn:
			return "WARN";
	}
	return "UNKNOWN";
}

std::string Logger::timestamp() const {
	std::time_t now = std::time(nullptr);
	std::tm localtime{};
#ifdef _WIN32
	localtime_s(&localtime, &now);
#else
	localtime_r(&now, &localtime);
#endif
	
	char buffer[32];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d, $%H:%M:%S", &localtime);
	return buffer;
}

void Logger::log(LogLevel level, const std::string& message) {
	if (!shouldLog(level)) {
		return;
	}

	std::lock_guard<std::mutex> lock(mutex_);
	std::string line = "[" + timestamp() + "] [" + levelToString(level) + "] " + message;

	std::cout << line << std::endl;
	if (file_.is_open()) {
		file_ << line << std::endl;
		file_.flush();
	}
}
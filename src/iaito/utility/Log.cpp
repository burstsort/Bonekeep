#include "iaito/utility/Log.hpp"

#include <ctime>

namespace iaito
{
	std::string Log::_filename;
	LogLevel Log::_reportingLevel = Error;

	Log::Log(LogLevel level)
	{
		_logLevel = level;

		if (_logLevel > _reportingLevel || _filename.empty())
			return;

		time_t t = std::time(0);
		struct tm * now = localtime( & t );

		std::ofstream file(_filename.c_str(), std::fstream::app);
		char buffer[9];
		strftime (buffer, 9, "%H:%M:%S", now);
		file << buffer;

		switch (_logLevel)
		{
			case Error:
				file << " -ERR- ";
				break;
			case Warning:
				file << " -WRN- ";
				break;
			case Info:
				file << " -INF- ";
				break;
			case Debug:
				file << " -DBG- ";
				break;
		}

		file.close();
	}

	Log::~Log()
	{
		if (_logLevel > _reportingLevel || _filename.empty())
			return;

		std::ofstream file(_filename.c_str(), std::fstream::app);
		file << std::endl;
		file.close();
	}

	void Log::Init(LogLevel level, const std::string& filename)
	{
		_reportingLevel = level;
		_filename = filename;

		if (_filename.empty())
			return;

		time_t t = std::time(0);
		struct tm * now = localtime( & t );
		
		std::ofstream file(_filename.c_str(), std::fstream::trunc);
		char buffer[11];
		strftime (buffer, 11, "%Y-%m-%d", now);
		file << buffer << std::endl;

		file.close();
	}
}
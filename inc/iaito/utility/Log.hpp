#ifndef IAITO_LOG_HPP
#define IAITO_LOG_HPP

#include <string>
#include <fstream>

namespace iaito
{
	enum LogLevel 
	{
		Error,
		Warning,
		Info,
		Debug,
	};

	class Log
	{
		public:
			Log(LogLevel level);
			~Log();

			static void Init(LogLevel level = Info, const std::string& filename = "log.txt");

			template <typename T>
			Log& operator<<(T message)
			{
				if (_logLevel > _reportingLevel || _filename.empty())
					return *this;

				std::ofstream file(_filename.c_str(), std::fstream::app);
				file << message;
				file.close();

				return *this;
			}

		private:
			Log(const Log& that);
			Log& operator=(const Log& that);

			static std::string _filename;
			static LogLevel _reportingLevel;
			LogLevel _logLevel;
	};
}

#endif // IAITO_LOG_HPP
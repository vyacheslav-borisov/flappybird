#include "log.h"

#include <stdarg.h>
#include <android/log.h>

namespace pegas
{
		void Log::error(const char* tag, const char* message, ...)
		{
			va_list args;
			va_start(args, message);

			__android_log_vprint(ANDROID_LOG_ERROR, tag, message, args);
			__android_log_print(ANDROID_LOG_ERROR, tag, "\n");

			va_end(args);
		}

		void Log::warning(const char* tag, const char* message, ...)
		{
			va_list args;
			va_start(args, message);

			__android_log_vprint(ANDROID_LOG_WARN, tag, message, args);
			__android_log_print(ANDROID_LOG_WARN, tag, "\n");

			va_end(args);
		}

		void Log::info(const char* tag, const char* message, ...)
		{
			va_list args;
			va_start(args, message);

			__android_log_vprint(ANDROID_LOG_INFO, tag, message, args);
			__android_log_print(ANDROID_LOG_INFO, tag, "\n");

			va_end(args);
		}

		void Log::debug(const char* tag, const char* message, ...)
		{
			va_list args;
			va_start(args, message);

			__android_log_vprint(ANDROID_LOG_DEBUG, tag, message, args);
			__android_log_print(ANDROID_LOG_DEBUG, tag, "\n");

			va_end(args);
		}
}



#ifndef PEGAS_LOG_H_
#define PEGAS_LOG_H_

namespace pegas
{
	class Log
	{
	public:
		static void error(const char* tag, const char* message, ...);
		static void warning(const char* tag, const char* message, ...);
		static void info(const char* tag, const char* message, ...);
		static void debug(const char* tag, const char* message, ...);
	};
}

#ifndef NDEBUG

	#define LOGE(...) pegas::Log::error("Pegas_debug", __VA_ARGS__)
	#define LOGW(...) pegas::Log::warning("Pegas_debug", __VA_ARGS__)
	#define LOGI(...) pegas::Log::info("Pegas_debug", __VA_ARGS__)
	#define LOGD(...) pegas::Log::debug("Pegas_debug", __VA_ARGS__)

	#define LOGE_LOOP(...) pegas::Log::error("Pegas_loop", __VA_ARGS__)
	#define LOGW_LOOP(...) pegas::Log::warning("Pegas_loop", __VA_ARGS__)
	#define LOGI_LOOP(...) pegas::Log::info("Pegas_loop", __VA_ARGS__)
	#define LOGD_LOOP(...) pegas::Log::debug("Pegas_loop", __VA_ARGS__)

	/*#define LOGE(...)
	#define LOGW(...)
	#define LOGI(...)
	#define LOGD(...)

	#define LOGE_LOOP(...)
	#define LOGW_LOOP(...)
	#define LOGI_LOOP(...)
	#define LOGD_LOOP(...)*/

	#define LOGE_TAG(...) pegas::Log::error(__VA_ARGS__)
	#define LOGW_TAG(...) pegas::Log::warning(__VA_ARGS__)
	#define LOGI_TAG(...) pegas::Log::info(__VA_ARGS__)
	#define LOGD_TAG(...) pegas::Log::debug(__VA_ARGS__)
#else

	#define LOGE(...)
	#define LOGW(...)
	#define LOGI(...)
	#define LOGD(...)

	#define LOGE_LOOP(...)
	#define LOGW_LOOP(...)
	#define LOGI_LOOP(...)
	#define LOGD_LOOP(...)

	#define LOGE_TAG(...)
	#define LOGW_TAG(...)
	#define LOGI_TAG(...)
	#define LOGD_TAG(...)
#endif


#endif /* LOG_H_ */

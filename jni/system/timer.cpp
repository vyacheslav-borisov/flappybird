#include "../common.h"

#include "timer.h"
#include "log.h"

#include <time.h>

namespace pegas
{
	Timer::Timer()
		:m_elapsed(0.0f), m_lastTime(0.0f)
	{
		LOGI("Timer constructor");
	}

	void Timer::reset()
	{
		m_elapsed = 0.0f;
		m_lastTime = now();
	}

	void Timer::update()
	{
		double currentTime = now();
		m_elapsed = (float)(currentTime - m_lastTime);
	}

	double Timer::now()
	{
		timespec timeValue;
		clock_gettime(CLOCK_MONOTONIC, &timeValue);

		double result = timeValue.tv_sec + (timeValue.tv_nsec * 1.0e-9);

		return result;
	}

	float Timer::elapsed()
	{
		return m_elapsed;
	}
}




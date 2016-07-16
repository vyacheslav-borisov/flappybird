#ifndef PEGAS_TIMER_H_
#define PEGAS_TIMER_H_


namespace pegas
{
	class Timer
	{
	public:
		Timer();

		void reset();
		void update();
		double now();
		float elapsed();

	private:
		float m_elapsed;
		double m_lastTime;
	};
}

#endif /* TIMER_H_ */

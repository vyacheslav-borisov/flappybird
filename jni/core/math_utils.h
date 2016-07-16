#pragma once

namespace pegas
{
	class Math
	{
	public:
		static const float PI;

		// Generate a random number between 0 and 1
		// return a uniform number in [0,1].
		static float rand();

		// Generate a random number in a real interval.
		// param a one end point of the interval
		// param b the other end of the interval
		// return a inform rand numberin [a,b].
		static float rand(float a, float b);
	};
}

#include "../core/math_utils.h"

#include "../common.h"


namespace pegas
{
	const float Math::PI = 3.14159265f;

	// Generate a random number between 0 and 1
	// return a uniform number in [0,1].
	float Math::rand()
	{
		return ::rand() / float(RAND_MAX);
	}
	
	// Generate a random number in a real interval.
	// param a one end point of the interval
	// param b the other end of the interval
	// return a inform rand numberin [a,b].
	float Math::rand(float a, float b)
	{
		return (b-a) * Math::rand() + a;
	}
}

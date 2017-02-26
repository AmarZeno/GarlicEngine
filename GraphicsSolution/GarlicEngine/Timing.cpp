#include "Timing.h"
#include <windows.h>

#define DESIRED_FPS		60.0f
#define DESIRED_FRAMETIME_MS	(1000.0f / DESIRED_FPS)
#define MAX_FRAMETIME_MS	(2 * DESIRED_FRAMETIME_MS)
#define CONSTANT_FRAMETIME

namespace Timing
{
	long g_LastFrameStartTick = 0;

	double GetCounter();
	double GetFrequency();


	float CalcLastFrameTime()
	{
		float g_LastFrameTime_ms;

		// Grab the CPU tick counter
		double currentTick = GetCounter();
		if (g_LastFrameStartTick) {
			// How many ticks have elapsed since we last did this

			double elapsedTicks = (currentTick - g_LastFrameStartTick);
			g_LastFrameTime_ms = static_cast<float>(elapsedTicks / (GetFrequency()/100000));
		}
		else {
			// Calculate the frame time - converting ticks to ms.

			g_LastFrameTime_ms = 13.3f; // Assume a 60Hz frame for first call.
		}

		// Note start of this frame
		g_LastFrameStartTick = static_cast<long>(currentTick);


#if defined( CONSTANT_FRAMETIME )
		return DESIRED_FRAMETIME_MS;
#elif defined( CLAMP_FRAMETIME )
		if (g_LastFrameTime_ms > MAX_FRAMETIME_MS)
			return MAX_FRAMETIME_MS;
		else
			return g_LastFrameTime_ms;
#else
		return g_LastFrameTime_ms;
#endif
	}

	double GetCounter() {
		LARGE_INTEGER m_lpPerfomanceCount;
		QueryPerformanceCounter(&m_lpPerfomanceCount);
		return static_cast<long>(m_lpPerfomanceCount.QuadPart);
	}

	double GetFrequency() {
		LARGE_INTEGER m_lpFrequency;
		QueryPerformanceFrequency(&m_lpFrequency);
		return static_cast<long>(m_lpFrequency.QuadPart);
	}
}

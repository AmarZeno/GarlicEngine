#pragma once

#include "Utilities.h"
#include <math.h>

inline bool IsNaN(const double i_value) 
{
	volatile double val = i_value;
	return val != val;
}

inline bool AreAboutEqual(const double i_lhs, const double i_rhs) {
	const float epsilon = .0001f;
	return fabs(i_rhs - i_lhs) < epsilon;
}
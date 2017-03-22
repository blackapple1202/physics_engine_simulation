#pragma once

#ifndef CYCLONE_PRECISION_H
#define CYCLONE_PRECISION_H

#include <float.h>

namespace cyclone {
#if 0
	#define SINGLE_PRECISION
	typedef float real;

#else
	#define DOUBLE_PRECISION
	typedef double real;
	#define REAL_MAX DBL_MAX
	#define real_sqrt sqrt
	#define real_abs fabs
	#define real_sin sin
	#define real_cos cos
	#define real_exp exp
	#define real_pow pow
	#define real_fmod fmod
	#define real_epsilon DBL_EPSILON
	#define R_PI 3.14159265358979
#endif
}


#endif // !CYCLONE_PRECISION_H

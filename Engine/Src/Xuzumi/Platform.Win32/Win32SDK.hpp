#pragma once

#ifndef NOMINMAX
# define UNDEFINE_NOMINMAX_LATER
#	define NOMINMAX
#endif

#include <Windows.h>

#ifdef UNDEFINE_NOMINMAX_LATER
#	undef UNDEFINE_NOMINMAX_LATER
#	undef NOMINMAX
#endif

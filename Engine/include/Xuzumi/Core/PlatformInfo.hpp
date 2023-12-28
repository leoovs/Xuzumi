#pragma once

#define XZ_PLATFORM_ID_WIN32 0

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
# define XZ_PLATFORM_ID XZ_PLATFORM_ID_WIN32	
#else
#	error "Xuzumi: currently, Windows is the only supported platform"
#endif

#define XZ_PLATFORM_IS(platform) \
	(XZ_PLATFORM_ID == XZ_PLATFORM_ID_##platform)

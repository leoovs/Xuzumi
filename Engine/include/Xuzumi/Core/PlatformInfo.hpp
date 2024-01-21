#pragma once

/**
 * @def XZ_PLATFORM_ID_WIN32
 *
 * @brief Identifier for the WIN32 platform.
 */
#define XZ_PLATFORM_ID_WIN32 0

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
# define XZ_PLATFORM_ID XZ_PLATFORM_ID_WIN32
#else
#	error "Xuzumi: currently, Windows is the only supported platform"
#endif

/**
 * @def XZ_PLATFORM_IS(platform)
 *
 * @brief Checks whether the specified @p platform is the target platform.
 *
 * @param platform Name of the platform, must be one of the following:
 * - `WIN32`
 */
#define XZ_PLATFORM_IS(platform) \
	(XZ_PLATFORM_ID == XZ_PLATFORM_ID_##platform)

#pragma once

#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 0

#if defined(_WIN32)
#	undef PLATFORM_WINDOWS
#	define PLATFORM_WINDOWS 1
#	include <SDKDDKVer.h>
#endif // _WIN32

// LINUX, ANDROID
#if defined(__linux__)
#	undef PLATFORM_LINUX
#	define PLATFORM_LINUX 1
#	include <sys/endian.h>
#	if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#		if __BYTE_ORDER == __LITTLE_ENDIAN
#			define __LITTLE_ENDIAN__ 1
#		elif __BYTE_ORDER == __BIG_ENDIAN
#			define __BIG_ENDIAN__ 1
#		endif
#	endif
#endif // __linux__

// Macros for byte order conversion.
// byte order should be defined, It will be verified at run-time.
#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#	if defined(_M_X64) || defined(_M_IX86) || defined(__i386__)
#		define __LITTLE_ENDIAN__ 1
#	elif defined(__ARMEB__)
#		define __BIG_ENDIAN__ 1
#	else
#		error "System byte order not defined."
#	endif
#endif

// Inline macros
#ifndef FORCEINLINE
#	if defined(_MSC_VER)
#		define FORCEINLINE __forceinline
#	else
#		define FORCEINLINE inline __attribute__((always_inline))
#	endif
#endif
#ifndef NOINLINE
#	if defined(_MSC_VER)
#		define NOINLINE __declspec(noinline)
#	else
#		define NOINLINE __attribute__((noinline))
#	endif
#endif
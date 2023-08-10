#pragma once

//// Possible Platform Values
//// -----------------------------------------------------------------------------------------------
#define ATOM_PLATFORM_WINDOWS 0
#define ATOM_PLATFORM_LINUX 1

//// Windows
//// -----------------------------------------------------------------------------------------------
#if defined(_WIN32)
#    if defined(ATOM_PLATFORM)
#        error ATOM_PLATFORM is already defined.

#    endif

#    define ATOM_PLATFORM ATOM_PLATFORM_WINDOWS

#endif

//// Linux
//// -----------------------------------------------------------------------------------------------
#if defined(__unix__)
#    if defined(ATOM_PLATFORM)
#        error ATOM_PLATFORM is already defined.

#    endif

#    define ATOM_PLATFORM ATOM_PLATFORM_LINUX

#endif

//// Fail Check
//// -----------------------------------------------------------------------------------------------
#if !defined(ATOM_PLATFORM)
#    error Could not determine value for ATOM_PLATFORM.

#endif

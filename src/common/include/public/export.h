/**
 * @author Cliff Foster (Nou) <cliff@idi-systems.com>
 *
 * Common defines for exporting to dynamic linked libraries.
 */
#pragma once

#include "platform_config.h"

#ifdef CBX_IS_DYNAMIC

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __attribute__((visibility("default")))
#endif // _WIN32

#else

#define DLLEXPORT

#endif // CBX_IS_DYNAMIC

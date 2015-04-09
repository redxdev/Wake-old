#pragma once

#define WAKE_VERSION ("1.0")
#define WAKE_VERSION_ID (1)

#if defined(WAKE_RELEASE)
#define WAKE_NO_EXIT_PAUSE
#endif

#if !defined(WAKE_NO_EXIT_PAUSE)
#define WAKE_EXIT_PAUSE
#endif
#pragma once

#if defined(WAKE_RELEASE)
#define WAKE_NO_EXIT_PAUSE
#endif

#if !defined(WAKE_NO_EXIT_PAUSE)
#define WAKE_EXIT_PAUSE
#endif
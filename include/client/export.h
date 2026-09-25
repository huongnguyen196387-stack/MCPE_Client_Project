#pragma once

#if defined(__GNUC__)
#define MCPE_CLIENT_API __attribute__((visibility("default")))
#else
#define MCPE_CLIENT_API
#endif

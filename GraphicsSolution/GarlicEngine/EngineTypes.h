
#ifndef __ENGINE_TYPES_H
#define __ENGINE_TYPES_H
#include <stdint.h>

#ifdef _WIN64
typedef uint64_t guint_t;
#else 
typedef uint32_t guint_t;
#endif

#endif // __ENGINE_TYPES_H
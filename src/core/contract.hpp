// core/contract.hpp

#pragma once

#include <cstdio>
#include <cstdlib>
#include <utility>

// --- Internal helper ---

#define FAIL(kind, cond, msg) \
    do { \
        std::fprintf(stderr, "%s failed: (%s)\n  message: %s\n  at %s:%d\n", \
                     kind, #cond, msg, __FILE__, __LINE__); \
        std::abort(); \
    } while (false)

// --- Always checked ---

#define REQUIRE(cond, msg) \
    do { if (!(cond)) FAIL("REQUIRE", cond, msg); } while (false)

#define ENSURE(cond, msg) \
    do { if (!(cond)) FAIL("ENSURE", cond, msg); } while (false)

#define INVARIANT(cond, msg) \
    do { if (!(cond)) FAIL("INVARIANT", cond, msg); } while (false)

#ifdef NDEBUG
	#define UNREACHABLE(msg) std::unreachable()
#else
	#define UNREACHABLE(msg) \
    do { \
        std::fprintf(stderr, "UNREACHABLE reached: %s\n  at %s:%d\n", msg, __FILE__, __LINE__); \
        std::abort(); \
    } while (false)
#endif

// --- Debug only ---

#ifdef NDEBUG
    #define ASSERT(cond, msg) ((void)0)
#else
    #define ASSERT(cond, msg) \
        do { if (!(cond)) FAIL("ASSERT", cond, msg); } while (false)
#endif
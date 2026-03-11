// core/contract.hpp

#pragma once

#include <cstdlib>
#include <iostream>

// --- Internal helper ---

#define FAIL(kind, cond, msg) \
    do { \
        std::cerr << kind << " failed: (" #cond ")" \
                  << "\n  message: " << msg \
                  << "\n  at " << __FILE__ << ":" << __LINE__ << "\n"; \
        std::abort(); \
    } while (false)

// --- Always checked ---

#define REQUIRE(cond, msg) \
    do { if (!(cond)) FAIL("REQUIRE", cond, msg); } while (false)

#define ENSURE(cond, msg) \
    do { if (!(cond)) FAIL("ENSURE", cond, msg); } while (false)

#define INVARIANT(cond, msg) \
    do { if (!(cond)) FAIL("INVARIANT", cond, msg); } while (false)

#define UNREACHABLE(msg) \
    do { \
        std::cerr << "UNREACHABLE reached: " << msg \
                  << "\n  at " << __FILE__ << ":" << __LINE__ << "\n"; \
        std::abort(); \
    } while (false)

// --- Debug only ---

#ifdef NDEBUG
    #define ASSERT(cond, msg) ((void)0)
#else
    #define ASSERT(cond, msg) \
        do { if (!(cond)) FAIL("ASSERT", cond, msg); } while (false)
#endif
#pragma once
#include "Strong.h"

#define STRONG_OPAQUE(name, type, ...)                                                             \
    struct name;                                                                                   \
    constexpr auto makeOpaqueType(Strong<type, struct name##Tag, ##__VA_ARGS__>)->name;            \
    struct name : Strong<type, name##Tag, ##__VA_ARGS__> {                                         \
        using Strong::Strong;                                                                      \
    }

STRONG_OPAQUE(PersonId, int);

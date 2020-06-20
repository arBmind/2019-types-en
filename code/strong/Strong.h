#pragma once

template<class T>
constexpr T *nullptr_to = nullptr;

namespace strong {

struct ADL;
/// true if T was defined as a strong type
template<class T>
constexpr auto is_strong = isStrong(nullptr_to<ADL>, nullptr_to<T>);

// tag::weak[]
template<class V>
struct Weak {
    V v{};
    constexpr Weak() = default;
    constexpr explicit Weak(V v)
        : v((V &&) v) {}
    bool operator==(const Weak &) const = default;
};
// end::weak[]

/// Defines a strong value type with name @param NAME that stores value of type @param VALUE and
/// optional tags
#define DEFINE_STRONG(NAME, VALUE)                                                                 \
    struct NAME : private strong::Weak<VALUE> {                                                    \
        using Weak::Weak;                                                                          \
        using Weak::v;                                                                             \
        bool operator==(const NAME &) const = default;                                             \
    };                                                                                             \
    constexpr inline auto isStrong(strong::ADL *, NAME *)->bool { return true; }                   \
    auto strongValueType(NAME *)->VALUE;                                                           \
    struct NAME

constexpr inline auto isStrong(ADL *, void *) -> bool { return false; }

template<class T>
using StrongValueType = decltype(strongValueType(nullptr_to<T>));

} // namespace strong

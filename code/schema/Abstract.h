#pragma once

namespace abstract {

// clang-format off

// schema primitives:
template<class...> struct AllOf {}; // struct
template<class...> struct OneOf {}; // variant
template<class...> struct SomeOf {};
// clang-format on

template<class Id, class Entity>
struct EntitySet {};

template<class Id, class Node>
struct OrderedTree {};

template<class T>
auto toComputedValues(T) -> AllOf<>; // Fallback

template<class T>
using Ptr = T *;

} // namespace abstract

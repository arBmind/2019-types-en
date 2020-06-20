#pragma once

namespace recursive {

// clang-format off

// schema primitives:
template<class...> struct AllOf {};
template<class...> struct OneOf {};
template<class> struct SequenceOf {};
// clang-format on

template<class Id, class Entity>
struct EntitySet {};

template<class Id, class Node>
struct Hierarchy {};

template<class>
constexpr auto is_recursive = false;

template<class... Ts>
constexpr auto is_recursive<AllOf<Ts...>> = true;
template<class... Ts>
constexpr auto is_recursive<OneOf<Ts...>> = true;
template<class T>
constexpr auto is_recursive<SequenceOf<T>> = true;
template<class Id, class E>
constexpr auto is_recursive<EntitySet<Id, E>> = true;
template<class Id, class N>
constexpr auto is_recursive<Hierarchy<Id, N>> = true;

template<class T>
auto computedValuesFor(T *) -> AllOf<>; // Fallback

} // namespace recursive

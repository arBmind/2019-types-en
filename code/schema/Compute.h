#pragma once
#include "Storage.h"

namespace compute {

using namespace recursive;
using storage::is_value;
using storage::StorageFor;

template<class... As, class... Bs>
auto join(AllOf<As...>, AllOf<Bs...>) -> AllOf<As..., Bs...>;

template<class A, class B>
using Join = decltype(join(std::declval<A>(), std::declval<B>()));

struct ADL {};
constexpr auto adl = ADL{};

template<class T>
using ComputedFor = decltype(computedFor(adl, ptr<T>));

template<class T>
using ComputedValuesFor = decltype(computedValuesFor(std::declval<T>()));

// Schema -> Computed Schema
template<class... Ts>
auto computedFor(ADL, AllOf<Ts...> *)
    -> Join<AllOf<ComputedFor<Ts>...>, ComputedValuesFor<AllOf<Ts...>>>;
// … keep remaining schema

template<class... Ts>
auto computedFor(ADL, OneOf<Ts...> *) -> OneOf<ComputedFor<Ts>...>;

template<class T>
auto computedFor(ADL, T *) -> std::enable_if_t<is_value<T>, T>;

template<class Id, class Entity>
auto computedFor(ADL, EntitySet<Id, Entity> *) -> EntitySet<Id, ComputedFor<Entity>>;

} // namespace compute

#pragma once
#include "Recursive.h"

#include <tuple>
#include <type_traits>
#include <variant>

namespace storage {

using namespace recursive;

template<class T>
auto storageFor(T);

template<class T>
using StorageFor = decltype(storageFor(std::declval<T>()));

template<class... Ts>
auto storageFor(AllOf<Ts...>) -> std::tuple<StorageFor<Ts>...>;

// using Test = StorageFor<AllOf<>>; // error

} // namespace storage

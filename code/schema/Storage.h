#pragma once
#include "../strong/Strong.h"
#include "Recursive.h"

#include <tuple>
#include <variant>
#include <vector>

// Storage ADL Boilerplate
namespace storage {

using namespace recursive;

struct ADL {};
constexpr auto adl = ADL{};

template<class T>
using StorageFor = decltype(storageFor(adl, ptr<T>));

// tag::abstracts[]
template<class... Ts>
auto storageFor(ADL, AllOf<Ts...> *) -> std::tuple<StorageFor<Ts>...>;

template<class... Ts>
auto storageFor(ADL, OneOf<Ts...> *) -> std::variant<StorageFor<Ts>...>;

template<class Id, class Entity>
auto storageFor(ADL, EntitySet<Id, Entity> *) -> std::vector<std::tuple<Id, StorageFor<Entity>>>;
// end::abstracts[]

// tag::values[]
// storage for values
template<class T>
constexpr bool is_value = [] {
    if constexpr (std::is_class_v<T>)
        return !std::is_empty_v<T>;
    else
        return std::is_enum_v<T>;
}();

template<class T>
auto storageFor(ADL, T *) -> std::enable_if_t<is_value<T>, T>;
// end::values[]
// end::abstracts[]

} // namespace storage

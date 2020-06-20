#pragma once
#include "../strong/Strong.h"
#include "Recursive.h"
#include "Storage.h"

#include <optional>
#include <tuple>
#include <variant>
#include <vector>

namespace command {

using namespace recursive;
using storage::is_value;
using storage::StorageFor;

struct ADL;
template<class T>
using CommandFor = decltype(commandFor(nullptr_to<ADL>, nullptr_to<T>));

template<class... Ts>
auto commandFor(ADL *, AllOf<Ts...> *) -> std::tuple<CommandFor<Ts>...>;

template<class T>
auto commandFor(ADL *, T *) -> std::enable_if_t<is_value<T>, std::optional<T>>;

template<class Entity>
using EntityCreate = StorageFor<Entity>;
template<class Id>
using EntityDestroy = Id;
template<class Id, class Entity>
using EntityUpdate = std::tuple<Id, CommandFor<Entity>>;

template<class Id, class Entity>
auto commandFor(ADL *, EntitySet<Id, Entity> *)
    -> std::variant<EntityCreate<Entity>, EntityDestroy<Id>, EntityUpdate<Id, Entity>>;

} // namespace command

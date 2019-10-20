#pragma once
#include "../strong/Strong.h"
#include "Abstract.h"
#include "Storage.h"

#include <optional>
#include <tuple>
#include <variant>
#include <vector>

namespace command {

using namespace abstract;
using storage::is_value;
using storage::ToStorage;

struct ADL {};

template<class T>
using ToCommand = decltype(toCommand(ADL{}, Ptr<T>{}));

template<class... Ts>
auto toCommand(ADL, AllOf<Ts...> *) -> std::tuple<ToCommand<Ts>...>;

template<class T>
auto toCommand(ADL, T *) -> std::enable_if_t<is_value<T>, std::optional<T>>;

template<class Entity>
using EntityCreate = ToStorage<Entity>;
template<class Id>
using EntityDestroy = Id;
template<class Id, class Entity>
using EntityUpdate = std::tuple<Id, ToCommand<Entity>>;

template<class Id, class Entity>
auto toCommand(ADL, EntitySet<Id, Entity> *)
    -> std::variant<EntityCreate<Entity>, EntityDestroy<Id>, EntityUpdate<Id, Entity>>;

} // namespace command

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

template<class Data>
using EntityCreate = ToStorage<Data>;
template<class Id>
using EntityDestroy = Id;
template<class Id, class Data>
using EntityUpdate = std::tuple<Id, ToCommand<Data>>;

template<class Id, class Data>
auto toCommand(ADL, EntitySet<Id, Data> *)
    -> std::variant<EntityCreate<Data>, EntityDestroy<Id>, EntityUpdate<Id, Data>>;

using storage::ParentId;
template<class Id>
using BeforeId = StrongAddTag<Id, struct BeforeIdTag>;

// clang-format off

// Commands for OrderedTree
template<class Id, class Node>
using TreeCreate = std::tuple<
    ParentId<Id>, BeforeId<Id>, ToStorage<OrderedTree<Id, Node>>>;

template<class Id, class Node>
using TreeUpdate = std::tuple<
    Id, std::variant<ToCommand<Node>>>;

template<class Id, class Node>
auto toCommand(ADL, OrderedTree<Id, Node>*)
    -> std::variant<
        TreeCreate<Id, Node>,                       // Create
        TreeUpdate<Id, Node>,                       // Update
        std::tuple<Id, ParentId<Id>, BeforeId<Id>>, // Move
        Id>;                                        // Destroy

} // namespace command

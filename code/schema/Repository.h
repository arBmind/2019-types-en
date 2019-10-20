#pragma once
#include "../strong/Strong.h"
#include "Abstract.h"
#include "Storage.h"

#include <map>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

namespace repository {

using namespace abstract;
using storage::is_value;
using storage::ToStorage;

struct ADL {};

template<class T>
using ToRepository = decltype(toRepository(ADL{}, Ptr<T>{}));

template<class... Ts>
auto toRepository(ADL, AllOf<Ts...> *) -> std::tuple<ToRepository<Ts>...>;

template<class T>
auto toRepository(ADL, T *) -> std::enable_if_t<is_value<T>, T>;

namespace simple {

// tag::simpleEntitySet[]
template<class Id, class Entity>
auto toRepository(ADL, EntitySet<Id, Entity> *) -> std::map<Id, ToRepository<Entity>>;
// end::simpleEntitySet[]

} // namespace simple

template<class Id>
struct Less {
    constexpr bool operator()(Id l, Id r) const noexcept { return l.v < r.v; }
};

// tag::entityRepository[]
template<class Id, class EntityRepo>
class EntityRepository {
    std::map<Id, EntityRepo, Less<Id>> m;

public:
    auto begin() const { return m.begin(); }
    auto end() const { return m.end(); }

    bool contains(Id) const;
    auto count() const { return m.size(); }
    auto operator[](Id) -> EntityRepo &;
    auto operator[](Id) const -> const EntityRepo &;
    auto createId() -> Id;
    void destroy(Id);
};

template<class Id, class Entity>
auto toRepository(ADL, EntitySet<Id, Entity> *) //
    -> EntityRepository<Id, ToRepository<Entity>>;
// end::entityRepository[]

// implementation
template<class Id, class EntityRepo>
bool EntityRepository<Id, EntityRepo>::contains(Id id) const {
    return m.find(id) != m.end();
}

template<class Id, class EntityRepo>
auto EntityRepository<Id, EntityRepo>::operator[](Id id) -> EntityRepo & {
    return m[id];
}
template<class Id, class EntityRepo>
auto EntityRepository<Id, EntityRepo>::operator[](Id id) const -> const EntityRepo & {
    return m[id];
}

template<class Id, class EntityRepo>
auto EntityRepository<Id, EntityRepo>::createId() -> Id {
    static auto id = 0;
    return Id{++id};
}

template<class Id, class EntityRepo>
auto EntityRepository<Id, EntityRepo>::destroy(Id id) -> void {
    m.erase(id);
}

} // namespace repository

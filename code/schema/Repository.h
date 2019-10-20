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
class EntitySetRepository {
    std::map<Id, EntityRepo, Less<Id>> m;

public:
    auto begin() const { return m.begin(); }
    auto end() const { return m.end(); }

    bool contains(Id id) const { return m.find(id) != m.end(); }
    auto count() const { return m.size(); }
    auto operator[](Id id) -> EntityRepo & { return m[id]; }
    auto operator[](Id id) const -> const EntityRepo & { return m[id]; }
    auto createId() -> Id {
        static auto id = 0;
        return Id{++id};
    }
    void destroy(Id id) { m.erase(id); }
};

template<class Id, class Entity>
auto toRepository(ADL, EntitySet<Id, Entity> *) //
    -> EntitySetRepository<Id, ToRepository<Entity>>;
// end::entityRepository[]

} // namespace repository

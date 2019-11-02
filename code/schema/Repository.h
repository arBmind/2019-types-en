#pragma once
#include "../strong/Strong.h"
#include "Recursive.h"
#include "Storage.h"

#include <map>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

namespace repository {

using namespace recursive;
using storage::is_value;
using storage::StorageFor;

struct ADL {};
constexpr auto adl = ADL{};

template<class T>
using RepositoryFor = decltype(repositoryFor(adl, ptr<T>));

template<class... Ts>
auto repositoryFor(ADL, AllOf<Ts...> *) -> std::tuple<RepositoryFor<Ts>...>;

template<class T>
auto repositoryFor(ADL, T *) -> std::enable_if_t<is_value<T>, T>;

namespace simple {

// tag::simpleEntitySet[]
template<class Id, class Entity>
auto repositoryFor(ADL, EntitySet<Id, Entity> *) -> std::map<Id, RepositoryFor<Entity>>;
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
auto repositoryFor(ADL, EntitySet<Id, Entity> *) //
    -> EntitySetRepository<Id, RepositoryFor<Entity>>;
// end::entityRepository[]

} // namespace repository

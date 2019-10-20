#pragma once
#include "Command.h"
#include "Repository.h"

namespace processor {

using namespace abstract;
using command::ToCommand;
using repository::ToRepository;
using storage::is_value;
using storage::ToStorage;

// tag::boilerplate[]
struct ADL {};

template<class T>
constexpr auto to_command_processor = toCommandProcessor(ADL{}, Ptr<T>{});
// end::boilerplate[]

template<class... Ts>
constexpr auto toCommandProcessor(ADL, AllOf<Ts...> *) {
    return [](const ToCommand<AllOf<Ts...>> &cmd, ToRepository<AllOf<Ts...>> &repo) {
        return (to_command_processor<Ts>(std::get<ToCommand<Ts>>(cmd),
                                         std::get<ToRepository<Ts>>(repo)),
                ...);
    };
}

template<class T, std::enable_if_t<is_value<T>, void *> = nullptr>
constexpr auto toCommandProcessor(ADL, T *) {
    return [](const ToCommand<T> &cmd, ToRepository<T> &repo) {
        if (cmd) repo = *cmd;
    };
}

// clang-format off
template<class... Fs> struct Overloaded : Fs... { using Fs::operator()...; };
template<class... Fs> Overloaded(Fs...) -> Overloaded<Fs...>;
// clang-format on

template<class V, class... Fs>
auto oneVisit(V &&v, Fs &&... fs) {
    return std::visit(Overloaded{fs...}, v);
}

// tag::entitySet[]
template<class Id, class Entity>
constexpr auto toCommandProcessor(ADL, EntitySet<Id, Entity> *) {
    using T = EntitySet<Id, Entity>;
    return [](const ToCommand<T> &cmd, ToRepository<T> &repo) {
        oneVisit(
            cmd,
            [&repo](const command::EntityCreate<Entity> &storage) {
                repo[repo.createId()] = storage;
                // hint: to allow nesting you will need a storage processor
                // to_storage_processor<Data>(storage, repo[repo.createId()]);
            },
            [&repo](const command::EntityUpdate<Id, Entity> &update) {
                auto [id, dataCmd] = update;
                to_command_processor<Entity>(dataCmd, repo[id]);
            },
            [&repo](const command::EntityDestroy<Id> &destroy) {
                repo.destroy(destroy); //
            });
    };
}
// end::entitySet[]

} // namespace processor

#pragma once
#include "Command.h"
#include "Repository.h"

namespace processor {

using namespace recursive;
using command::CommandFor;
using repository::RepositoryFor;
using storage::is_value;
using storage::StorageFor;

// tag::boilerplate[]
struct ADL {};
constexpr auto adl = ADL{};

template<class T>
constexpr auto command_processor_for = commandProcessorFor(adl, ptr<T>);
// end::boilerplate[]

template<class... Ts>
constexpr auto commandProcessorFor(ADL, AllOf<Ts...> *) {
    return [](const CommandFor<AllOf<Ts...>> &cmd, RepositoryFor<AllOf<Ts...>> &repo) {
        return (command_processor_for<Ts>(std::get<CommandFor<Ts>>(cmd),
                                          std::get<RepositoryFor<Ts>>(repo)),
                ...);
    };
}

template<class T, std::enable_if_t<is_value<T>, void *> = nullptr>
constexpr auto commandProcessorFor(ADL, T *) {
    return [](const CommandFor<T> &cmd, RepositoryFor<T> &repo) {
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
constexpr auto commandProcessorFor(ADL, EntitySet<Id, Entity> *) {
    using T = EntitySet<Id, Entity>;
    return [](const CommandFor<T> &cmd, RepositoryFor<T> &repo) {
        oneVisit(
            cmd,
            [&repo](const command::EntityCreate<Entity> &storage) {
                repo[repo.createId()] = storage;
                // hint: to allow nesting you will need a storage processor
                // command_processor_for<Data>(storage, repo[repo.createId()]);
            },
            [&repo](const command::EntityUpdate<Id, Entity> &update) {
                auto [id, dataCmd] = update;
                command_processor_for<Entity>(dataCmd, repo[id]);
            },
            [&repo](const command::EntityDestroy<Id> &destroy) {
                repo.destroy(destroy); //
            });
    };
}
// end::entitySet[]

} // namespace processor

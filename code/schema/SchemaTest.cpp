#include "Command.h"
#include "Compute.h"
#include "PersonCompute.h"
#include "PersonSchema.h"
#include "Processor.h"
#include "Repository.h"

#include <iostream>

using namespace person;

using command::EntityCreate;
using command::CommandFor;
using processor::command_processor_for;
using repository::RepositoryFor;

using Command = CommandFor<Persons>;
using Repository = RepositoryFor<Persons>;
constexpr auto processCommand = command_processor_for<Persons>;

// nest++
void testCreate() {
    auto repo = Repository{};
    using CreateCmd = EntityCreate<PersonData>;
    auto cmd = CreateCmd{Name{"Bjarne Stroustrup"}, Role::Teacher};
    processCommand(cmd, repo);

    std::cout << std::get<Name>(repo[PersonId{1}]).v << "\n";
}

using compute::ComputedFor;
using OutPersons = ComputedFor<Persons>;

void testCompute() {
    using CreateCmd = EntityCreate<PersonData>;
    auto input = CreateCmd{Name{"Bjarne Stroustrup"}, Role::Teacher};
    auto output = Introduction{};
    computeComputed(input, output);

    std::cout << output.v << "\n";

    using OutCommand = CommandFor<OutPersons>;
    using OutCreateCommand = EntityCreate<ComputedFor<PersonData>>;
    using OutRepository = RepositoryFor<OutPersons>;
    constexpr auto processOutCommand = processor::command_processor_for<OutPersons>;

    OutCommand outCmd1 = OutCreateCommand{
        std::get<Name>(input), //
        std::get<Role>(input), //
        output                 //
    };
    OutRepository outRepo;
    processOutCommand(outCmd1, outRepo);

    std::cout << std::get<Introduction>(outRepo[PersonId{1}]).v << "\n";
}

int main() {
    testCreate();
    testCompute();
}

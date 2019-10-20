#pragma once
#include "../strong/Strong.h"
#include "Abstract.h"

#include <string>

namespace person {

using std::string;
using namespace abstract;

// tag::persons[]
// example usage:
using PersonId = Strong<int, struct PersonIdTag>;
using Name = Strong<string, struct NameTag>;
enum class Role { Unknown, Teacher, Student };

using PersonData = AllOf<Name, Role>;

using Persons = EntitySet<PersonId, PersonData>;
// end::persons[]

// tag::introduction[]
using Introduction = Strong<std::string, struct IntroductionTag>;

auto toComputedValues(PersonData) -> AllOf<Introduction>;
// end::introduction[]

} // namespace person

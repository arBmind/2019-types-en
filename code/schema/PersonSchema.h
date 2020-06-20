#pragma once
#include "../strong/Strong.h"
#include "Recursive.h"

#include <string>

namespace person {

using std::string;
using namespace recursive;

// tag::persons[]
// example usage:
DEFINE_STRONG(PersonId, int);
DEFINE_STRONG(Name, string);
enum class Role { Unknown, Teacher, Student };

using PersonData = AllOf<Name, Role>;

using Persons = EntitySet<PersonId, PersonData>;
// end::persons[]

// tag::introduction[]
DEFINE_STRONG(Introduction, string);

auto computedValuesFor(PersonData *) -> AllOf<Introduction>;
// end::introduction[]

} // namespace person

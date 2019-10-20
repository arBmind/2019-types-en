#pragma once
#include "PersonSchema.h"
#include "Storage.h"

#include <sstream>

namespace person {

using storage::ToStorage;

// tag::ansprache[]
inline void computeComputed(const ToStorage<PersonData> &s, Introduction &o) {
    auto [name, role] = s;
    auto out = std::stringstream{};
    switch (role) {
    case Role::Unknown: out << name.v << " plays an unknown role."; break;
    case Role::Teacher: out << name.v << " is a teacher."; break;
    case Role::Student: out << name.v << " is a student."; break;
    }
    o.v = out.str();
}
// end::ansprache[]

} // namespace person

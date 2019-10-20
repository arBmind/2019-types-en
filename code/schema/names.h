#include "PersonSchema.h"

#include "wobjectcpp.h"

namespace names {

using namespace person;

struct ADL {};

template<class T>
constexpr auto property_name = getPropertyName(ADL{}, Ptr<T>{});

template<class T>
constexpr auto property_changed_name = getPropertyChangedName(ADL{}, Ptr<T>{});

// tag::propertyNames[]
constexpr auto getPropertyName(ADL, Name *) { return w_cpp::viewLiteral("name"); }
constexpr auto getPropertyName(ADL, Role *) { return w_cpp::viewLiteral("role"); }

constexpr auto getPropertyChangedName(ADL, Name *) { return w_cpp::viewLiteral("nameChanged"); }
constexpr auto getPropertyChangedName(ADL, Role *) { return w_cpp::viewLiteral("roleChanged"); }
// end::propertyNames[]

} // namespace names

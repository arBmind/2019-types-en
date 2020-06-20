#include "Strong.h"

DEFINE_STRONG(Velocity, double);

int main() {
    using Distance = double; // Alias

    auto d = Distance{3} + 2; // Ok: just double

    // auto v = Velocity{3} + 2; // Error: no operator

    (void)d;
}

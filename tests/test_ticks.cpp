
#include "vizlib/ticks.hpp"

#include <cassert>
#include <stdexcept>

int main() {
    auto ticks = vizlib::generateTicks(0, 20, 5);

    assert(!ticks.empty());
    assert(ticks.front() == 0);
    assert(ticks.back() == 20);
    assert(ticks.size() == 5);

    auto smallTicks = vizlib::generateTicks(0, 1, 5);
    assert(!smallTicks.empty());

    bool errorThrown = false;

    try {
        vizlib::generateTicks(10, 10);
    } catch (const std::invalid_argument&) {
        errorThrown = true;
    }

    assert(errorThrown);
}

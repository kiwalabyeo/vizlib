#include "doctest.h"
#include <vizlib/version.hpp>
#include <string>

TEST_CASE("version returns the current version")
{
    CHECK(std::string(vizlib::version()) == "0.1.0");
}
// Crystal's ECDF tests (from tests_ecdf.cpp), rewritten for doctest so they
// run with the rest of the suite in CTest and CI.
#include "doctest.h"

#include <vizlib/ecdf.hpp>

#include <vector>

TEST_CASE("ECDF of an empty vector is empty")
{
    vizlib::ECDFPlot ecdf;
    CHECK(ecdf.compute_coordinates({}).empty());
}

TEST_CASE("ECDF sorts the data and steps up by 1/n")
{
    vizlib::ECDFPlot ecdf;
    const std::vector<vizlib::Point> result = ecdf.compute_coordinates({3.0, 1.0, 2.0});

    // Expected sorted order: 1.0, 2.0, 3.0
    // Expected y values: 1/3, 2/3, 3/3 (1.0)
    REQUIRE(result.size() == 3);
    CHECK(result[0].x == 1.0);
    CHECK(result[0].y == doctest::Approx(1.0 / 3.0));
    CHECK(result[1].x == 2.0);
    CHECK(result[1].y == doctest::Approx(2.0 / 3.0));
    CHECK(result[2].x == 3.0);
    CHECK(result[2].y == doctest::Approx(1.0));
}

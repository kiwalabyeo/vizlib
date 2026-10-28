#include "doctest.h"
#include"../include/vizlib/stats.hpp"
#include <vector>
void check_vectors_approx(const std::vector<double>& actual, const std::vector<double>& expected) {
    // Stop the test immediately if sizes don't match to prevent out-of-bounds errors
    REQUIRE(actual.size() == expected.size()); 
    
    // Compare each element with floating-point tolerance
    for (size_t i = 0; i < expected.size(); ++i) {
        CHECK(actual[i] == doctest::Approx(expected[i]));
    }
}
TEST_CASE("Testing summaryStatistics class") {
    std::vector<double> data = {1, 2, 2, 3, 4};
    vizlib::summaryStatistics stats(data);

    CHECK(stats.mean() == doctest::Approx(2.4));
    CHECK(stats.median() == doctest::Approx(2));
    CHECK(stats.range() == doctest::Approx(3));
    CHECK(stats.mode() == std::vector<double>{2});
    CHECK(stats.standard_deviation() == doctest::Approx(1.019803902718557));
    CHECK(stats.quantile(0.25) == doctest::Approx(2));
    CHECK(stats.quantile(0.75) == doctest::Approx(3));
    CHECK(stats.interquartile_range() == doctest::Approx(1));
    CHECK(stats.count(2) == 2);
    check_vectors_approx(stats.histogram_bin_edges(), {1.0, 2.16961, 3.33921, 4.50882});
    //CHECK(stats.histogram_bin_edges() == std::vector<double>{1, 2.16961, 3.33921, 4.50882});
}
TEST_CASE("Testing FreedmanDiaconisBinning and SturgesBinning classes") {
    std::vector<double> data = {1, 2, 2, 3, 4};
    vizlib::FreedmanDiaconisBinning fd_binning(data);
    vizlib::SturgesBinning sturges_binning(data);

    //CHECK(fd_binning.histogram_bin_edges() == std::vector<double>{1, 2.1696, 3.33921, 4.50882});
    check_vectors_approx(sturges_binning.histogram_bin_edges(), {1.0, 1.75, 2.5, 3.25, 4.0});
    check_vectors_approx(fd_binning.histogram_bin_edges(), {1.0, 2.16961, 3.33921, 4.50882});
}
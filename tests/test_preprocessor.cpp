#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include "data_path.hpp"
#include "doctest.h"
#include "vizlib/csv_reader.hpp"
#include "vizlib/preprocessing.hpp"

using namespace vizlib;

static DataFrame sample() {
    return parse_csv(
        "id,score,group\n"
        "1,10,a\n"
        "2,,b\n"
        "3,30,\n"
        "4,40,a\n"
        "5,NA,b\n");
}

TEST_CASE("parse_number") {
    double v = 0;
    CHECK(parse_number("12", v));
    CHECK(v == 12);
    CHECK(parse_number("-3.5", v));
    CHECK(v == -3.5);
    CHECK(parse_number("  2e3 ", v));
    CHECK(v == 2000);
    CHECK(parse_number(".5", v));
    CHECK(v == 0.5);
    CHECK_FALSE(parse_number("", v));
    CHECK_FALSE(parse_number("abc", v));
    CHECK_FALSE(parse_number("1,5", v));
    CHECK_FALSE(parse_number("1.2.3", v));
    CHECK_FALSE(parse_number("--1", v));
    CHECK_FALSE(parse_number("e", v));
    CHECK_FALSE(parse_number("+", v));
    CHECK_FALSE(parse_number("inf", v));
    CHECK_FALSE(parse_number("nan", v));
    CHECK_FALSE(parse_number("0x10", v));
    CHECK_FALSE(parse_number("1e999", v));
}

TEST_CASE("drop_missing: any column") {
    DataFrame df = sample();
    DataFrame out = drop_missing(df);
    CHECK(out.n_rows() == 2);
    CHECK(out["id"].values() == std::vector<double>{1, 4});
    CHECK(out["group"].text()[1] == "a");
    CHECK(df.n_rows() == 5);  // the input is untouched
}

TEST_CASE("drop_missing: chosen columns only") {
    DataFrame out = drop_missing(sample(), {"score"});
    CHECK(out.n_rows() == 3);
    CHECK(out["id"].values() == std::vector<double>{1, 3, 4});
    CHECK(out["group"].count_missing() == 1);  // still has a missing group
    CHECK_THROWS_AS(drop_missing(sample(), {"nope"}), std::invalid_argument);
}

TEST_CASE("drop_missing: nothing missing returns an equal frame; all missing gives zero rows") {
    DataFrame clean = parse_csv("a\n1\n2\n");
    CHECK(drop_missing(clean).n_rows() == 2);
    DataFrame allbad = parse_csv("a,b\n,\n,\n");
    DataFrame out = drop_missing(allbad);
    CHECK(out.n_rows() == 0);
    CHECK(out.n_cols() == 2);
}

TEST_CASE("fill_missing: numeric constant") {
    DataFrame out = fill_missing(sample(), "score", 0.0);
    CHECK(out["score"].count_missing() == 0);
    CHECK(out["score"].values() == std::vector<double>{10, 0, 30, 40, 0});
    CHECK_THROWS_AS(fill_missing(sample(), "group", 0.0), std::invalid_argument);
    CHECK_THROWS_AS(fill_missing(sample(), "nope", 0.0), std::invalid_argument);
}

TEST_CASE("fill_missing: text constant") {
    DataFrame out = fill_missing(sample(), "group", std::string("unknown"));
    CHECK(out["group"].count_missing() == 0);
    CHECK(out["group"].text()[2] == "unknown");
    CHECK_THROWS_AS(fill_missing(sample(), "score", std::string("x")), std::invalid_argument);
}

TEST_CASE("fill_missing_mean and median") {
    DataFrame m = fill_missing_mean(sample(), "score");  // (10+30+40)/3
    CHECK(m["score"].values()[1] == doctest::Approx(80.0 / 3.0));
    DataFrame d = fill_missing_median(sample(), "score");  // median of 10,30,40
    CHECK(d["score"].values()[1] == 30);

    DataFrame even = parse_csv("a,b\n1,x\n3,y\n,z\n");  // median of 1,3 is 2
    CHECK(fill_missing_median(even, "a")["a"].values()[2] == 2);

    DataFrame none = parse_csv("a,b\n,1\n,2\n");
    CHECK_THROWS_AS(fill_missing_mean(none, "a"), std::invalid_argument);
    CHECK_THROWS_AS(fill_missing_median(none, "a"), std::invalid_argument);
    CHECK_THROWS_AS(fill_missing_mean(sample(), "group"), std::invalid_argument);
}

TEST_CASE("to_numeric: converts text, strict by default") {
    // "1" "2" "x": reader makes this a text column
    DataFrame df = parse_csv("v\n1\n2\nx\n");
    CHECK_FALSE(df["v"].is_numeric());
    CHECK_THROWS_AS(to_numeric(df, "v"), std::invalid_argument);

    DataFrame out = to_numeric(df, "v", true);
    CHECK(out["v"].is_numeric());
    CHECK(out["v"].values()[0] == 1);
    CHECK(out["v"].is_missing(2));
}

TEST_CASE("to_numeric: keeps missing cells missing and ignores numeric columns") {
    DataFrame df = parse_csv("v,w\n1,5\nabc,\n\n2,7\n", CsvOptions{});
    // v has "abc" so it is text; row 3 is blank and skipped by the reader
    DataFrame out = to_numeric(df, "v", true);
    CHECK(out["v"].count_missing() == 1);
    DataFrame same = to_numeric(df, "w");
    CHECK(same["w"].is_numeric());
    CHECK_THROWS_AS(to_numeric(df, "nope"), std::invalid_argument);
}

TEST_CASE("filter_rows and filter_numeric") {
    DataFrame df = sample();
    DataFrame big = filter_numeric(df, "score", [](double s) { return s >= 30; });
    CHECK(big["id"].values() == std::vector<double>{3, 4});  // missing never passes

    DataFrame odd = filter_rows(df, [](const DataFrame& d, std::size_t r) {
        return static_cast<long>(d["id"].values()[r]) % 2 == 1;
    });
    CHECK(odd["id"].values() == std::vector<double>{1, 3, 5});

    CHECK_THROWS_AS(filter_numeric(df, "group", [](double) { return true; }), std::invalid_argument);
    CHECK(filter_rows(df, [](const DataFrame&, std::size_t) { return false; }).n_rows() == 0);
}

TEST_CASE("select_columns and drop_columns") {
    DataFrame df = sample();
    DataFrame sel = select_columns(df, {"group", "id"});
    CHECK(sel.column_names() == std::vector<std::string>{"group", "id"});
    CHECK(sel.n_rows() == 5);
    CHECK_THROWS_AS(select_columns(df, {"id", "nope"}), std::invalid_argument);
    CHECK_THROWS_AS(select_columns(df, {"id", "id"}), std::invalid_argument);

    DataFrame dropped = drop_columns(df, {"score"});
    CHECK(dropped.column_names() == std::vector<std::string>{"id", "group"});
    CHECK_THROWS_AS(drop_columns(df, {"nope"}), std::invalid_argument);
}

TEST_CASE("pipeline: read, clean, ready for plotting") {
    DataFrame df = read_csv(data_path("messy.csv"));
    df = drop_missing(df, {"score"});
    df = to_numeric(df, "age", true);
    CHECK(df["score"].count_missing() == 0);
    CHECK(df["score"].values().size() == df.n_rows());
}

TEST_CASE("handle_missing_values keeps working as a name for drop_missing") {
    DataFrame out = handle_missing_values(sample());
    CHECK(out.n_rows() == 2);
}

TEST_CASE("drop_nan leaves out NaN and keeps the order") {
    const double nan = std::numeric_limits<double>::quiet_NaN();
    CHECK(vizlib::drop_nan({3.0, nan, 1.0, nan, 2.0}) == std::vector<double>{3.0, 1.0, 2.0});
    CHECK(vizlib::drop_nan({}).empty());
    CHECK(vizlib::drop_nan({nan, nan}).empty());
}

TEST_CASE("drop_nan gives the same numbers as dropping missing rows") {
    const vizlib::DataFrame df = vizlib::parse_csv("x\n1\nNA\n3\nnull\n5\n");
    const vizlib::DataFrame cleaned = vizlib::drop_missing(df);
    const std::vector<double>& with_missing = df["x"];
    const std::vector<double>& dropped = cleaned["x"];
    CHECK(with_missing.size() == 5);
    CHECK(vizlib::drop_nan(with_missing) == dropped);
    CHECK(dropped == std::vector<double>{1.0, 3.0, 5.0});
}

// Tests for the numeric-type templates: numeric.hpp, Column::as<T>,
// Column(name, vector<T>), fill_missing(T), parse_number(T&), filter_numeric<T>,
// read_csv_column<T> and NumericColumn<T>.
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "data_path.hpp"
#include "doctest.h"
#include "vizlib/csv_reader.hpp"
#include "vizlib/dataframe.hpp"
#include "vizlib/numeric.hpp"
#include "vizlib/preprocessing.hpp"
#include "vizlib/typed_column.hpp"

using namespace vizlib;

namespace {
const double kNaN = std::numeric_limits<double>::quiet_NaN();
}

// --- the numeric trait / concept ---------------------------------------------

TEST_CASE("is_numeric_type_v: arithmetic types except bool") {
    static_assert(is_numeric_type_v<int>);
    static_assert(is_numeric_type_v<short>);
    static_assert(is_numeric_type_v<unsigned long long>);
    static_assert(is_numeric_type_v<float>);
    static_assert(is_numeric_type_v<double>);
    static_assert(is_numeric_type_v<long double>);
    static_assert(!is_numeric_type_v<bool>);
    static_assert(!is_numeric_type_v<std::string>);
    static_assert(!is_numeric_type_v<const char*>);
#if VIZLIB_HAS_CONCEPTS
    static_assert(Numeric<int>);
    static_assert(Numeric<float>);
    static_assert(!Numeric<bool>);
    static_assert(!Numeric<std::string>);
#endif
    CHECK(true);
}

TEST_CASE("is_representable: integer types") {
    CHECK(is_representable<int>(3.0));
    CHECK(is_representable<int>(-3.0));
    CHECK_FALSE(is_representable<int>(2.5));
    CHECK_FALSE(is_representable<int>(kNaN));
    CHECK_FALSE(is_representable<int>(std::numeric_limits<double>::infinity()));
    CHECK(is_representable<int>(2147483647.0));
    CHECK_FALSE(is_representable<int>(2147483648.0));
    CHECK(is_representable<int>(-2147483648.0));
    CHECK_FALSE(is_representable<int>(-2147483649.0));
    CHECK(is_representable<unsigned>(3e9));
    CHECK_FALSE(is_representable<unsigned>(-1.0));
    CHECK(is_representable<std::int8_t>(-128.0));
    CHECK_FALSE(is_representable<std::int8_t>(-129.0));
    CHECK(is_representable<std::uint8_t>(255.0));
    CHECK_FALSE(is_representable<std::uint8_t>(256.0));
    CHECK(is_representable<long long>(9.0e18));
    CHECK_FALSE(is_representable<long long>(9223372036854775808.0));  // 2^63
}

TEST_CASE("is_representable: floating types") {
    CHECK(is_representable<float>(1.5));
    CHECK(is_representable<float>(kNaN));
    CHECK_FALSE(is_representable<float>(1e39));
    CHECK(is_representable<double>(1e300));
    CHECK(is_representable<long double>(1e300));
}

TEST_CASE("to_double_exact") {
    CHECK(to_double_exact(5) == 5.0);
    CHECK(to_double_exact(2.5f) == 2.5);
    CHECK(to_double_exact(static_cast<long long>(9007199254740992LL)) == 9007199254740992.0);  // 2^53
    CHECK_THROWS_AS(to_double_exact(9007199254740993LL), std::invalid_argument);                // 2^53 + 1
    CHECK(std::isnan(to_double_exact(kNaN)));
}

// --- Column ------------------------------------------------------------------

TEST_CASE("Column: constructed from any numeric vector") {
    Column a("a", std::vector<int>{1, 2, 3});
    CHECK(a.is_numeric());
    CHECK(a.values() == std::vector<double>{1, 2, 3});

    Column b("b", std::vector<long>{-5, 0, 7});
    CHECK(b.values()[0] == -5.0);

    Column c("c", std::vector<float>{0.5f, 1.5f});
    CHECK(c.values()[1] == 1.5);

    Column d("d", std::vector<unsigned short>{10, 20});
    CHECK(d.values()[1] == 20.0);

    Column e("e", std::vector<std::uint8_t>{});
    CHECK(e.size() == 0);
}

TEST_CASE("Column: existing double and text constructors are unaffected") {
    Column a("a", std::vector<double>{1.5, 2.5});
    CHECK(a.values()[0] == 1.5);
    Column b("b", {1.0, 2.0, 3.0});
    CHECK(b.size() == 3);
    Column t("t", std::vector<std::string>{"x", "y"});
    CHECK_FALSE(t.is_numeric());
}

TEST_CASE("Column: 64-bit integers that a double cannot hold exactly throw") {
    CHECK_NOTHROW(Column("a", std::vector<long long>{9007199254740992LL}));
    CHECK_THROWS_AS(Column("a", std::vector<long long>{9007199254740993LL}), std::invalid_argument);
}

TEST_CASE("Column::as<T>: integer, floating and exact conversions") {
    Column c("c", std::vector<double>{1, 2, 3});
    CHECK(c.as<int>() == std::vector<int>{1, 2, 3});
    CHECK(c.as<short>() == std::vector<short>{1, 2, 3});
    CHECK(c.as<unsigned>() == std::vector<unsigned>{1, 2, 3});
    CHECK(c.as<long long>() == std::vector<long long>{1, 2, 3});
    CHECK(c.as<double>() == std::vector<double>{1, 2, 3});
    CHECK(c.as<float>() == std::vector<float>{1.f, 2.f, 3.f});
}

TEST_CASE("Column::as<T>: floating types keep NaN (missing)") {
    Column c("c", std::vector<double>{1.5, kNaN});
    std::vector<float> f = c.as<float>();
    CHECK(f[0] == 1.5f);
    CHECK(std::isnan(f[1]));
    std::vector<double> d = c.as<double>();
    CHECK(std::isnan(d[1]));
}

TEST_CASE("Column::as<T>: integer types refuse values that would change") {
    CHECK_THROWS_AS(Column("c", std::vector<double>{1.0, 2.5}).as<int>(), std::invalid_argument);
    CHECK_THROWS_AS(Column("c", std::vector<double>{1.0, kNaN}).as<int>(), std::invalid_argument);
    CHECK_THROWS_AS(Column("c", std::vector<double>{300.0}).as<std::int8_t>(), std::invalid_argument);
    CHECK_THROWS_AS(Column("c", std::vector<double>{-1.0}).as<unsigned>(), std::invalid_argument);
    CHECK_THROWS_AS(Column("c", std::vector<double>{3e9}).as<int>(), std::invalid_argument);
    CHECK_THROWS_AS(Column("c", std::vector<double>{1e39}).as<float>(), std::invalid_argument);
}

TEST_CASE("Column::as<T>: the error names the column and row") {
    try {
        Column("size", std::vector<double>{1.0, 2.5}).as<int>();
        FAIL("expected an exception");
    } catch (const std::invalid_argument& e) {
        const std::string msg = e.what();
        CHECK(msg.find("'size'") != std::string::npos);
        CHECK(msg.find("row 1") != std::string::npos);
    }
}

TEST_CASE("Column::as<T>: text column throws") {
    Column t("t", std::vector<std::string>{"a"});
    CHECK_THROWS_AS(t.as<int>(), std::invalid_argument);
    CHECK_THROWS_AS(t.as<double>(), std::invalid_argument);
}

TEST_CASE("DataFrame::column_as<T>") {
    DataFrame df;
    df.add_column(Column("n", std::vector<int>{4, 5, 6}));
    CHECK(df.column_as<long>("n") == std::vector<long>{4, 5, 6});
    CHECK_THROWS_AS(df.column_as<int>("nope"), std::invalid_argument);
}

// --- preprocessing templates ---------------------------------------------------

namespace {
DataFrame frame_with_missing() {
    DataFrame df;
    df.add_column(Column("age", std::vector<double>{20, kNaN, 40}));
    df.add_column(Column("name", std::vector<std::string>{"a", "", "c"},
                         std::vector<char>{0, 1, 0}));
    return df;
}
}  // namespace

TEST_CASE("fill_missing: any numeric type works, doubles and strings still work") {
    DataFrame df = frame_with_missing();
    CHECK(fill_missing(df, "age", 30)["age"].values()[1] == 30.0);          // int
    CHECK(fill_missing(df, "age", 30L)["age"].values()[1] == 30.0);         // long
    CHECK(fill_missing(df, "age", 30u)["age"].values()[1] == 30.0);         // unsigned
    CHECK(fill_missing(df, "age", 1.5f)["age"].values()[1] == 1.5);         // float
    CHECK(fill_missing(df, "age", 30.5)["age"].values()[1] == 30.5);        // double
    CHECK(fill_missing(df, "name", "none")["name"].text()[1] == "none");    // text
    CHECK(fill_missing(df, "name", std::string("x"))["name"].text()[1] == "x");
    CHECK(fill_missing(df, "age", 0)["age"].values()[1] == 0.0);            // literal 0
}

TEST_CASE("fill_missing: template errors") {
    DataFrame df = frame_with_missing();
    CHECK_THROWS_AS(fill_missing(df, "name", 5), std::invalid_argument);      // numeric into text
    CHECK_THROWS_AS(fill_missing(df, "nope", 5), std::invalid_argument);
    CHECK_THROWS_AS(fill_missing(df, "age", 9007199254740993LL), std::invalid_argument);
}

TEST_CASE("parse_number: into any numeric type") {
    int i = 0;
    CHECK(parse_number("42", i));
    CHECK(i == 42);
    CHECK(parse_number("1e3", i));  // a whole number written in exponent form
    CHECK(i == 1000);
    CHECK_FALSE(parse_number("2.5", i));
    CHECK_FALSE(parse_number("3000000000", i));
    CHECK_FALSE(parse_number("abc", i));
    CHECK_FALSE(parse_number("0x1A", i));

    unsigned u = 0;
    CHECK_FALSE(parse_number("-1", u));
    CHECK(parse_number("3000000000", u));
    CHECK(u == 3000000000u);

    float f = 0;
    CHECK(parse_number("2.5", f));
    CHECK(f == 2.5f);

    short s = 0;
    CHECK_FALSE(parse_number("40000", s));

    double d = 0;
    CHECK(parse_number("-7.25", d));  // the original overload
    CHECK(d == -7.25);
}

TEST_CASE("filter_numeric<T>: integer and floating predicates") {
    DataFrame df;
    df.add_column(Column("size", std::vector<double>{2, 3, kNaN, 4}));
    DataFrame big = filter_numeric<int>(df, "size", [](int s) { return s >= 3; });
    CHECK(big.n_rows() == 2);  // 3 and 4; the missing cell never passes
    CHECK(big["size"].values()[0] == 3.0);

    DataFrame f = filter_numeric<float>(df, "size", [](float s) { return s < 3.5f; });
    CHECK(f.n_rows() == 2);  // 2 and 3

    // The original non-template call (lambda taking double) is unchanged.
    CHECK(filter_numeric(df, "size", [](double x) { return x > 2; }).n_rows() == 2);
}

TEST_CASE("filter_numeric<T>: errors") {
    DataFrame df;
    df.add_column(Column("x", std::vector<double>{1.0, 2.5}));
    df.add_column(Column("t", std::vector<std::string>{"a", "b"}));
    auto any = [](int) { return true; };
    CHECK_THROWS_AS(filter_numeric<int>(df, "x", any), std::invalid_argument);     // 2.5 is not an int
    CHECK_THROWS_AS(filter_numeric<int>(df, "t", any), std::invalid_argument);     // text column
    CHECK_THROWS_AS(filter_numeric<int>(df, "nope", any), std::invalid_argument);  // no such column
}

// --- CSV ----------------------------------------------------------------------

TEST_CASE("read_csv_column<T>: tips.csv as different types") {
    std::vector<int> size = read_csv_column<int>(data_path("tips.csv"), "size");
    CHECK(size.size() == 244);
    CHECK(size[0] == 2);
    CHECK(size[1] == 3);

    std::vector<float> bill = read_csv_column<float>(data_path("tips.csv"), "total_bill");
    CHECK(bill.size() == 244);
    CHECK(bill[0] == doctest::Approx(16.99f));

    std::vector<double> tip = read_csv_column<double>(data_path("tips.csv"), "tip");
    CHECK(tip[0] == doctest::Approx(1.01));

    std::vector<std::uint8_t> small = read_csv_column<std::uint8_t>(data_path("tips.csv"), "size");
    CHECK(small[0] == 2);
}

TEST_CASE("read_csv_column<T>: errors") {
    CHECK_THROWS_AS(read_csv_column<int>(data_path("tips.csv"), "tip"), std::invalid_argument);  // 1.01
    CHECK_THROWS_AS(read_csv_column<int>(data_path("tips.csv"), "sex"), std::invalid_argument);  // text
    CHECK_THROWS_AS(read_csv_column<int>(data_path("tips.csv"), "nope"), std::invalid_argument);
    CHECK_THROWS_AS(read_csv_column<int>("no_such_file.csv", "a"), std::runtime_error);
}

// --- NumericColumn<T> -----------------------------------------------------------

TEST_CASE("NumericColumn<int>: built from a Column") {
    Column c("size", std::vector<double>{2, 3, 4});
    NumericColumn<int> n(c);
    CHECK(n.name() == "size");
    CHECK(n.size() == 3);
    CHECK_FALSE(n.empty());
    CHECK(n[1] == 3);
    CHECK(n.at(2) == 4);
    CHECK(n.sum() == 9.0);
    int total = 0;
    for (int x : n) total += x;
    CHECK(total == 9);
    CHECK_THROWS_AS(n.at(3), std::out_of_range);
}

TEST_CASE("NumericColumn<T>: other types and error cases") {
    Column c("x", std::vector<double>{0.5, 1.5});
    NumericColumn<float> f(c);
    CHECK(f[0] == 0.5f);
    CHECK(f.sum() == doctest::Approx(2.0));

    CHECK_THROWS_AS(NumericColumn<int>{c}, std::invalid_argument);  // 0.5 is not an int
    Column t("t", std::vector<std::string>{"a"});
    CHECK_THROWS_AS(NumericColumn<double>{t}, std::invalid_argument);
}

TEST_CASE("NumericColumn<T>: from values, and back to a Column") {
    NumericColumn<short> n("s", std::vector<short>{1, 2, 3});
    Column c = n.to_column();
    CHECK(c.name() == "s");
    CHECK(c.is_numeric());
    CHECK(c.values() == std::vector<double>{1, 2, 3});
}

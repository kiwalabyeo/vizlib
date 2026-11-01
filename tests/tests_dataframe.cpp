#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>

#include "doctest.h"
#include "vizlib/dataframe.hpp"

using namespace vizlib;

static const double NaN = std::nan("");

TEST_CASE("Column: numeric basics") {
    Column c("x", std::vector<double>{1.0, NaN, 3.0});
    CHECK(c.name() == "x");
    CHECK(c.is_numeric());
    CHECK(c.size() == 3);
    CHECK_FALSE(c.is_missing(0));
    CHECK(c.is_missing(1));
    CHECK(c.count_missing() == 1);
    CHECK(c.values()[2] == 3.0);
    CHECK_THROWS_AS(c.text(), std::invalid_argument);
    CHECK_THROWS_AS(c.is_missing(3), std::out_of_range);
}

TEST_CASE("Column: converts to vector<double> for other modules") {
    Column c("x", std::vector<double>{4.0, 5.0});
    const std::vector<double>& v = c;
    CHECK(v.size() == 2);
    CHECK(v[1] == 5.0);
}

TEST_CASE("Column: text basics") {
    Column c("s", std::vector<std::string>{"a", "", "c"}, std::vector<char>{0, 1, 0});
    CHECK_FALSE(c.is_numeric());
    CHECK(c.size() == 3);
    CHECK(c.is_missing(1));
    CHECK(c.count_missing() == 1);
    CHECK(c.text()[2] == "c");
    CHECK(c.as_string(1) == "");
    CHECK_THROWS_AS(c.values(), std::invalid_argument);
    // conversion to vector<double> on a text column is an error, not garbage
    CHECK_THROWS_AS((void)static_cast<const std::vector<double>&>(c), std::invalid_argument);
}

TEST_CASE("Column: text with no flags means nothing missing; wrong flag length throws") {
    Column c("s", std::vector<std::string>{"", "b"});
    CHECK(c.count_missing() == 0);
    CHECK_THROWS_AS(Column("s", std::vector<std::string>{"a", "b"}, std::vector<char>{0}),
                    std::invalid_argument);
}

TEST_CASE("Column: as_string formats numbers") {
    Column c("x", std::vector<double>{2.5, 3.0, NaN});
    CHECK(c.as_string(0) == "2.5");
    CHECK(c.as_string(1) == "3");
    CHECK(c.as_string(2) == "");
}

TEST_CASE("Column: take picks rows in order") {
    Column n("x", std::vector<double>{10, 20, 30});
    Column t = n.take({2, 0});
    CHECK(t.values() == std::vector<double>{30, 10});

    Column s("s", std::vector<std::string>{"a", "", "c"}, std::vector<char>{0, 1, 0});
    Column u = s.take({1, 2});
    CHECK(u.is_missing(0));
    CHECK(u.text()[1] == "c");
    CHECK_THROWS_AS(n.take({3}), std::out_of_range);
}

TEST_CASE("DataFrame: add and look up columns") {
    DataFrame df;
    CHECK(df.n_rows() == 0);
    CHECK(df.empty());
    df.add_column(Column("a", std::vector<double>{1, 2, 3}));
    df.add_column(Column("b", std::vector<std::string>{"x", "y", "z"}));
    CHECK(df.n_rows() == 3);
    CHECK(df.n_cols() == 2);
    CHECK(df.has_column("a"));
    CHECK_FALSE(df.has_column("zzz"));
    CHECK(df.column_names() == std::vector<std::string>{"a", "b"});
    CHECK(df["a"].values()[1] == 2);
    CHECK(df.column(1).name() == "b");
    CHECK_THROWS_AS(df["zzz"], std::invalid_argument);
    CHECK_THROWS_AS(df.column(2), std::out_of_range);
}

TEST_CASE("DataFrame: rejects duplicate names and uneven columns") {
    DataFrame df;
    df.add_column(Column("a", std::vector<double>{1, 2}));
    CHECK_THROWS_AS(df.add_column(Column("a", std::vector<double>{3, 4})), std::invalid_argument);
    CHECK_THROWS_AS(df.add_column(Column("b", std::vector<double>{1, 2, 3})), std::invalid_argument);
    CHECK(df.n_cols() == 1);  // failed adds change nothing
}

TEST_CASE("DataFrame: replace_column") {
    DataFrame df;
    df.add_column(Column("a", std::vector<std::string>{"1", "2"}));
    df.replace_column(Column("a", std::vector<double>{1, 2}));
    CHECK(df["a"].is_numeric());
    CHECK_THROWS_AS(df.replace_column(Column("nope", std::vector<double>{1, 2})), std::invalid_argument);
    CHECK_THROWS_AS(df.replace_column(Column("a", std::vector<double>{1})), std::invalid_argument);
}

TEST_CASE("DataFrame: take_rows and to_string") {
    DataFrame df;
    df.add_column(Column("a", std::vector<double>{1, 2, 3}));
    df.add_column(Column("b", std::vector<std::string>{"x", "y", "z"}));
    DataFrame sub = df.take_rows({2, 1});
    CHECK(sub.n_rows() == 2);
    CHECK(sub["b"].text()[0] == "z");

    std::string s = df.to_string(2);
    CHECK(s.find("a") != std::string::npos);
    CHECK(s.find("3 rows total") != std::string::npos);
}

TEST_CASE("compatibility: row_count, column_count, get_column") {
    DataFrame df;
    df.add_column(Column("n", std::vector<double>{1, NaN}));
    df.add_column(Column("s", std::vector<std::string>{"a", ""}, std::vector<char>{0, 1}));
    CHECK(df.row_count() == 2);
    CHECK(df.column_count() == 2);

    std::vector<CellValue> n = df.get_column("n");
    CHECK(std::get<double>(n[0]) == 1);
    CHECK(std::isnan(std::get<double>(n[1])));
    const auto& s = df.get_column("s");  // binding to the returned copy is fine
    CHECK(std::get<std::string>(s[0]) == "a");
    CHECK(std::get<std::string>(s[1]) == "");
    CHECK_THROWS_AS(df.get_column("zzz"), std::invalid_argument);
}

TEST_CASE("compatibility: add_column from CellValue vectors") {
    DataFrame df;
    df.add_column("x", std::vector<CellValue>{1.5, 2.5});
    CHECK(df["x"].is_numeric());
    CHECK(df["x"].values()[1] == 2.5);

    df.add_column("mix", std::vector<CellValue>{std::string("a"), 3.0});
    CHECK_FALSE(df["mix"].is_numeric());
    CHECK(df["mix"].text()[1] == "3");

    df.add_column("blank", std::vector<CellValue>{std::string(""), std::string("b")});
    CHECK(df["blank"].is_missing(0));

    CHECK_THROWS_AS(df.add_column("short", std::vector<CellValue>{1.0}), std::invalid_argument);
    CHECK_THROWS_AS(df.add_column("x", std::vector<CellValue>{1.0, 2.0}), std::invalid_argument);
    CHECK(df.n_cols() == 3);
}

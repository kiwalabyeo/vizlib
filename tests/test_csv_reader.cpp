#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

#include "data_path.hpp"
#include "doctest.h"
#include "vizlib/csv_reader.hpp"

using namespace vizlib;

TEST_CASE("read_csv: simple numeric and text columns") {
    DataFrame df = parse_csv("a,b,c\n1,2.5,x\n3,-4e1,y\n");
    CHECK(df.n_rows() == 2);
    CHECK(df.n_cols() == 3);
    CHECK(df["a"].is_numeric());
    CHECK(df["b"].values()[1] == doctest::Approx(-40.0));
    CHECK_FALSE(df["c"].is_numeric());
    CHECK(df["c"].text()[1] == "y");
}

TEST_CASE("read_csv: missing file throws runtime_error") {
    CHECK_THROWS_AS(read_csv("definitely/not/here.csv"), std::runtime_error);
}

TEST_CASE("read_csv: empty input throws invalid_argument") {
    CHECK_THROWS_AS(parse_csv(""), std::invalid_argument);
    CHECK_THROWS_AS(parse_csv("\n\n  \n"), std::invalid_argument);
}

TEST_CASE("read_csv: header only gives zero rows") {
    DataFrame df = parse_csv("a,b\n");
    CHECK(df.n_cols() == 2);
    CHECK(df.n_rows() == 0);
}

TEST_CASE("read_csv: empty cells and NA words are missing") {
    DataFrame df = parse_csv("x,s\n1,a\n,\nNA,NA\n4,d\n");
    CHECK(df["x"].is_numeric());
    CHECK(df["x"].count_missing() == 2);
    CHECK(df["s"].count_missing() == 2);
    CHECK(df["s"].text()[3] == "d");
    CHECK(std::isnan(df["x"].values()[1]));
}

TEST_CASE("read_csv: custom NA list") {
    CsvOptions o;
    o.na_values = {"?"};
    DataFrame df = parse_csv("x\n1\n?\nNA\n", o);
    CHECK_FALSE(df["x"].is_numeric());  // "NA" is no longer missing, so text
    CHECK(df["x"].count_missing() == 1);
}

TEST_CASE("read_csv: quoted fields can contain commas") {
    DataFrame df = parse_csv("name,city\n\"Smith, John\",Kampala\n\"Doe, Jane\",\"Entebbe, UG\"\n");
    CHECK(df.n_cols() == 2);
    CHECK(df["name"].text()[0] == "Smith, John");
    CHECK(df["city"].text()[1] == "Entebbe, UG");
}

TEST_CASE("read_csv: doubled quotes and newlines inside quotes") {
    DataFrame df = parse_csv("t,n\n\"say \"\"hi\"\"\",1\n\"line1\nline2\",2\n");
    CHECK(df["t"].text()[0] == "say \"hi\"");
    CHECK(df["t"].text()[1] == "line1\nline2");
    CHECK(df["n"].values()[1] == 2);
}

TEST_CASE("read_csv: quoted numbers are still numbers") {
    DataFrame df = parse_csv("\"a\",\"b\"\n\"1.5\",\"2\"\n");
    CHECK(df["a"].is_numeric());
    CHECK(df["a"].values()[0] == 1.5);
}

TEST_CASE("read_csv: unterminated quote throws") {
    CHECK_THROWS_AS(parse_csv("a,b\n\"oops,1\n"), std::invalid_argument);
}

TEST_CASE("read_csv: junk after a closing quote throws") {
    CHECK_THROWS_AS(parse_csv("a,b\n\"x\"y,1\n"), std::invalid_argument);
}

TEST_CASE("read_csv: uneven rows throw with the line number") {
    try {
        parse_csv("a,b,c\n1,2,3\n4,5\n");
        FAIL("expected an exception");
    } catch (const std::invalid_argument& e) {
        std::string msg = e.what();
        CHECK(msg.find("line 3") != std::string::npos);
    }
    CHECK_THROWS_AS(parse_csv("a,b\n1,2,3\n"), std::invalid_argument);
}

TEST_CASE("read_csv: BadRowPolicy::Skip drops bad rows") {
    CsvOptions o;
    o.bad_rows = BadRowPolicy::Skip;
    DataFrame df = parse_csv("a,b\n1,2\n3\n4,5\n6,7,8\n", o);
    CHECK(df.n_rows() == 2);
    CHECK(df["a"].values() == std::vector<double>{1, 4});
}

TEST_CASE("read_csv: CRLF, lone CR, blank lines, no trailing newline, BOM") {
    DataFrame df = parse_csv("\xEF\xBB\xBF" "a,b\r\n1,2\r\n\r\n3,4");
    CHECK(df.column_names() == std::vector<std::string>{"a", "b"});
    CHECK(df.n_rows() == 2);
    CHECK(df["b"].values()[1] == 4);

    DataFrame mac = parse_csv("a,b\r1,2\r3,4\r");
    CHECK(mac.n_rows() == 2);
}

TEST_CASE("read_csv: whitespace around unquoted cells is trimmed") {
    DataFrame df = parse_csv("a , b\n 1 , x y \n");
    CHECK(df.has_column("a"));
    CHECK(df.has_column("b"));
    CHECK(df["a"].values()[0] == 1);
    CHECK(df["b"].text()[0] == "x y");
}

TEST_CASE("read_csv: duplicate header names throw") {
    CHECK_THROWS_AS(parse_csv("a,a\n1,2\n"), std::invalid_argument);
}

TEST_CASE("read_csv: empty header name gets a default") {
    DataFrame df = parse_csv("a,,c\n1,2,3\n");
    CHECK(df.has_column("col1"));
}

TEST_CASE("read_csv: no header option") {
    CsvOptions o;
    o.has_header = false;
    DataFrame df = parse_csv("1,2\n3,4\n", o);
    CHECK(df.n_rows() == 2);
    CHECK(df["col0"].values()[1] == 3);
}

TEST_CASE("read_csv: other delimiters") {
    CsvOptions o;
    o.delimiter = ';';
    DataFrame df = parse_csv("a;b\n1,5;2\n", o);  // comma is just text here
    CHECK_FALSE(df["a"].is_numeric());
    CHECK(df["a"].text()[0] == "1,5");
    CHECK(df["b"].values()[0] == 2);
}

TEST_CASE("read_csv: things that look like numbers but are not") {
    DataFrame df = parse_csv("a,b,c,d\n0x1A,inf,1e999,1.2.3\n");
    CHECK_FALSE(df["a"].is_numeric());
    CHECK_FALSE(df["b"].is_numeric());
    CHECK_FALSE(df["c"].is_numeric());
    CHECK_FALSE(df["d"].is_numeric());
}

TEST_CASE("read_csv: one bad cell makes the whole column text") {
    DataFrame df = parse_csv("a\n1\n2\nthree\n");
    CHECK_FALSE(df["a"].is_numeric());
    CHECK(df["a"].text()[0] == "1");
}

TEST_CASE("read_csv: from a file on disk") {
    const char* path = "vizlib_test_tmp.csv";
    {
        std::ofstream out(path);
        out << "x,y\n1,2\n3,4\n";
    }
    DataFrame df = read_csv(path);
    CHECK(df.n_rows() == 2);
    std::remove(path);
}

TEST_CASE("read_csv: sample datasets load") {
    DataFrame iris = read_csv(data_path("iris.csv"));
    CHECK(iris.n_rows() == 150);
    CHECK(iris.n_cols() == 5);
    CHECK(iris["sepal_length"].is_numeric());
    CHECK_FALSE(iris["species"].is_numeric());
    CHECK(iris["sepal_length"].values()[0] == doctest::Approx(5.1));
    CHECK(iris["species"].text()[149] == "virginica");

    DataFrame tips = read_csv(data_path("tips.csv"));
    CHECK(tips.n_rows() == 244);
    CHECK(tips.n_cols() == 7);
    CHECK(tips["total_bill"].is_numeric());  // header and strings are quoted in this file
    CHECK(tips["size"].is_numeric());
    CHECK(tips["sex"].text()[0] == "Female");
    CHECK(tips["total_bill"].values()[0] == doctest::Approx(16.99));
    CHECK(tips["total_bill"].count_missing() == 0);
}

TEST_CASE("read_csv: messy sample file loads with its problems intact") {
    DataFrame df = read_csv(data_path("messy.csv"));
    CHECK(df.n_rows() == 6);
    CHECK(df["name"].text()[0] == "Okello, Peter");
    CHECK(df["score"].count_missing() == 2);
    CHECK(df["group"].count_missing() == 1);
}

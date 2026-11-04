// data_example.cpp - load a CSV, look at it, clean it (Member 1)
#include <iostream>

#include "vizlib/csv_reader.hpp"
#include "vizlib/preprocessing.hpp"

#ifndef VIZLIB_DATA_DIR
#define VIZLIB_DATA_DIR "data/input"
#endif

int main() {
    using namespace vizlib;
    try {
        DataFrame df = read_csv(std::string(VIZLIB_DATA_DIR) + "/messy.csv");
        std::cout << "Loaded " << df.n_rows() << " rows, " << df.n_cols() << " columns\n"
                  << df.to_string() << '\n';

        df = to_numeric(df, "age", /*coerce=*/true);       // "n/a" and "x" become missing
        df = fill_missing_median(df, "age");               // fill them with the median age
        df = drop_missing(df, {"score"});                  // drop rows with no score

        std::cout << "After cleaning: " << df.n_rows() << " rows\n" << df.to_string() << '\n';

        DataFrame tips = read_csv(std::string(VIZLIB_DATA_DIR) + "/tips.csv");
        const std::vector<double>& bill = tips["total_bill"];  // ready for ecdf_plot
        std::cout << "tips.csv: " << bill.size() << " bills, first = " << bill[0] << '\n';

        // Filtering and choosing columns. Every function returns a new table.
        const DataFrame big_tips = filter_numeric(tips, "tip", [](double t) { return t > 5; });
        std::cout << "Tips above 5: " << big_tips.n_rows() << " rows\n";

        const DataFrame dinners = filter_rows(tips, [](const DataFrame& d, std::size_t row) {
            return d["time"].text()[row] == "Dinner";
        });
        std::cout << "Dinner bills: " << dinners.n_rows() << " rows\n";

        const DataFrame money = select_columns(tips, {"total_bill", "tip"});
        const DataFrame no_size = drop_columns(tips, {"size"});
        std::cout << "select_columns keeps " << money.n_cols() << " columns, drop_columns leaves "
                  << no_size.n_cols() << "\n";

        // Missing values: count them, then fill them with the mean or a fixed value.
        DataFrame scores = parse_csv("student,score\nA,70\nB,NA\nC,90\n");
        std::cout << "Missing scores: " << scores["score"].count_missing() << '\n';
        std::cout << "Filled with the mean: " << fill_missing_mean(scores, "score")["score"].values()[1]
                  << ", filled with 0: " << fill_missing(scores, "score", 0.0)["score"].values()[1] << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}

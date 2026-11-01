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
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}

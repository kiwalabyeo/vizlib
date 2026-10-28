# include <iostream>
# include <vector>
# include "ecdf.hpp"

void test_empty_vector(){
    std::vector<double> input = {};
    auto result = vizlib::ECDFPlot::compute_coordinates(input);

if (result.empty()){
    std::cout << "PASSED: empty vector test";
}
else{
    std::cout << "FAILED: empty vector test";
}}
void test_standard_data() {
    std::vector<double> input = {3.0, 1.0, 2.0};
    auto result = vizlib::ECDFPlot::compute_coordinates(input);
    
    // Expected sorted order: 1.0, 2.0, 3.0
    // Expected y values: 1/3, 2/3, 3/3 (1.0)
    if (result.size() == 3 && 
        result[0].x == 1.0 && result[0].y == (1.0 / 3.0) &&
        result[2].x == 3.0 && result[2].y == 1.0) {
        std::cout << "PASSED: test_standard_data\n";
    } else {
        std::cout << "FAILED: test_standard_data\n";
    }
}
int main(){
    std::cout<<"Running ECDF unit tests....";
    test_empty_vector();
    std::cout<<"All tests finished\n";
    return 0;
}

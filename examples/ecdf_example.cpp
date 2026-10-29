#include <iostream>
#include <vector>
#include <vizlib/ecdf.hpp>

int main() {
    std::cout << "ECDF Plot Module Demo\n";

    //  sample data 
    std::vector<double> raw_data = {12.5, 5.0, 20.0, 12.5, 8.0, 15.0};

    std::cout << "Input raw data size: " << raw_data.size() << " data points.\n";

    // Createing theECDFPlot class
    vizlib::ECDFPlot ecdf_plotter;

    // 3. Compute the ECDF coordinates
    std::vector<vizlib::Point> coordinates = ecdf_plotter.compute_coordinates(raw_data);

    // 4. Display the resulting (x, y) step coordinate pairs
    std::cout << "\nComputed ECDF Coordinates (x, cumulative probability y):\n";
    for (std::size_t i = 0; i < coordinates.size(); ++i) {
        std::cout << "Point " << (i + 1) 
                  << " -> x: " << coordinates[i].x 
                  << ", y: " << coordinates[i].y << "\n";
    }
    std::cout << "Demo completed successfully!\n";

    return 0;
}

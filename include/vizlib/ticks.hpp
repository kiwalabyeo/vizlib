
#pragma once

#include <vector>

namespace vizlib {

std::vector<double> generateTicks(double min, double max,
                                  int targetCount = 5);

std::vector<double> generateLogTicks(double min, double max);

}

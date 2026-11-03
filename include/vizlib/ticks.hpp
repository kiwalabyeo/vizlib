#pragma once

#include <string>
#include <vector>

namespace vizlib {

std::vector<double> generateTicks(double min, double max,
                                  int targetCount = 5);

std::vector<double> generateLogTicks(double min, double max);

// The label text for one tick, given all the ticks on that axis. The number of
// decimals follows the gap between ticks, so neighbouring labels never print
// the same (12345 and 12350, not "1.234e+04" twice).
std::string formatTickLabel(double tick, const std::vector<double>& ticks,
                            bool logScale = false);

}

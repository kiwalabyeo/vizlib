#pragma once

#include <string>
#include <vector>

namespace vizlib {

// ticks.hpp - tick values for the axes (Member 5)

// "Nice" tick values between min and max, about targetCount of them, with a
// step of 1, 2 or 5 times a power of ten, because people read 0, 5, 10 faster
// than 0, 3.33, 6.67. targetCount is a target, not a promise:
// generateTicks(0, 100, 10) gives 0, 20, 40, 60, 80, 100.
// Throws std::invalid_argument for NaN, infinity, min >= max or targetCount < 2.
std::vector<double> generateTicks(double min, double max,
                                  int targetCount = 5);

// Powers of 10 between min and max, for a log axis: 1, 10, 100, 1000.
// A range with fewer than two powers of 10 (2 to 5) falls back to
// generateTicks. Throws unless 0 < min < max.
std::vector<double> generateLogTicks(double min, double max);

// The label text for one tick, given all the ticks on that axis. The number of
// decimals follows the gap between ticks, so neighbouring labels never print
// the same (12345 and 12350, not "1.234e+04" twice).
std::string formatTickLabel(double tick, const std::vector<double>& ticks,
                            bool logScale = false);

}

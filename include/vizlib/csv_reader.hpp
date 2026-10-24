#pragma once
#include <string>
#include "vizlib/dataframe.hpp"

namespace vizlib {

DataFrame read_csv(const std::string& filepath);

} // namespace vizlib
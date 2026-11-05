// data_path.hpp - find files in data/input from any build folder (tests only)
#pragma once

#include <fstream>
#include <string>

inline std::string data_path(const std::string& file) {
#ifdef VIZLIB_DATA_DIR
    return std::string(VIZLIB_DATA_DIR) + "/" + file;
#else
    const char* prefixes[] = {"data/input/", "../data/input/", "../../data/input/",
                              "../../../data/input/", "../../../../data/input/"};
    for (const char* p : prefixes) {
        std::string candidate = std::string(p) + file;
        if (std::ifstream(candidate)) return candidate;
    }
    return "data/input/" + file;
#endif
}


#include "vizlib/legend.hpp"

namespace vizlib {

void Legend::add_entry(const std::string& label, Color color) {
    entries_.push_back(LegendEntry{label, color});
}

const std::vector<LegendEntry>& Legend::get_entries() const {
    return entries_;
}

void Legend::clear() {
    entries_.clear();
}

void Legend::set_position(Point position) {
    position_ = position;
}

Point Legend::get_position() const {
    return position_;
}

} // namespace vizlib

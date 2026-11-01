#include "vizlib/csv_reader.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <limits>
#include <set>
#include <sstream>
#include <stdexcept>

#include "vizlib/preprocessing.hpp"

namespace vizlib {

namespace {

struct Field {
    std::string text;
    bool quoted = false;
};

struct Record {
    std::vector<Field> fields;
    std::size_t line = 0;  // line where the record starts (1-based)
};

[[noreturn]] void fail(std::size_t line, const std::string& what) {
    throw std::invalid_argument("CSV error on line " + std::to_string(line) + ": " + what);
}

std::string trim(const std::string& s) {
    const char* ws = " \t";
    std::size_t a = s.find_first_not_of(ws);
    if (a == std::string::npos) return "";
    std::size_t b = s.find_last_not_of(ws);
    return s.substr(a, b - a + 1);
}

// Split the whole text into records of fields. Handles quotes, doubled quotes,
// delimiters and line breaks inside quotes, CRLF / LF / CR line endings.
std::vector<Record> split_records(const std::string& text, char delim) {
    std::vector<Record> records;
    Record rec;
    Field field;
    enum class State { FieldStart, Unquoted, Quoted, AfterQuote } state = State::FieldStart;
    std::size_t line = 1;
    rec.line = 1;
    bool any_content = false;  // anything seen on the current record

    auto end_field = [&]() {
        if (!field.quoted) field.text = trim(field.text);
        rec.fields.push_back(std::move(field));
        field = Field{};
        state = State::FieldStart;
    };
    auto end_record = [&]() {
        end_field();
        // A line with a single empty unquoted cell is a blank line: skip it.
        bool blank = rec.fields.size() == 1 && !rec.fields[0].quoted && rec.fields[0].text.empty();
        if (!blank) records.push_back(std::move(rec));
        rec = Record{};
        any_content = false;
    };

    std::size_t i = 0;
    if (text.compare(0, 3, "\xEF\xBB\xBF") == 0) i = 3;  // UTF-8 byte order mark

    for (; i < text.size(); ++i) {
        const char c = text[i];
        if (!any_content) rec.line = line;
        any_content = true;
        switch (state) {
            case State::FieldStart:
            case State::Unquoted:
                if (c == '"' && state == State::FieldStart) {
                    // Allow leading spaces before an opening quote (already in text).
                    field.quoted = true;
                    field.text.clear();
                    state = State::Quoted;
                } else if (c == '"' && state == State::Unquoted && trim(field.text).empty()) {
                    field.quoted = true;
                    field.text.clear();
                    state = State::Quoted;
                } else if (c == delim) {
                    end_field();
                } else if (c == '\n' || c == '\r') {
                    if (c == '\r' && i + 1 < text.size() && text[i + 1] == '\n') ++i;
                    end_record();
                    ++line;
                } else {
                    field.text.push_back(c);
                    state = State::Unquoted;
                }
                break;
            case State::Quoted:
                if (c == '"') {
                    if (i + 1 < text.size() && text[i + 1] == '"') {
                        field.text.push_back('"');
                        ++i;
                    } else {
                        state = State::AfterQuote;
                    }
                } else {
                    if (c == '\n') ++line;
                    field.text.push_back(c);
                }
                break;
            case State::AfterQuote:
                if (c == delim) {
                    end_field();
                } else if (c == '\n' || c == '\r') {
                    if (c == '\r' && i + 1 < text.size() && text[i + 1] == '\n') ++i;
                    end_record();
                    ++line;
                } else if (c == ' ' || c == '\t') {
                    // trailing spaces after a closing quote are ignored
                } else {
                    fail(line, "unexpected character after closing quote");
                }
                break;
        }
    }
    if (state == State::Quoted) fail(rec.line, "unterminated quoted field");
    if (any_content) end_record();  // last line without a trailing newline
    return records;
}

bool is_missing_cell(const Field& f, const std::set<std::string>& na) {
    if (f.text.empty()) return true;
    return !f.quoted && na.count(f.text) > 0;
}

}  // namespace

DataFrame parse_csv(const std::string& text, const CsvOptions& options) {
    std::vector<Record> records = split_records(text, options.delimiter);
    if (records.empty()) throw std::invalid_argument("CSV input is empty");

    // --- column names ------------------------------------------------------
    const std::size_t n_cols = records.front().fields.size();
    std::vector<std::string> names;
    std::size_t first_data = 0;
    if (options.has_header) {
        for (std::size_t c = 0; c < n_cols; ++c) {
            std::string name = records[0].fields[c].text;
            if (name.empty()) name = "col" + std::to_string(c);
            if (std::find(names.begin(), names.end(), name) != names.end())
                fail(records[0].line, "duplicate column name '" + name + "'");
            names.push_back(std::move(name));
        }
        first_data = 1;
    } else {
        for (std::size_t c = 0; c < n_cols; ++c) names.push_back("col" + std::to_string(c));
    }

    // --- rows --------------------------------------------------------------
    std::vector<const Record*> rows;
    for (std::size_t r = first_data; r < records.size(); ++r) {
        const Record& rec = records[r];
        if (rec.fields.size() != n_cols) {
            if (options.bad_rows == BadRowPolicy::Skip) continue;
            fail(rec.line, "expected " + std::to_string(n_cols) + " fields but found " +
                               std::to_string(rec.fields.size()));
        }
        rows.push_back(&rec);
    }

    // --- columns: infer numeric vs text ------------------------------------
    const std::set<std::string> na(options.na_values.begin(), options.na_values.end());
    DataFrame df;
    for (std::size_t c = 0; c < n_cols; ++c) {
        std::vector<double> numbers;
        numbers.reserve(rows.size());
        bool numeric = true;
        for (const Record* rec : rows) {
            const Field& f = rec->fields[c];
            if (is_missing_cell(f, na)) {
                numbers.push_back(std::numeric_limits<double>::quiet_NaN());
                continue;
            }
            double v;
            if (!parse_number(f.text, v)) {
                numeric = false;
                break;
            }
            numbers.push_back(v);
        }
        if (numeric) {
            df.add_column(Column(names[c], std::move(numbers)));
        } else {
            std::vector<std::string> strings;
            std::vector<char> missing;
            strings.reserve(rows.size());
            missing.reserve(rows.size());
            for (const Record* rec : rows) {
                const Field& f = rec->fields[c];
                bool m = is_missing_cell(f, na);
                strings.push_back(m ? std::string() : f.text);
                missing.push_back(m ? 1 : 0);
            }
            df.add_column(Column(names[c], std::move(strings), std::move(missing)));
        }
    }
    return df;
}

DataFrame read_csv(std::istream& in, const CsvOptions& options) {
    std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return parse_csv(text, options);
}

DataFrame read_csv(const std::string& path, const CsvOptions& options) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open CSV file: " + path);
    return read_csv(file, options);
}

}  // namespace vizlib

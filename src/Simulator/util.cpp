#include "util.h"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

// JSON 키-값 추출 유틸리티 함수 구현
std::string get_val(const std::string& s, const std::string& key) {
    size_t pos = s.find(key);
    if (pos == std::string::npos) return std::string();
    pos = s.find(':', pos);
    if (pos == std::string::npos) return std::string();
    pos++;
    while (pos < s.size() && (s[pos] == ' ' || s[pos] == '"' || s[pos] == '[')) ++pos;
    size_t end = s.find_first_of(",}]\"", pos);
    return s.substr(pos, end - pos);
}

// INI 파일 읽기 유틸리티 함수 구현
std::unordered_map<std::string, std::map<std::string, std::string>> read_ini(const std::string& file_path) {
    std::unordered_map<std::string, std::map<std::string, std::string>> ini_data;
    std::ifstream file(file_path);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open INI file: " + file_path);
    }

    std::string line, current_section;
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // Skip comments and empty lines
        if (line.empty() || line[0] == ';' || line[0] == '#') {
            continue;
        }

        // Section header
        if (line[0] == '[' && line.back() == ']') {
            current_section = line.substr(1, line.size() - 2);
            ini_data[current_section] = {};
        }
        // Key-value pair
        else if (!current_section.empty()) {
            size_t delimiter_pos = line.find('=');
            if (delimiter_pos != std::string::npos) {
                std::string key = line.substr(0, delimiter_pos);
                std::string value = line.substr(delimiter_pos + 1);

                // Trim whitespace
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                ini_data[current_section][key] = value;
            }
        }
    }

    file.close();
    return ini_data;
}

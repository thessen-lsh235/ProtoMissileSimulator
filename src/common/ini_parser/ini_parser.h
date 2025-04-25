#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

class IniParser {
public:
    using Section = std::unordered_map<std::string, std::string>;
    using IniData = std::unordered_map<std::string, Section>;

    // INI 파일을 파싱하여 데이터를 반환
    IniData parse(const std::string& file_path);

    // 특정 섹션의 데이터를 반환
    Section getSection(const std::string& section_name) const;

private:
    IniData ini_data_;
};
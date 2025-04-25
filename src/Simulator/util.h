#pragma once
#include <string>
#include <map>
#include <unordered_map>

// JSON 키-값 추출 유틸리티 함수 선언
std::string get_val(const std::string& s, const std::string& key);

// INI 파일 읽기 유틸리티 함수 선언
std::unordered_map<std::string, std::map<std::string, std::string>> read_ini(const std::string& file_path);

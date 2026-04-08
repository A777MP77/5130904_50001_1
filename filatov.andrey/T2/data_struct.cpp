#include "data_struct.h"
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>

// вспомогательная функция удаляет пробелы в начале и конце
static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

// извлекает значение из строки после ключа
static std::string extractValue(const std::string& str, const std::string& key) {
    std::string search = ":" + key + " ";
    size_t pos = str.find(search);
    if (pos == std::string::npos) return "";
    pos += search.length();
    size_t end = str.find(':', pos);
    if (end == std::string::npos) return "";
    return trim(str.substr(pos, end - pos));
}

// оператор чтения
std::istream& operator>>(std::istream& in, DataStruct& data) {
    // пропускает пробелы и читает символ (
    char ch;
    in >> ch;
    if (ch != '(') {
        in.setstate(std::ios::failbit);
        return in;
    }
    in >> ch;
    if (ch != ':') {
        in.setstate(std::ios::failbit);
        return in;
    }
    // читает всё содержимое до :)
    std::string content;
    std::getline(in, content, ')');  // читаем до закрывающей скобки
    if (!in || content.empty() || content.back() != ':') {
        in.setstate(std::ios::failbit);
        return in;
    }
    content.pop_back(); // убираем последний символ :
    
    // извлекаем значения по ключам
    std::string key1_str = extractValue(content, "key1");
    std::string key2_str = extractValue(content, "key2");
    std::string key3_str = extractValue(content, "key3");
    
    if (key1_str.empty() || key2_str.empty() || key3_str.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }
    
    // парсим key1
    // удаляю суффикс LL, если есть
    if (key1_str.size() >= 2 && key1_str.back() == 'L' && key1_str[key1_str.size()-2] == 'L') {
        key1_str.pop_back();
        key1_str.pop_back();
    }
    try {
        data.key1 = std::stoll(key1_str);
    } catch (...) {
        in.setstate(std::ios::failbit);
        return in;
    }
    
    // парсим key2
    if (key2_str.size() < 5 || key2_str.substr(0,3) != "#c(" || key2_str.back() != ')') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string numbers = key2_str.substr(3, key2_str.size() - 4); // убираем "#c(" и ")"
    std::istringstream iss(numbers);
    double real, imag;
    if (!(iss >> real >> imag)) {
        in.setstate(std::ios::failbit);
        return in;
    }
    data.key2 = std::complex<double>(real, imag);
    
    // парсим key3 (строка в кавычках)
    if (key3_str.size() < 2 || key3_str.front() != '"' || key3_str.back() != '"') {
        in.setstate(std::ios::failbit);
        return in;
    }
    data.key3 = key3_str.substr(1, key3_str.size() - 2);
    
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 " << data.key1;
    out << ":key2 #c(" << std::fixed << std::setprecision(1)
        << data.key2.real() << " " << data.key2.imag() << ")";
    out << ":key3 \"" << data.key3 << "\":)";
    return out;
}

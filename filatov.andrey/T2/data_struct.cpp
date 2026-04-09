#include "data_struct.h"
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

static std::string extractValue(const std::string& str, const std::string& key) {
    std::string search = ":" + key + " ";
    size_t pos = str.find(search);
    if (pos == std::string::npos) return "";
    pos += search.length();
    size_t end = str.find(':', pos);
    if (end == std::string::npos) return "";
    return trim(str.substr(pos, end - pos));
}

std::istream& operator>>(std::istream& in, DataStruct& data) {
    // Пропускаем пробелы и читаем символ '('
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
    // Читаем всё содержимое до ')'
    std::string content;
    std::getline(in, content, ')');
    if (!in) {
        in.setstate(std::ios::failbit);
        return in;
    }
    // Проверяем, что перед ')' был ':'
    if (content.empty() || content.back() != ':') {
        in.setstate(std::ios::failbit);
        return in;
    }
    content.pop_back(); // убираем последний ':'

    std::string key1_str = extractValue(content, "key1");
    std::string key2_str = extractValue(content, "key2");
    std::string key3_str = extractValue(content, "key3");

    if (key1_str.empty() || key2_str.empty() || key3_str.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }

    // Парсим key1 (SLL LIT)
    // Удаляем суффикс LL, если есть
    std::string k1 = key1_str;
    if (k1.size() >= 2 && k1.back() == 'L' && k1[k1.size()-2] == 'L') {
        k1.pop_back();
        k1.pop_back();
    }
    try {
        data.key1 = std::stoll(k1);
    } catch (...) {
        in.setstate(std::ios::failbit);
        return in;
    }

    // Парсим key2 (CMP LSP) формат: #c(real imag)
    if (key2_str.size() < 5 || key2_str.substr(0,3) != "#c(" || key2_str.back() != ')') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string numbers = key2_str.substr(3, key2_str.size() - 4);
    std::istringstream iss(numbers);
    double real, imag;
    if (!(iss >> real >> imag)) {
        in.setstate(std::ios::failbit);
        return in;
    }
    data.key2 = std::complex<double>(real, imag);

    // Парсим key3
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

#include "data_struct.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <stdexcept>

std::string formatHexULL(unsigned long long value) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase << value;
    return oss.str();
}

std::string formatComplex(const std::complex<double>& value) {
    std::ostringstream oss;
    oss << "#c(" << std::fixed << std::setprecision(1)
        << value.real() << " " << value.imag() << ")";
    return oss.str();
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::string line;
    if (!std::getline(in, line)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    try {
        DataStruct temp;

        // Парсим key1 (0x...)
        size_t key1_pos = line.find("key1");
        if (key1_pos == std::string::npos) {
            throw std::runtime_error("key1 not found");
        }

        size_t hex_pos = line.find("0x", key1_pos);
        if (hex_pos == std::string::npos) {
            throw std::runtime_error("0x not found");
        }

        std::string hex_str;
        size_t i = hex_pos + 2;
        while (i < line.size() && std::isxdigit(static_cast<unsigned char>(line[i]))) {
            hex_str += line[i];
            i++;
        }
        if (hex_str.empty()) {
            throw std::runtime_error("Empty hex");
        }
        temp.key1 = std::stoull(hex_str, nullptr, 16);

        // Парсим key2 (#c(real imag))
        size_t key2_pos = line.find("key2");
        if (key2_pos == std::string::npos) {
            throw std::runtime_error("key2 not found");
        }

        size_t complex_pos = line.find("#c(", key2_pos);
        if (complex_pos == std::string::npos) {
            throw std::runtime_error("#c( not found");
        }

        size_t open_paren = complex_pos + 3;

        // Ищем закрывающую скобку
        size_t close_paren = line.find(")", open_paren);
        if (close_paren == std::string::npos) {
            throw std::runtime_error(") not found");
        }

        std::string complex_content = line.substr(open_paren, close_paren - open_paren);

        // Парсим real и imag - могут быть с минусом
        std::string real_str, imag_str;
        bool reading_real = true;
        for (char c : complex_content) {
            if (c == ' ' && reading_real) {
                reading_real = false;
                continue;
            }
            if (reading_real) {
                real_str += c;
            } else {
                imag_str += c;
            }
        }

        // Удаляем пробелы
        while (!real_str.empty() && std::isspace(static_cast<unsigned char>(real_str.front()))) real_str.erase(0, 1);
        while (!real_str.empty() && std::isspace(static_cast<unsigned char>(real_str.back()))) real_str.pop_back();
        while (!imag_str.empty() && std::isspace(static_cast<unsigned char>(imag_str.front()))) imag_str.erase(0, 1);
        while (!imag_str.empty() && std::isspace(static_cast<unsigned char>(imag_str.back()))) imag_str.pop_back();

        if (real_str.empty() || imag_str.empty()) {
            throw std::runtime_error("Empty real or imag");
        }

        double real = std::stod(real_str);
        double imag = std::stod(imag_str);
        temp.key2 = std::complex<double>(real, imag);

        // Парсим key3 ("...")
        size_t key3_pos = line.find("key3");
        if (key3_pos == std::string::npos) {
            throw std::runtime_error("key3 not found");
        }

        size_t quote1 = line.find('"', key3_pos);
        if (quote1 == std::string::npos) {
            throw std::runtime_error("Opening quote not found");
        }

        size_t quote2 = line.find('"', quote1 + 1);
        if (quote2 == std::string::npos) {
            throw std::runtime_error("Closing quote not found");
        }

        temp.key3 = line.substr(quote1 + 1, quote2 - quote1 - 1);

        dest = temp;
        return in;

    } catch (const std::exception&) {
        in.setstate(std::ios::failbit);
        return in;
    }
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    out << "(:key1 " << formatHexULL(src.key1)
        << ":key2 " << formatComplex(src.key2)
        << ":key3 \"" << src.key3 << "\":)";
    return out;
}

#include "IoTypes.h"
#include "IoFmtGuard.h"
#include <string>
#include <iomanip>
#include <cctype>

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c = '0';
    in >> c;
    if (in && (std::tolower(c) != std::tolower(dest.exp))) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, DoubleSciIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    std::string temp = "";
    char c = 0;

    while (std::isspace(in.peek())) {
        in.ignore();
    }

    while (in.get(c) && c != ':') {
        temp += c;
    }
    if (c == ':') {
        in.putback(':');
    }

    size_t dot_pos = temp.find('.');
    size_t e_pos = temp.find_first_of("eE");

    if (dot_pos == std::string::npos || e_pos == std::string::npos || dot_pos > e_pos) {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (e_pos + 1 >= temp.length() || (temp[e_pos + 1] != '+' && temp[e_pos + 1] != '-')) {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (e_pos + 2 >= temp.length() || !std::isdigit(temp[e_pos + 2])) {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (dot_pos == 0 || !std::isdigit(temp[dot_pos - 1])) {
        in.setstate(std::ios::failbit);
        return in;
    }

    try {
        dest.ref = std::stod(temp);
    }
    catch (...) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, LongLongLitIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    if (!(in >> dest.ref)) {
        return in;
    }
    in >> DelimiterIO{ 'l' } >> DelimiterIO{ 'l' };
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    DataStruct input;
    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

    for (int i = 0; i < 3; ++i) {
        std::string key = "";
        in >> key;
        if (key == "key1") {
            in >> DoubleSciIO{ input.key1 };
        }
        else if (key == "key2") {
            in >> LongLongLitIO{ input.key2 };
        }
        else if (key == "key3") {
            in >> DelimiterIO{ '"' };
            std::getline(in, input.key3, '"');
        }
        in >> DelimiterIO{ ':' };
    }
    in >> DelimiterIO{ ')' };

    if (in) {
        dest = input;
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }

    IoFmtGuard guard(out);

    out << std::scientific << std::setprecision(2) << std::nouppercase;

    out << "(:key1 " << src.key1;
    out << ":key2 " << src.key2 << "ll";
    out << ":key3 \"" << src.key3 << "\":)";

    return out;
}

#include "IoTypes.h"
#include <sstream>
#include <cmath>
#include <cctype>

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c;
    in >> c;
    if (in && c != dest.exp) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, DoubleSciIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    std::string token;
    in >> token;
    if (!in) {
        return in;
    }

    size_t ePos = token.find_first_of("eE");
    if (ePos == std::string::npos || ePos == 0 || ePos + 1 >= token.size()) {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::string mantissaStr = token.substr(0, ePos);
    if (mantissaStr.find('.') == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }

    try {
        double mantissa = std::stod(mantissaStr);
        int exponent = std::stoi(token.substr(ePos + 1));
        dest.ref = mantissa * std::pow(10.0, exponent);
        return in;
    } catch (...) {
        in.setstate(std::ios::failbit);
        return in;
    }
}

std::istream& operator>>(std::istream& in, SllLitIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    std::string token;
    in >> token;
    if (!in) {
        return in;
    }

    if (token.size() < 2) {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::string suffix = token.substr(token.size() - 2);
    if (suffix != "LL" && suffix != "ll") {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::string numStr = token.substr(0, token.size() - 2);
    if (numStr.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (size_t i = 0; i < numStr.size(); ++i) {
        if (i == 0 && numStr[i] == '-') continue;
        if (!std::isdigit(static_cast<unsigned char>(numStr[i]))) {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    try {
        dest.ref = std::stoll(numStr);
        return in;
    } catch (...) {
        in.setstate(std::ios::failbit);
        return in;
    }
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, LabelIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    std::string label;
    if ((in >> StringIO{ label }) && label != dest.exp) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

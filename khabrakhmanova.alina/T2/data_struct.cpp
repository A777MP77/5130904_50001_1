#include "data_struct.hpp"
#include "delimiters.hpp"
#include "iofmtguard.hpp"
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <sstream>

namespace nspace
{
    bool compareData(const DataStruct& a, const DataStruct& b)
    {
        if (a.key1 != b.key1)
        {
            return a.key1 < b.key1;
        }

        double modA = std::abs(a.key2);
        double modB = std::abs(b.key2);

        const double epsilon = 1e-10;
        if (std::abs(modA - modB) > epsilon)
        {
            return modA < modB;
        }

        return a.key3.length() < b.key3.length();
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct input;

        try
        {
            char openParen, colon1;
            in >> openParen >> colon1;
            if (openParen != '(' || colon1 != ':')
            {
                in.setstate(std::ios::failbit);
                throw std::runtime_error("Expected '(:' at start of record");
            }

            bool key1Read = false;
            bool key2Read = false;
            bool key3Read = false;

            while (in && (!key1Read || !key2Read || !key3Read))
            {
                std::string label;

                char quote;
                in >> quote;
                if (quote != '"')
                {
                    in.setstate(std::ios::failbit);
                    throw std::runtime_error("Expected quoted label");
                }
                std::getline(in, label, '"');

                char colon;
                in >> colon;
                if (colon != ':')
                {
                    in.setstate(std::ios::failbit);
                    throw std::runtime_error("Expected ':' after label");
                }

                if (label == "key1" && !key1Read)
                {
                    in >> ULLHexIO{input.key1};
                    key1Read = true;
                }
                else if (label == "key2" && !key2Read)
                {
                    in >> ComplexIO{input.key2};
                    key2Read = true;
                }
                else if (label == "key3" && !key3Read)
                {
                    in >> StringIO{input.key3};
                    key3Read = true;
                }
                else
                {
                    in.setstate(std::ios::failbit);
                    throw std::runtime_error("Duplicate or unknown label: " + label);
                }

                char next;
                in >> next;
                if (next == ':')
                {
                    continue;
                }
                else if (next == ')')
                {
                    break;
                }
                else
                {
                    in.setstate(std::ios::failbit);
                    throw std::runtime_error("Expected ':' or ')' after value");
                }
            }

            if (!key1Read || !key2Read || !key3Read)
            {
                in.setstate(std::ios::failbit);
                throw std::runtime_error("Missing required fields");
            }

            dest = std::move(input);
        }
        catch (const std::exception& e)
        {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;

        iofmtguard fmtguard(out);

        out << "(:";

        out << "\"key1\" 0x" << std::hex << std::uppercase << src.key1 << ":";

        out << "\"key2\" #c(" << std::fixed << std::setprecision(1)
            << src.key2.real() << " " << src.key2.imag() << "):";

        out << "\"key3\" \"" << src.key3 << "\":";

        out << ")";

        return out;
    }
}

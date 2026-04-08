#include "data_struct.hpp"
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <cctype>

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

        while (std::isspace(in.peek())) in.get();

        if (in.peek() != '(')
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        in.get();

        if (in.peek() != ':')
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        in.get();

        DataStruct input;
        bool key1Read = false;
        bool key2Read = false;
        bool key3Read = false;

        while (in && (!key1Read || !key2Read || !key3Read))
        {
            std::string label;
            while (std::isalpha(in.peek()))
            {
                label += in.get();
            }

            if (label != "key1" && label != "key2" && label != "key3")
            {
                in.setstate(std::ios::failbit);
                return in;
            }

            if (in.peek() != ':')
            {
                in.setstate(std::ios::failbit);
                return in;
            }
            in.get();

            if (std::isspace(in.peek())) in.get();

            if (label == "key1")
            {
                if (in.peek() != '0')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                in.get();

                if (in.peek() != 'x' && in.peek() != 'X')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                in.get();

                in >> std::hex >> input.key1;
                if (in)
                    key1Read = true;
                else
                    return in;
            }
            else if (label == "key2")
            {
                if (in.peek() != '#')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                in.get();

                if (in.peek() != 'c')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                in.get();

                if (in.peek() != '(')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                in.get();

                double real, imag;
                in >> real >> imag;

                if (in.peek() != ')')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                in.get();

                input.key2 = std::complex<double>(real, imag);
                key2Read = true;
            }
            else if (label == "key3")
            {
                if (in.peek() != '"')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                in.get();

                std::getline(in, input.key3, '"');
                key3Read = true;
            }

            if (std::isspace(in.peek())) in.get();

            if (in.peek() == ':')
            {
                in.get();
                continue;
            }
            else if (in.peek() == ')')
            {
                in.get();
                break;
            }
            else
            {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        if (key1Read && key2Read && key3Read)
        {
            dest = input;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;

        out << "(:";
        out << "key1 0x" << std::hex << std::uppercase << src.key1 << ":";
        out << "key2 #c(" << std::fixed << std::setprecision(1)
            << src.key2.real() << " " << src.key2.imag() << "):";
        out << "key3 \"" << src.key3 << "\":";
        out << ")";

        return out;
    }
}

#include "parser.hpp"
#include <cstdlib>
#include <stdexcept>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <complex>
#include <cmath>

static std::string trimString(const std::string& str)
{
    std::size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos)
    {
        return std::string();
    }
    std::size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

unsigned long long parseUllLiteral(const std::string& valueText)
{
    if (valueText.empty())
    {
        throw std::runtime_error("Empty ULL literal");
    }

    std::string processed = valueText;
    std::string lower = valueText;
    for (char& c : lower) c = std::tolower(c);

    if (lower.size() >= 3 && lower.substr(lower.size() - 3) == "ull")
    {
        processed = valueText.substr(0, valueText.size() - 3);
    }
    else if (lower.size() >= 3 && lower.substr(lower.size() - 3) == "u11")
    {
        processed = valueText.substr(0, valueText.size() - 3);
    }

    if (processed.empty())
    {
        throw std::runtime_error("No numeric part in ULL literal");
    }

    char* conversionEnd = nullptr;
    unsigned long long result = std::strtoull(processed.c_str(), &conversionEnd, 10);

    if (conversionEnd == processed.c_str() || *conversionEnd != '\0')
    {
        throw std::runtime_error("Invalid ULL number format");
    }

    return result;
}

std::complex<double> parseComplexLiteral(const std::string& valueText)
{
    if (valueText.size() < 6 || valueText[0] != '#' || valueText[1] != 'c' || valueText[2] != '(')
    {
        throw std::runtime_error("Invalid complex literal format");
    }

    std::string inner = valueText.substr(3, valueText.size() - 4);

    std::size_t spacePos = inner.find(' ');
    if (spacePos == std::string::npos)
    {
        throw std::runtime_error("Complex literal missing space separator");
    }

    std::string realStr = trimString(inner.substr(0, spacePos));
    std::string imagStr = trimString(inner.substr(spacePos + 1));

    char* endReal = nullptr;
    char* endImag = nullptr;

    double real = std::strtod(realStr.c_str(), &endReal);
    double imag = std::strtod(imagStr.c_str(), &endImag);

    if (endReal == realStr.c_str() || endImag == imagStr.c_str())
    {
        throw std::runtime_error("Failed to parse complex components");
    }

    return std::complex<double>(real, imag);
}

std::string parseQuotedString(const std::string& valueText)
{
    if (valueText.length() < 2)
    {
        throw std::runtime_error("String too short for quotes");
    }

    if (valueText.front() != '"' || valueText.back() != '"')
    {
        throw std::runtime_error("String must be in double quotes");
    }

    return valueText.substr(1, valueText.length() - 2);
}

bool parseRecordString(const std::string& recordStr, DataStruct& data)
{
    std::size_t openPos = recordStr.find("(:");
    std::size_t closePos = recordStr.rfind(":)");

    if (openPos == std::string::npos || closePos == std::string::npos || closePos <= openPos + 1)
    {
        return false;
    }

    std::string inner = recordStr.substr(openPos + 2, closePos - openPos - 2);

    std::vector<std::string> tokens;
    std::size_t start = 0;
    std::size_t colonPos;

    while ((colonPos = inner.find(':', start)) != std::string::npos)
    {
        if (colonPos > start)
        {
            std::string token = trimString(inner.substr(start, colonPos - start));
            if (!token.empty())
            {
                tokens.push_back(token);
            }
        }
        start = colonPos + 1;
    }

    if (start < inner.length())
    {
        std::string token = trimString(inner.substr(start));
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }

    std::string key1Val, key2Val, key3Val;
    bool found1 = false, found2 = false, found3 = false;

    for (const auto& token : tokens)
    {
        std::size_t spacePos = token.find(' ');
        if (spacePos == std::string::npos) continue;

        std::string key = token.substr(0, spacePos);
        std::string val = trimString(token.substr(spacePos + 1));

        if (key == "key1")
        {
            key1Val = val;
            found1 = true;
        }
        else if (key == "key2")
        {
            key2Val = val;
            found2 = true;
        }
        else if (key == "key3")
        {
            key3Val = val;
            found3 = true;
        }
    }

    if (!found1 || !found2 || !found3) return false;

    try
    {
        data.key1 = parseUllLiteral(key1Val);
        data.key2 = parseComplexLiteral(key2Val);
        data.key3 = parseQuotedString(key3Val);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

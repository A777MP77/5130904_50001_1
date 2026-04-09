#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <complex>
#include "data_struct.hpp"

unsigned long long parseUllLiteral(const std::string& valueText);
std::complex<double> parseComplexLiteral(const std::string& valueText);
std::string parseQuotedString(const std::string& valueText);
bool parseRecordString(const std::string& recordStr, DataStruct& data);

#endif

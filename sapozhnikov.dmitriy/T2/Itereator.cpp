#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>
#include <vector>
#include <sstream>

struct DataStruct
{
  unsigned long long key1;
  char key2;
  std::string key3;
};

bool parseLine(const std::string& input, DataStruct& out)
{
   if (input.empty()) return false;
   
   std::string s = input;
   if (s.size() < 4 || s.front() != '(' || s.back() != ')') return false;
   
   std::string content = s.substr(2, s.size() - 4); 
   std::stringstream ss(content);
   std::string part;
   
   bool has1 = false, has2 = false, has3 = false;

   while (std::getline(ss, part, ':')) {
      if (part.empty()) continue;
      
      std::stringstream ps(part);
      std::string key;
      ps >> key;

      if (key == "key1") {
        std::string val;
        ps >> val;
        try {
           size_t suffix = val.find_first_not_of("0123456789-xXabcdefABCDEF");
           if (suffix != std::string::npos) val = val.substr(0, suffix);
           
           out.key1 = std::stoull(val, nullptr, 0);
           has1 = true;
        } catch (...) { return false; }
      }
      else if (key == "key2") {
        char quote, ch, endQuote;
        if (ps >> quote >> ch >> endQuote && quote == '\'' && endQuote == '\'') {
           out.key2 = ch;
           has2 = true;
        } else return false;
      }
      else if (key == "key3") {
        size_t start = part.find('"');
        size_t end = part.find('"', start + 1);
        if (start != std::string::npos && end != std::string::npos) {
           out.key3 = part.substr(start + 1, end - start - 1);
           has3 = true;
        } else return false;
      }
   }

   return has1 && has2 && has3;
}


std::istream& operator>>(std::istream& in, DataStruct& v)
{
  std::string line;
  while (std::getline(in, line))
  {
    DataStruct tmp{};
    if (parseLine(line, tmp))
    {
    v = tmp;
    return in;
    }

  }


  in.setstate(std::ios::failbit);
  return in;
}


std::ostream& operator<<(std::ostream& os, const DataStruct& v)
{
  os << "(:key1 ";

  std::ios::fmtflags f = os.flags();
  os << std::showbase << std::oct << v.key1;
  os.flags(f);

  os << ":key2 '" << v.key2 << "':key3 \"" << v.key3 << "\":)";
  return os;
}

struct DataLess
{
  bool operator()(const DataStruct& a, const DataStruct& b) const
  {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.size() < b.key3.size();
  }
};

int main()
{
  std::cin.imbue(std::locale::classic());
  std::cout.imbue(std::locale::classic());

  std::vector<DataStruct> data;
  bool hadInput = false;

  std::string line;
  while (std::getline(std::cin, line))
  {
    hadInput = true;
    DataStruct tmp{};
    if (parseLine(line, tmp))
    {
    data.push_back(tmp);
    }
  }

  if (data.empty())
  {
    if (hadInput)
    {
    std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
    }
    else
    {
    std::cout << "Atleast one supported record type\n";
    }
    return 0;
  }

  std::sort(data.begin(), data.end(), DataLess{});

  for (const auto& d : data)
  {
    std::cout << d << "\n";
  }

  return 0;
}

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>
#include <vector>

struct DataStruct
{
  unsigned long long key1;
  char key2;
  std::string key3;
};

bool parseLine(const std::string &input, DataStruct &out)
{
  if (input.empty())
    return false;
  std::string s = input;

  if (s.size() < 6 || s.substr(0, 2) != "(:" || s.substr(s.size() - 2) != ":)")
    return false;

  std::size_t i = 2;
  bool has1 = false, has2 = false, has3 = false;

  unsigned long long t1 = 0;
  char t2 = 0;
  std::string t3 = "";

  while (i < s.size() - 2)
  {
    while (i < s.size() - 2 && std::isspace(static_cast<unsigned char>(s[i])))
      i++;
    if (i >= s.size() - 2)
      break;

    std::size_t start = i;
    while (i < s.size() - 2 && std::isalnum(static_cast<unsigned char>(s[i])))
      i++;
    std::string name = s.substr(start, i - start);

    if (i < s.size() && s[i] == ' ')
      i++;
    else
      return false;

    if (name == "key1")
    {
      if (has1)
        return false;
      std::size_t nextCol = s.find(':', i);
      if (nextCol == std::string::npos)
        return false;
      std::string tok = s.substr(i, nextCol - i);
      try
      {
        std::size_t pos = 0;
        t1 = std::stoull(tok, &pos, 8);
        if (pos != tok.size())
          return false;
        has1 = true;
      }
      catch (...)
      {
        return false;
      }
      i = nextCol;
    }
    else if (name == "key2")
    {
      if (has2)
        return false;
      if (i + 2 >= s.size() || s[i] != '\'' || s[i + 2] != '\'')
        return false;
      t2 = s[i + 1];
      i += 3;
      has2 = true;
    }
    else if (name == "key3")
    {
      if (has3)
        return false;
      if (i >= s.size() || s[i] != '"')
        return false;
      std::size_t endq = s.find('"', i + 1);
      if (endq == std::string::npos)
        return false;
      t3 = s.substr(i + 1, endq - i - 1);
      i = endq + 1;
      has3 = true;
    }
    else
      return false;

    if (i < s.size() && s[i] == ':')
      i++;
    else
      return false;
  }

  if (has1 && has2 && has3)
  {
    out.key1 = t1;
    out.key2 = t2;
    out.key3 = t3;
    return true;
  }
  return false;
}

std::istream &operator>>(std::istream &in, DataStruct &v)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;
  std::string line;
  if (std::getline(in, line))
  {
    if (!parseLine(line, v))
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::ostream &operator<<(std::ostream &os, const DataStruct &v)
{
  std::ostream::sentry sentry(os);
  if (!sentry)
    return os;
  os << "(:key1 0" << std::oct << v.key1 << ":key2 '" << v.key2 << "':key3 \"" << v.key3 << "\":)";
  return os;
}

struct DataLess
{
  bool operator()(const DataStruct &a, const DataStruct &b) const
  {
    if (a.key1 != b.key1)
      return a.key1 < b.key1;
    if (a.key2 != b.key2)
      return a.key2 < b.key2;
    return a.key3.size() < b.key3.size();
  }
};

int main()
{
  std::cin.imbue(std::locale::classic());
  std::cout.imbue(std::locale::classic());

  std::vector<DataStruct> data;

  std::cin >> std::ws;
  bool hadInput = (std::cin.peek() != EOF);
  std::cin.clear();

  std::copy(
      std::istream_iterator<DataStruct>(std::cin),
      std::istream_iterator<DataStruct>(),
      std::back_inserter(data));

  if (data.empty())
  {
    if (!hadInput)
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

  std::copy(
      data.begin(),
      data.end(),
      std::ostream_iterator<DataStruct>(std::cout, "\n"));

  return 0;
}

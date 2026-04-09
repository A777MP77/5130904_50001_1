std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::string line;
    std::getline(in, line);
    if (!in) return in;

    auto getValue = [&](const std::string& key) -> std::string {
        size_t p = line.find(":" + key);
        if (p == std::string::npos) return "";
        p += key.size() + 1;
        while (p < line.size() && line[p] == ' ') ++p;
        size_t q = line.find(':', p);
        if (q == std::string::npos) q = line.size();
        return line.substr(p, q - p);
    };

    std::string v1 = getValue("key1");
    std::string v2 = getValue("key2");
    std::string v3 = getValue("key3");
    if (v1.empty() || v2.empty() || v3.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }

    // key1
    if (v1.size() >= 2 && v1.back() == 'L' && v1[v1.size()-2] == 'L')
        v1 = v1.substr(0, v1.size()-2);
    data.key1 = std::stoll(v1);

    // key2: #c(real imag)
    if (v2.substr(0,3) != "#c(" || v2.back() != ')') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string nums = v2.substr(3, v2.size()-4);
    std::istringstream iss(nums);
    double r, i;
    iss >> r >> i;
    data.key2 = {r, i};

    // key3: "string"
    if (v3.front() != '"' || v3.back() != '"') {
        in.setstate(std::ios::failbit);
        return in;
    }
    data.key3 = v3.substr(1, v3.size()-2);

    return in;
}


// Yaroskavsky Oleg 5130904 / 50001 T3

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iomanip>
#include <cmath>

using namespace std::placeholders;

struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

struct Polygon {
    std::vector<Point> points;

    bool operator==(const Polygon& other) const {
        if (points.size() != other.points.size()) return false;

        auto sorted1 = points;
        auto sorted2 = other.points;
        std::sort(sorted1.begin(), sorted1.end(),
            std::bind(std::less<int>(),
                std::bind(&Point::x, _1),
                std::bind(&Point::x, _2)));
        std::sort(sorted2.begin(), sorted2.end(),
            std::bind(std::less<int>(),
                std::bind(&Point::x, _1),
                std::bind(&Point::x, _2)));

        return sorted1 == sorted2;
    }

    bool exactMatch(const Polygon& other) const {
        if (points.size() != other.points.size()) return false;
        for (size_t i = 0; i < points.size(); ++i) {
            if (points[i] != other.points[i]) return false;
        }
        return true;
    }
};

double triangleArea(const Point& a, const Point& b, const Point& c) {
    double ab = std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    double bc = std::sqrt((c.x - b.x) * (c.x - b.x) + (c.y - b.y) * (c.y - b.y));
    double ca = std::sqrt((a.x - c.x) * (a.x - c.x) + (a.y - c.y) * (a.y - c.y));
    double p = (ab + bc + ca) / 2.0;
    return std::sqrt(p * (p - ab) * (p - bc) * (p - ca));
}

double polygonArea(const Polygon& poly) {
    if (poly.points.size() < 3) return 0.0;

    double area = 0.0;
    const Point& p0 = poly.points[0];
    for (size_t i = 2; i < poly.points.size(); ++i) {
        area += triangleArea(p0, poly.points[i - 1], poly.points[i]);
    }
    return area;
}

std::vector<Polygon> readPolygons(const std::string& filename) {
    std::vector<Polygon> polygons;
    std::ifstream file(filename);

    if (!file.is_open()) {
        return polygons;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        int vertexCount;

        if (!(iss >> vertexCount)) {
            continue;
        }

        Polygon poly;
        bool valid = true;

        for (int i = 0; i < vertexCount; ++i) {
            char openParen, semicolon, closeParen;
            int x, y;

            if (!(iss >> openParen >> x >> semicolon >> y >> closeParen) ||
                openParen != '(' || semicolon != ';' || closeParen != ')') {
                valid = false;
                break;
            }

            poly.points.push_back({ x, y });
        }


        if (valid && poly.points.size() == static_cast<size_t>(vertexCount)) {
            polygons.push_back(poly);
        }
    }

    return polygons;
}


int countPermutations(const std::vector<Polygon>& polygons, const Polygon& target) {
    return std::count_if(polygons.begin(), polygons.end(),
        std::bind(std::equal_to<bool>(),
            std::bind(std::equal_to<Polygon>(), _1, target),
            std::bind(std::logical_not<bool>(), std::bind(std::equal_to<bool>(), false, false))));

}

struct IsPermutationOf {
    const Polygon& target;
    IsPermutationOf(const Polygon& t) : target(t) {}

    bool operator()(const Polygon& p) const {
        return p == target;
    }
};

int countPermutationsBind(const std::vector<Polygon>& polygons, const Polygon& target) {
    return std::count_if(polygons.begin(), polygons.end(),
        std::bind(std::equal_to<Polygon>(), _1, target));
}

struct ExactMatchWith {
    const Polygon& target;
    ExactMatchWith(const Polygon& t) : target(t) {}

    bool operator()(const Polygon& p) const {
        return p.exactMatch(target);
    }
};

int maxSequence(const std::vector<Polygon>& polygons, const Polygon& target) {
    if (polygons.empty()) return 0;

    int maxLen = 0;
    int currentLen = 0;

    for (const auto& poly : polygons) {
        if (poly.exactMatch(target)) {
            currentLen++;
            maxLen = std::max(maxLen, currentLen);
        }
        else {
            currentLen = 0;
        }
    }

    return maxLen;
}

int maxSequenceAlgorithms(const std::vector<Polygon>& polygons, const Polygon& target) {
    if (polygons.empty()) return 0;

    std::vector<int> matchFlags(polygons.size());
    std::transform(polygons.begin(), polygons.end(), matchFlags.begin(),
        std::bind(std::equal_to<bool>(),
            std::bind(&Polygon::exactMatch, _1, target),
            std::bind(std::logical_not<bool>(), std::bind(std::equal_to<bool>(), false, false))));

    int maxLen = 0, curr = 0;
    for (bool match : matchFlags) {
        if (match) {
            curr++;
            maxLen = std::max(maxLen, curr);
        }
        else {
            curr = 0;
        }
    }
    return maxLen;
}

bool parsePolygon(const std::string& str, Polygon& poly) {
    std::istringstream iss(str);
    int vertexCount;

    if (!(iss >> vertexCount)) return false;

    poly.points.clear();

    for (int i = 0; i < vertexCount; ++i) {
        char openParen, semicolon, closeParen;
        int x, y;

        if (!(iss >> openParen >> x >> semicolon >> y >> closeParen) ||
            openParen != '(' || semicolon != ';' || closeParen != ')') {
            return false;
        }

        poly.points.push_back({ x, y });
    }

    return poly.points.size() == static_cast<size_t>(vertexCount);
}

void processCommands(std::vector<Polygon>& polygons) {
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "PERMS") {
            std::string polygonStr;
            std::getline(iss, polygonStr);
            size_t start = polygonStr.find_first_not_of(" \t");
            if (start != std::string::npos) {
                polygonStr = polygonStr.substr(start);
            }

            Polygon target;
            if (!parsePolygon(polygonStr, target)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }

            int count = std::count_if(polygons.begin(), polygons.end(),
                std::bind(std::equal_to<Polygon>(), _1, target));
            std::cout << count << std::endl;

        }
        else if (command == "MAXSEQ") {
            std::string polygonStr;
            std::getline(iss, polygonStr);
            size_t start = polygonStr.find_first_not_of(" \t");
            if (start != std::string::npos) {
                polygonStr = polygonStr.substr(start);
            }

            Polygon target;
            if (!parsePolygon(polygonStr, target)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }

            int maxLen = 0;
            int currentLen = 0;

            for (const auto& poly : polygons) {
                if (poly.exactMatch(target)) {
                    currentLen++;
                    if (currentLen > maxLen) maxLen = currentLen;
                }
                else {
                    currentLen = 0;
                }
            }

            std::cout << maxLen << std::endl;

        }
        else {
            std::cout << "<INVALID COMMAND>" << std::endl;
        }
    }
}


int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "Russian");

    if (argc != 2) {
        std::cerr << "Ошибка: не указан файл с фигурами" << std::endl;
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];

    std::vector<Polygon> polygons = readPolygons(filename);

    processCommands(polygons);

    return EXIT_SUCCESS;
}


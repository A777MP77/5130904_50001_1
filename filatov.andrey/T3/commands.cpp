cat > filatov.andrey/T3/commands.cpp << 'EOF'
#include "commands.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

void printInvalid() {
    std::cout << "<INVALID COMMAND>" << std::endl;
}

// ==================== ОБЯЗАТЕЛЬНЫЕ КОМАНДЫ ====================

void doArea(const std::vector<Polygon>& polys, const std::string& param) {
    double result = 0.0;

    if (param == "ODD" || param == "EVEN") {
        bool isOdd = (param == "ODD");
        for (const auto& p : polys) {
            if ((p.getVertexCount() % 2 == 1) == isOdd) {
                result += p.getArea();
            }
        }
    }
    else if (param == "MEAN") {
        if (polys.empty()) {
            printInvalid();
            return;
        }
        double sum = 0.0;
        for (const auto& p : polys) sum += p.getArea();
        result = sum / polys.size();
        std::cout << std::fixed << std::setprecision(1) << result << std::endl;
        return;
    }
    else {
        int n = 0;
        try { n = std::stoi(param); }
        catch (...) { printInvalid(); return; }
        for (const auto& p : polys) {
            if (p.getVertexCount() == n) result += p.getArea();
        }
    }
    std::cout << std::fixed << std::setprecision(1) << result << std::endl;
}

void doMax(const std::vector<Polygon>& polys, const std::string& what) {
    if (polys.empty()) { printInvalid(); return; }

    if (what == "AREA") {
        double maxA = polys[0].getArea();
        for (const auto& p : polys) if (p.getArea() > maxA) maxA = p.getArea();
        std::cout << std::fixed << std::setprecision(1) << maxA << std::endl;
    }
    else if (what == "VERTEXES") {
        int maxV = polys[0].getVertexCount();
        for (const auto& p : polys) if (p.getVertexCount() > maxV) maxV = p.getVertexCount();
        std::cout << maxV << std::endl;
    }
    else printInvalid();
}

void doMin(const std::vector<Polygon>& polys, const std::string& what) {
    if (polys.empty()) { printInvalid(); return; }

    if (what == "AREA") {
        double minA = polys[0].getArea();
        for (const auto& p : polys) if (p.getArea() < minA) minA = p.getArea();
        std::cout << std::fixed << std::setprecision(1) << minA << std::endl;
    }
    else if (what == "VERTEXES") {
        int minV = polys[0].getVertexCount();
        for (const auto& p : polys) if (p.getVertexCount() < minV) minV = p.getVertexCount();
        std::cout << minV << std::endl;
    }
    else printInvalid();
}

void doCount(const std::vector<Polygon>& polys, const std::string& param) {
    int cnt = 0;
    if (param == "EVEN") {
        for (const auto& p : polys) if (p.getVertexCount() % 2 == 0) cnt++;
    }
    else if (param == "ODD") {
        for (const auto& p : polys) if (p.getVertexCount() % 2 == 1) cnt++;
    }
    else {
        int n = 0;
        try { n = std::stoi(param); }
        catch (...) { printInvalid(); return; }
        for (const auto& p : polys) if (p.getVertexCount() == n) cnt++;
    }
    std::cout << cnt << std::endl;
}

// ==================== КОМАНДЫ ВАРИАНТА 9 ====================

void doRmecho(std::vector<Polygon>& polys, const Polygon& sample) {
    int removed = 0;
    size_t i = 0;
    while (i < polys.size()) {
        if (polys[i] == sample) {
            size_t j = i + 1;
            while (j < polys.size() && polys[j] == sample) {
                j++;
                removed++;
            }
            polys.erase(polys.begin() + i + 1, polys.begin() + j);
            i++;
        } else {
            i++;
        }
    }
    std::cout << removed << std::endl;
}

void doIntersections(const std::vector<Polygon>& polys, const Polygon& sample) {
    int count = 0;
    for (const auto& p : polys) {
        bool intersects = false;
        for (const auto& pt1 : p.points) {
            for (const auto& pt2 : sample.points) {
                if (pt1.x == pt2.x && pt1.y == pt2.y) {
                    intersects = true;
                    break;
                }
            }
            if (intersects) break;
        }
        if

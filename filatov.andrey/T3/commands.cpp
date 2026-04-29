#include "commands.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <iterator>

// Вспомогательная функция
void printInvalid() {
    std::cout << "<INVALID COMMAND>" << std::endl;
}

// ====================== ОБЯЗАТЕЛЬНЫЕ КОМАНДЫ ======================

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
        double maxArea = polys[0].getArea();
        for (const auto& p : polys) {
            if (p.getArea() > maxArea) maxArea = p.getArea();
        }
        std::cout << std::fixed << std::setprecision(1) << maxArea << std::endl;
    }
    else if (what == "VERTEXES") {
        int maxV = polys[0].getVertexCount();
        for (const auto& p : polys) {
            if (p.getVertexCount() > maxV) maxV = p.getVertexCount();
        }
        std::cout << maxV << std::endl;
    }
    else printInvalid();
}

void doMin(const std::vector<Polygon>& polys, const std::string& what) {
    if (polys.empty()) { printInvalid(); return; }

    if (what == "AREA") {
        double minArea = polys[0].getArea();
        for (const auto& p : polys) {
            if (p.getArea() < minArea) minArea = p.getArea();
        }
        std::cout << std::fixed << std::setprecision(1) << minArea << std::endl;
    }
    else if (what == "VERTEXES") {
        int minV = polys[0].getVertexCount();
        for (const auto& p : polys) {
            if (p.getVertexCount() < minV) minV = p.getVertexCount();
        }
        std::cout << minV << std::endl;
    }
    else printInvalid();
}

void doCount(const std::vector<Polygon>& polys, const std::string& param) {
    int cnt = 0;

    if (param == "EVEN") {
        for (const auto& p : polys)
            if (p.getVertexCount() % 2 == 0) cnt++;
    }
    else if (param == "ODD") {
        for (const auto& p : polys)
            if (p.getVertexCount() % 2 == 1) cnt++;
    }
    else {
        int n = 0;
        try { n = std::stoi(param); }
        catch (...) { printInvalid(); return; }
        for (const auto& p : polys)
            if (p.getVertexCount() == n) cnt++;
    }
    std::cout << cnt << std::endl;
}

// ====================== КОМАНДЫ ВАРИАНТА 9 ======================

// RMECHO <Polygon> — удаляет идущие подряд дубликаты указанной фигуры
void doRmecho(std::vector<Polygon>& polys, const Polygon& sample) {
    if (polys.empty()) {
        std::cout << 0 << std::endl;
        return;
    }

    int removed = 0;
    auto it = polys.begin();

    while (it != polys.end()) {
        if (*it == sample) {
            auto nextIt = std::next(it);
            while (nextIt != polys.end() && *nextIt == sample) {
                nextIt = polys.erase(nextIt);
                removed++;
            }
            it = nextIt;
        } else {
            ++it;
        }
    }

    std::cout << removed << std::endl;
}

// INTERSECTIONS <Polygon> — считает, сколько фигур пересекаются с данной
void doIntersections(const std::vector<Polygon>& polys, const Polygon& sample) {
    int

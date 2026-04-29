#include "commands.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

void printInvalidCommand() {
    std::cout << "<INVALID COMMAND>" << std::endl;
}

// ====================== ОБЯЗАТЕЛЬНЫЕ КОМАНДЫ ======================

void doArea(const std::vector<Polygon>& polys, const std::string& param) {
    double result = 0.0;

    if (param == "ODD" || param == "EVEN") {
        bool needOdd = (param == "ODD");
        for (const auto& p : polys) {
            if ((p.getVertexCount() % 2 == 1) == needOdd) {
                result += p.getArea();
            }
        }
    }
    else if (param == "MEAN") {
        if (polys.empty()) {
            printInvalidCommand();
            return;
        }
        double sum = 0.0;
        for (const auto& p : polys) {
            sum += p.getArea();
        }
        result = sum / polys.size();
        std::cout << std::fixed << std::setprecision(1) << result << std::endl;
        return;
    }
    else {
        // AREA число
        int n = 0;
        try {
            n = std::stoi(param);
        } catch (...) {
            printInvalidCommand();
            return;
        }
        for (const auto& p : polys) {
            if (p.getVertexCount() == n) {
                result += p.getArea();
            }
        }
    }

    std::cout << std::fixed << std::setprecision(1) << result << std::endl;
}

void doMax(const std::vector<Polygon>& polys, const std::string& what) {
    if (polys.empty()) {
        printInvalidCommand();
        return;
    }

    if (what == "AREA") {
        double maxA = polys[0].getArea();
        for (const auto& p : polys) {
            if (p.getArea() > maxA) maxA = p.getArea();
        }
        std::cout << std::fixed << std::setprecision(1) << maxA << std::endl;
    }
    else if (what == "VERTEXES") {
        int maxV = polys[0].getVertexCount();
        for (const auto& p : polys) {
            if (p.getVertexCount() > maxV) maxV = p.getVertexCount();
        }
        std::cout << maxV << std::endl;
    }
    else {
        printInvalidCommand();
    }
}

void doMin(const std::vector<Polygon>& polys, const std::string& what) {
    if (polys.empty()) {
        printInvalidCommand();
        return;
    }

    if (what == "AREA") {
        double minA = polys[0].getArea();
        for (const auto& p : polys) {
            if (p.getArea() < minA) minA = p.getArea();
        }
        std::cout << std::fixed << std::setprecision(1) << minA << std::endl;
    }
    else if (what == "VERTEXES") {
        int minV = polys[0].getVertexCount();
        for (const auto& p : polys) {
            if (p.getVertexCount() < minV) minV = p.getVertexCount();
        }
        std::cout << minV << std::endl;
    }
    else {
        printInvalidCommand();
    }
}

void doCount(const std::vector<Polygon>& polys, const std::string& param) {
    int cnt = 0;

    if (param == "EVEN") {
        for (const auto& p : polys) {
            if (p.getVertexCount() % 2 == 0) cnt++;
        }
    }
    else if (param == "ODD") {
        for (const auto& p : polys) {
            if (p.getVertexCount() % 2 == 1) cnt++;
        }
    }
    else {
        int n = 0;
        try {
            n = std::stoi(param);
        } catch (...) {
            printInvalidCommand();
            return;
        }
        for (const auto& p : polys) {
            if (p.getVertexCount() == n) cnt++;
        }
    }
    std::cout << cnt << std::endl;
}

// ====================== ДОПОЛНИТЕЛЬНЫЕ КОМАНДЫ ======================

void doLessArea(const std::vector<Polygon>& polys, const Polygon& sample) {
    double sampleArea = sample.getArea();
    int count = 0;
    for (const auto& p : polys) {
        if (p.getArea() < sampleArea) {
            count++;
        }
    }
    std::cout << count << std::endl;
}

void doMaxSeq(const std::vector<Polygon>& polys, const Polygon& sample) {
    int maxLength = 0;
    int current = 0;

    for (const auto& p : polys) {
        if (p == sample) {
            current++;
            if (current > maxLength) maxLength =

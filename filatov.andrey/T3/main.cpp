#include "polygon.h"
#include "commands.h"
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
    // Проверка на наличие имени файла
    if (argc < 2) {
        std::cerr << "Error: no filename given" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        return 1;
    }

    std::vector<Polygon> figures;

    // Читаем фигуры из файла
    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        Polygon poly;
        if (iss >> poly) {
            figures.push_back(poly);
        }
        // некорректные строки просто игнорируем
    }

    // Теперь обрабатываем команды из стандартного ввода
    processCommands(figures);

    return 0;
}

std::vector<Polygon> readPolygons(const std::string& filename) {
    std::vector<Polygon> polygons;
    std::ifstream file(filename);
    if (!file.is_open()) return polygons;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        int n;
        if (!(iss >> n)) continue;
        Polygon poly;
        bool ok = true;
        for (int i = 0; i < n; ++i) {
            char c1, c2, c3;
            int x, y;
            if (!(iss >> c1 >> x >> c2 >> y >> c3) || c1 != '(' || c2 != ';' || c3 != ')') {
                ok = false;
                break;
            }
            poly.points.push_back({x, y});
        }
        // Проверяем, что после прочтения n точек в строке не осталось мусора
        std::string rest;
        if (ok && !(iss >> rest)) {
            // Всё хорошо: rest пуст или только пробелы
            polygons.push_back(poly);
        }
        // Если остались символы (rest не пустой) – фигура некорректна, пропускаем
    }
    return polygons;
}

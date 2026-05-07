void processArea(const std::vector<Polygon>& polygons, const std::string& arg) {
    if (arg == "MEAN") {
        if (polygons.empty()) {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double sum, const Polygon& p) { return sum + polygonArea(p); });
        std::cout << std::fixed << std::setprecision(1) << total / polygons.size() << std::endl;
        return;
    }

    // Для EVEN/ODD/числа – если фигур нет, сумма = 0.0
    if (polygons.empty()) {
        std::cout << "0.0" << std::endl;
        return;
    }

    if (arg == "EVEN" || arg == "ODD") {
        bool isEven = (arg == "EVEN");
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [isEven](double s, const Polygon& p) {
                return s + (((p.points.size() % 2 == 0) == isEven) ? polygonArea(p) : 0.0);
            });
        std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
    } else {
        int vertexCount = std::stoi(arg);
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [vertexCount](double s, const Polygon& p) {
                return s + (p.points.size() == static_cast<size_t>(vertexCount) ? polygonArea(p) : 0.0);
            });
        std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
    }
}

void processCount(const std::vector<Polygon>& polygons, const std::string& arg) {
    if (arg == "EVEN" || arg == "ODD") {
        bool isEven = (arg == "EVEN");
        int cnt = std::count_if(polygons.begin(), polygons.end(),
            [isEven](const Polygon& p) { return (p.points.size() % 2 == 0) == isEven; });
        std::cout << cnt << std::endl;
    } else {
        int vertexCount = std::stoi(arg);
        // Многоугольник должен иметь хотя бы 3 вершины
        if (vertexCount < 3) {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }
        int cnt = std::count_if(polygons.begin(), polygons.end(),
            [vertexCount](const Polygon& p) { return p.points.size() == static_cast<size_t>(vertexCount); });
        std::cout << cnt << std::endl;
    }
}

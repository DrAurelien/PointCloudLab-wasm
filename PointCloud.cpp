#include <emscripten.h>
#include <emscripten/bind.h>

struct Point
{
    double x;
    double y;
    double z;
};

class PointCloud {
public:
    using Index_t = unsigned long long;
    using Size_t = unsigned long long;

    Size_t Size() { return static_cast<Size_t>(m_Coordinates.size() / 3);}
    Point GetPoint(Index_t iIndex) const {
        Index_t shift = iIndex*3;
        return {m_Coordinates[shift++], m_Coordinates[shift++], m_Coordinates[shift++]};
    }
private:
    std::vector<double> m_Coordinates {1, 2, 3};
};

EMSCRIPTEN_BINDINGS(PointCloudLab) {
    emscripten::class_<Point>("Point")
      .constructor()
      .property("x", &Point::x)
      .property("y", &Point::y)
      .property("z", &Point::z);

  emscripten::class_<PointCloud>("PointCloud")
    .constructor()
    .function("Size", &PointCloud::Size)
    .function("GetPoint", &PointCloud::GetPoint);
}
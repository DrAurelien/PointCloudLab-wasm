#include <vector>
#include "model/Vector.hpp"

#pragma once

class PointCloud {
public:
    using Index_t = unsigned long long;
    using Size_t = unsigned long long;

    Size_t Size();
    Point3D GetPoint(Index_t iIndex) const;
private:
    std::vector<double> m_Coordinates;
};
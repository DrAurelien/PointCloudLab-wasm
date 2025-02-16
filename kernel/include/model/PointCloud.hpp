#include <memory>
#include "model/Vector.hpp"

#pragma once

class PointCloud {
public:
    using Index_t = unsigned long long;
    using Size_t = unsigned long long;

    PointCloud();
    PointCloud(PointCloud&&);
    PointCloud(const PointCloud&) = delete;
    ~PointCloud();
    Size_t Size();
    Point3D GetPoint(Index_t iIndex) const;
    void AddPoint(const Point3D& iPoint);
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_Impl;
};
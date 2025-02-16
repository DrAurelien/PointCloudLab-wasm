#include "model/PointCloud.hpp"
#include <vector>

struct PointCloud::Impl
{
    std::vector<double> m_Coordinates; // Stores the coordinates of the points in the cloud.
};

PointCloud::PointCloud() : m_Impl(std::make_unique<Impl>())
{
}

PointCloud::PointCloud(PointCloud&& iOther) : m_Impl(std::move(iOther.m_Impl))
{
    iOther.m_Impl = nullptr; // Ensure the moved-from object is in a valid state.
}
PointCloud::~PointCloud() = default;


PointCloud::Size_t PointCloud::Size()
{
    return static_cast<Size_t>(m_Impl->m_Coordinates.size() / 3);
}

Point3D PointCloud::GetPoint(Index_t iIndex) const {
    Index_t shift = iIndex * 3;
    return Point3D({m_Impl->m_Coordinates[shift++], m_Impl->m_Coordinates[shift++], m_Impl->m_Coordinates[shift++]});
}

void PointCloud::AddPoint(const Point3D& iPoint)
{
    m_Impl->m_Coordinates.push_back(iPoint[0]);
    m_Impl->m_Coordinates.push_back(iPoint[1]);
    m_Impl->m_Coordinates.push_back(iPoint[2]);
}
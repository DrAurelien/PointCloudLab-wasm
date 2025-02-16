#include "model/PointCloud.hpp"

PointCloud::Size_t PointCloud::Size()
{
    return static_cast<Size_t>(m_Coordinates.size() / 3);
}

Point3D PointCloud::GetPoint(Index_t iIndex) const {
    Index_t shift = iIndex * 3;
    return Point3D({m_Coordinates[shift++], m_Coordinates[shift++], m_Coordinates[shift++]});
}
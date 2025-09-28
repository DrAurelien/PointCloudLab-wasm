#pragma once

#include "model/Octree.hpp"

namespace Model
{
class InsideSphereQuery : public Model::IOctreeQuery
{
    public:
        InsideSphereQuery(const Point3D& iCenter, double iRadius)
            : m_Center(iCenter), m_RadiusSquared(iRadius * iRadius)
        {
        }

        bool AddPoint(const Point3D& iPoint) override
        {
            if((iPoint - m_Center).LengthSquared() > m_RadiusSquared)
                return false;
            m_Result.AddPoint(iPoint);
            return true;
        }

        std::optional<double> Eval(const BoundingBox3D& iBox) const override
        {
            // Return the squared distance from the sphere center to the closest point on the bounding box
            Point3D closestPoint {std::clamp(m_Center[0], iBox.m_Min[0], iBox.m_Max[0]),
                                  std::clamp(m_Center[1], iBox.m_Min[1], iBox.m_Max[1]),
                                  std::clamp(m_Center[2], iBox.m_Min[2], iBox.m_Max[2])};
            double sqLength = (closestPoint - m_Center).LengthSquared();
            return sqLength <= m_RadiusSquared ? std::make_optional<double>(sqLength) : std::nullopt;
        }

        bool Accept(const BoundingBox3D& iBox) const override
        {
            return Eval(iBox).value_or(m_RadiusSquared + 1) <= m_RadiusSquared;
        }

        PointCloud GetResult() const override
        {
            return m_Result;
        }
private:
        Point3D m_Center;
        double m_RadiusSquared;
        PointCloud m_Result;
};
}
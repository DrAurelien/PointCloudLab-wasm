#pragma once

#include "model/Octree.hpp"

namespace Model
{
class KnnQuery : public Model::IOctreeQuery
{
    public:
        KnnQuery(const Point3D& iQueryPoint, size_t iK)
            : m_QueryPoint(iQueryPoint), m_K(iK)
        {
        }

        bool AddPoint(const Point3D& iPoint) override
        {
            PointAndDistance newPoint {iPoint, (iPoint - m_QueryPoint).LengthSquared()};
            if(m_CandidatePoints.size() < m_K)
            {
                m_CandidatePoints.push_back(newPoint);
                std::push_heap(m_CandidatePoints.begin(), m_CandidatePoints.end());
                return true;
            }
            else if(newPoint.distanceSquared < m_CandidatePoints.front().distanceSquared)
            {
                std::pop_heap(m_CandidatePoints.begin(), m_CandidatePoints.end());
                m_CandidatePoints.back() = newPoint;
                std::push_heap(m_CandidatePoints.begin(), m_CandidatePoints.end());
                return true;
            }
            return false;
        }

        std::optional<double> Eval(const BoundingBox3D& iBox) const override
        {
            // Return the squared distance from the sphere center to the closest point on the bounding box
            Point3D closestPoint {std::clamp(m_QueryPoint[0], iBox.m_Min[0], iBox.m_Max[0]),
                                  std::clamp(m_QueryPoint[1], iBox.m_Min[1], iBox.m_Max[1]),
                                  std::clamp(m_QueryPoint[2], iBox.m_Min[2], iBox.m_Max[2])};
            double sqLength = (closestPoint - m_QueryPoint).LengthSquared();
            double sqMaxDist = m_CandidatePoints.size() < m_K ? std::numeric_limits<double>::max() : m_CandidatePoints.front().distanceSquared;
            return sqLength <= sqMaxDist ? std::make_optional<double>(sqLength) : std::nullopt;
        }

        bool Accept(const BoundingBox3D& iBox) const override
        {
            return Eval(iBox) != std::nullopt;
        }

        PointCloud GetResult() const override
        {
            PointCloud result;
            for(const PointAndDistance& pd : m_CandidatePoints)
                result.AddPoint(pd.point);
            return result;
        }

private:
        struct PointAndDistance
        {
            Point3D point;
            double distanceSquared;

            bool operator<(const PointAndDistance& other) const
            {
                return distanceSquared < other.distanceSquared;
            }
        };
        Point3D m_QueryPoint;
        size_t m_K;
        std::vector<PointAndDistance> m_CandidatePoints;
};
}
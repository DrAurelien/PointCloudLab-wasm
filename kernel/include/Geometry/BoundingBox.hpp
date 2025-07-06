#include "Geometry/Vector.hpp"

template <typename Scalar_t, Dimension_t Dim>
struct BoundingBox
{
    using Point = Point<Scalar_t, Dim>;
    
    BoundingBox() = default;

    BoundingBox(const Point& iMin, const Point& iMax)
        : m_Min(iMin), m_Max(iMax)
    {
    }

    BoundingBox(const BoundingBox& iOther) = default;

    template<typename PointsCollection>
    BoundingBox(const PointsCollection& iPointsCollections)
    {
        for(const auto& point : iPointsCollections)
            Add(point);
    }

    void Add(const Point& iPoint)
    {
        for (Dimension_t index = 0; index < Dim; ++index)
        {
            m_Min[index] = std::min(m_Min[index], iPoint[index]);
            m_Max[index] = std::max(m_Max[index], iPoint[index]);
        }
    }

    void Add(const BoundingBox& iOther)
    {
        for (Dimension_t index = 0; index < Dim; ++index)
        {
            m_Min[index] = std::min(m_Min[index], iOther.m_Min[index]);
            m_Max[index] = std::max(m_Max[index], iOther.m_Max[index]);
        }
    }
    
    BoundingBox Intersection(const BoundingBox& iOther) const
    {
        BoundingBox result;
        for (Dimension_t index = 0; index < Dim; ++index)
        {
            result.m_Min[index] = std::max(m_Min[index], iOther.m_Min[index]);
            result.m_Max[index] = std::min(m_Max[index], iOther.m_Max[index]);
        }
        return result;
    }

    bool Intersects(const BoundingBox& iOther) const
    {
        for (Dimension_t index = 0; index < Dim; ++index)
        {
            if (m_Max[index] < iOther.m_Min[index] || m_Min[index] > iOther.m_Max[index])
                return false;
        }
        return true;
    }

    bool IsValid() const
    {
        for (Dimension_t index = 0; index < Dim; ++index)
        {
            if (m_Min[index] > m_Max[index])
                return false;
        }
        return true;
    }

    Point Center() const
    {
        Point center;
        for (Dimension_t index = 0; index < Dim; ++index)
        {
            center[index] = (m_Min[index] + m_Max[index]) / 2.0;
        }
        return center;
    }

    Vector<Scalar_t, Dim> Extent() const
    {
        Vector<Scalar_t, Dim> extent;
        for (Dimension_t index = 0; index < Dim; ++index)
        {
            extent[index] = m_Max[index] - m_Min[index];
        }
        return extent;
    }

    operator bool() const
    {
        return IsValid();
    }

    Point m_Min {Point::MaxValue()};
    Point m_Max {Point::MinValue()};
};

using BoundingBox3D = BoundingBox<double, 3>;
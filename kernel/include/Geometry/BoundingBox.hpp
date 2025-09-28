#include "Geometry/Vector.hpp"
#include <tbb/tbb.h>

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
    /*requires(tbb::is_range_v<PointsCollection> && 
             tbb::is_convertible_v<typename PointsCollection::value_type, Point>)*/
    static BoundingBox Compute(const PointsCollection& iPointsCollections)
    {
        return tbb::parallel_reduce(
            iPointsCollections, 
            BoundingBox{},
            [&](const PointsCollection& iSubRange, const BoundingBox& iInitial) -> BoundingBox
            {
                BoundingBox box(iInitial);
                for (const Point& point : iSubRange)
                {
                    box.Add(point);
                }
                return box;
            },
            [&](const BoundingBox& box1, const BoundingBox& box2) -> BoundingBox
            {
                BoundingBox result(box1);
                result.Add(box2);
                return result;
            });
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

    BoundingBox& operator=(const BoundingBox& iOther)
    {
        m_Min = iOther.m_Min;
        m_Max = iOther.m_Max;
        return *this;
    };

    bool Contains(const Point& iPoint) const
    {
        for (Dimension_t index = 0; index < Dim; ++index)
        {
            if (iPoint[index] < m_Min[index] || iPoint[index] > m_Max[index])
                return false;
        }
        return true;
    }

    Point m_Min {Point::MaxValue()};
    Point m_Max {Point::MinValue()};
};

using BoundingBox3D = BoundingBox<double, 3>;
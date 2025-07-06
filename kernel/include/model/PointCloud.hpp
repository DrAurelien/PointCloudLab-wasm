#include <memory>
#include "Geometry/Vector.hpp"

#pragma once

namespace Model
{

class PointCloudSentinel;
class PointCloudIterator;

class PointCloud {
public:
    using Index_t = unsigned long;
    using Size_t = unsigned long long;

    PointCloud();
    PointCloud(PointCloud&& iOther);
    PointCloud(const PointCloud& iOther);
    ~PointCloud();
    Size_t Size();
    const Point3D& GetPoint(Index_t iIndex) const;
    void AddPoint(const Point3D& iPoint);

    PointCloud& operator=(const PointCloud& iOther);
    PointCloud& operator=(PointCloud&& iOther);
    
    PointCloudIterator begin() const;
    PointCloudSentinel end() const;

private:
    struct Impl;
    std::shared_ptr<Impl> m_Impl;

    friend class PointCloudIterator;
};

class PointCloudSentinel {};

class PointCloudIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Point3D;
    using difference_type = std::ptrdiff_t;
    using pointer = const Point3D*;
    using reference = const Point3D&;

    PointCloudIterator(const PointCloud& cloud);
    ~PointCloudIterator();
    reference operator*() const;
    value_type operator->() const;

    PointCloudIterator& operator++();

    bool operator==(const PointCloudSentinel&) const;
    bool operator!=(const PointCloudSentinel&) const;

private:
    struct Impl;
    PointCloudIterator(std::unique_ptr<Impl> impl);
    
    std::unique_ptr<Impl> m_Impl;
};
}
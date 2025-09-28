#include <memory>
#include "Geometry/Vector.hpp"
#pragma once

namespace Model
{

class PointCloudView;
class PointCloudSentinel;
class PointCloudIterator;

class PointCloud {
public:
    using Index_t = size_t;
    using Size_t = size_t;

    PointCloud();
    PointCloud(PointCloud&& iOther);
    PointCloud(const PointCloud& iOther);
    ~PointCloud();
    Size_t Size() const;
    void Reserve(Size_t iSize);
    const Point3D& GetPoint(Index_t iIndex) const;
    void AddPoint(const Point3D& iPoint);
    PointCloud Clone();

    PointCloud& operator=(const PointCloud& iOther);
    PointCloud& operator=(PointCloud&& iOther);
    PointCloud& operator+=(const PointCloud& iOther);

private:
    struct Impl;
    std::shared_ptr<Impl> m_Impl;

    friend class PointCloudView;
};

class PointCloudView
{
public:
    using value_type = Point3D;
    using pointer = const Point3D*;
    using reference = const Point3D&;

    PointCloudView(const PointCloud& iCloud, PointCloud::Index_t iFrom, PointCloud::Index_t iTo, PointCloud::Size_t iGrainSize=DefaultGrainSize());
    PointCloudView(const PointCloud& iCloud, PointCloud::Size_t iGrainSize=DefaultGrainSize());
    PointCloudView(const PointCloudView&);
    template<typename SplitType>
    PointCloudView(PointCloudView& ioViewToSplit, SplitType) // Split constructor
    {
        Split(ioViewToSplit);
    }
    ~PointCloudView();

    bool empty() const;
    bool is_divisible() const;
    PointCloudIterator begin() const;
    PointCloudSentinel end() const;
    size_t size() const;

    static PointCloud::Size_t DefaultGrainSize();
private:
    void Split(PointCloudView& ioViewToSplit);

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

    PointCloudIterator(const PointCloudView& iCloudView);
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
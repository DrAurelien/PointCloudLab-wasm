#include "Model/PointCloud.hpp"
#include <vector>

namespace Model
{
struct PointCloud::Impl
{
    std::vector<Point3D> m_Points; // Stores the coordinates of the points in the cloud.
};

PointCloud::PointCloud() : m_Impl(std::make_shared<Impl>())
{
}

PointCloud::PointCloud(PointCloud&& iOther) : m_Impl(std::move(iOther.m_Impl))
{
    iOther.m_Impl = nullptr; // Ensure the moved-from object is in a valid state.
}

PointCloud::PointCloud(const PointCloud& iOther) : m_Impl(std::move(iOther.m_Impl))
{
}

PointCloud::~PointCloud() = default;


PointCloud::Size_t PointCloud::Size()
{
    return static_cast<Size_t>(m_Impl->m_Points.size());
}

const Point3D& PointCloud::GetPoint(Index_t iIndex) const
{
    return m_Impl->m_Points[iIndex];
}

void PointCloud::AddPoint(const Point3D& iPoint)
{
    m_Impl->m_Points.push_back(iPoint);
}


PointCloud& PointCloud::operator=(const PointCloud& iOther)
{ 
    m_Impl = iOther.m_Impl;
    return *this;
}    

PointCloud& PointCloud::operator=(PointCloud&& iOther)
{
    m_Impl = std::move(iOther.m_Impl);
    iOther.m_Impl = nullptr; // Ensure the moved-from object is in a valid state.
    return *this;
}

PointCloudIterator PointCloud::begin() const
{
    return PointCloudIterator(*this);
}

PointCloudSentinel PointCloud::end() const
{
    return {};
}

//====================================================================
struct PointCloudIterator::Impl
{
    std::shared_ptr<PointCloud::Impl> m_CloudImpl; // Shared pointer to the point cloud implementation.
    std::vector<Point3D>::const_iterator m_Iterator; // Iterator for traversing the points in the cloud.
};

PointCloudIterator::PointCloudIterator(const PointCloud& cloud)
    : m_Impl(std::make_unique<Impl>())
{
    m_Impl->m_CloudImpl = cloud.m_Impl;
    m_Impl->m_Iterator = m_Impl->m_CloudImpl->m_Points.cbegin();
}

PointCloudIterator::~PointCloudIterator() = default;

PointCloudIterator::reference PointCloudIterator::operator*() const
{
    return *m_Impl->m_Iterator;
}

PointCloudIterator::value_type PointCloudIterator::operator->() const
{
    return *m_Impl->m_Iterator;
}

PointCloudIterator& PointCloudIterator::operator++()
{
    ++m_Impl->m_Iterator;
    return *this;
}

bool PointCloudIterator::operator==(const PointCloudSentinel&) const
{
    return m_Impl->m_Iterator == m_Impl->m_CloudImpl->m_Points.cend();
}

bool PointCloudIterator::operator!=(const PointCloudSentinel&) const
{
    return m_Impl->m_Iterator != m_Impl->m_CloudImpl->m_Points.cend();
}
}
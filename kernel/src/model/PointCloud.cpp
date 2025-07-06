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


PointCloud::Size_t PointCloud::Size() const
{
    return static_cast<Size_t>(m_Impl->m_Points.size());
}

void PointCloud::Reserve(Size_t iSize)
{
    m_Impl->m_Points.reserve(iSize);
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

//====================================================================
struct PointCloudView::Impl
{
    std::shared_ptr<PointCloud::Impl> CloudImpl{nullptr}; // Shared pointer to the point cloud implementation.
    PointCloud::Index_t From{0};
    PointCloud::Index_t To{0}; // Range of indices in the point cloud that this view represents.
    PointCloud::Size_t GrainSize{PointCloudView::DefaultGrainSize()}; // Size of the grain for parallel processing.
};

PointCloudView::PointCloudView(const PointCloud& iCloud, PointCloud::Size_t iGrainSize)
:m_Impl(std::make_shared<Impl>())
{
    m_Impl->CloudImpl = iCloud.m_Impl;
    m_Impl->From = 0;
    m_Impl->To = static_cast<PointCloud::Index_t>(iCloud.Size());
    m_Impl->GrainSize = iGrainSize;
}

PointCloudView::PointCloudView(const PointCloud& iCloud, PointCloud::Index_t iFrom, PointCloud::Index_t iTo, PointCloud::Size_t iGrainSize)
:m_Impl(std::make_shared<Impl>())
{
    m_Impl->CloudImpl = iCloud.m_Impl;
    m_Impl->From = iFrom;
    m_Impl->To = iTo;
    m_Impl->GrainSize = iGrainSize;
}

PointCloudView::PointCloudView(const PointCloudView& iOther)
:m_Impl(std::make_shared<Impl>(*iOther.m_Impl))
{   
}

PointCloudView::~PointCloudView() = default;

bool PointCloudView::empty() const
{
    return m_Impl->From >= m_Impl->To;
}

bool PointCloudView::is_divisible() const
{
    return (m_Impl->To - m_Impl->From) > m_Impl->GrainSize;
}

PointCloudIterator PointCloudView::begin() const
{
    return PointCloudIterator(*this);
}

PointCloudSentinel PointCloudView::end() const
{
    return {};
}

size_t PointCloudView::size() const
{
    return static_cast<size_t>(m_Impl->To - m_Impl->From);
}

void PointCloudView::Split(PointCloudView& ioViewToSplit)
{
    m_Impl = std::make_shared<Impl>(*ioViewToSplit.m_Impl);
    m_Impl->To += m_Impl->GrainSize;
    ioViewToSplit.m_Impl->From = m_Impl->To;
}

PointCloud::Size_t PointCloudView::DefaultGrainSize()
{
    return 2048; // Default grain size for parallel processing.
}

//====================================================================
struct PointCloudIterator::Impl
{
    std::shared_ptr<PointCloudView::Impl> m_CloudViewImpl; // Shared pointer to the point cloud implementation.
    PointCloud::Index_t m_CurrentIndex{0}; // Starting index of the iterator.
};

PointCloudIterator::PointCloudIterator(const PointCloudView& iCloudView)
    : m_Impl(std::make_unique<Impl>())
{
    m_Impl->m_CloudViewImpl = iCloudView.m_Impl;
    m_Impl->m_CurrentIndex = m_Impl->m_CloudViewImpl->From;
}

PointCloudIterator::~PointCloudIterator() = default;

PointCloudIterator::reference PointCloudIterator::operator*() const
{
    return m_Impl->m_CloudViewImpl->CloudImpl->m_Points[m_Impl->m_CurrentIndex];
}

PointCloudIterator::value_type PointCloudIterator::operator->() const
{
    return m_Impl->m_CloudViewImpl->CloudImpl->m_Points[m_Impl->m_CurrentIndex];
}

PointCloudIterator& PointCloudIterator::operator++()
{
    ++m_Impl->m_CurrentIndex;
    return *this;
}

bool PointCloudIterator::operator==(const PointCloudSentinel&) const
{
    return m_Impl->m_CurrentIndex >= m_Impl->m_CloudViewImpl->To;
}

bool PointCloudIterator::operator!=(const PointCloudSentinel&) const
{
    return m_Impl->m_CurrentIndex < m_Impl->m_CloudViewImpl->To;
}
}
#pragma once

#include <memory>
#include <vector>
#include "model/PointCloud.hpp"
#include "Geometry/Vector.hpp"
#include "Geometry/BoundingBox.hpp"
#include <optional>

namespace Model
{

class IOctreeQuery
{
public:
    virtual ~IOctreeQuery() noexcept = default;
    virtual bool AddPoint(const Point3D& iPoint) = 0;
    virtual std::optional<double> Eval(const BoundingBox3D& iBox) const = 0;
    virtual bool Accept(const BoundingBox3D& iBox) const = 0;
    virtual PointCloud GetResult() const = 0;
};

class Octree {
public:
    using Size_t = PointCloud::Size_t;

    Octree(const PointCloud& iCloud);
    virtual ~Octree() noexcept;

    PointCloud Query(IOctreeQuery& iQuery) const;
    Size_t Size() const;
    BoundingBox3D BoundingBox() const;

protected:
    struct Node;
    using NodePtr = std::shared_ptr<Node>;

    NodePtr m_Root;
};
}
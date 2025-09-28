#include "Model/Octree.hpp"
#include <vector>
#include <optional>
#include <span>
#include <queue>

namespace Model
{
struct Octree::Node
{
    using Index_t = size_t;
    using Size_t = size_t;

    std::vector<Point3D> m_Innerpoints; // Points that are directly contained in this node.
    BoundingBox3D m_BoundingBox{}; // Cached bounding box for the node.
    std::array<NodePtr, 8> m_Children{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}; // Child nodes of this node.
    Size_t m_Size{0}; // Cached total size including children.
    Index_t m_Depth{0}; // Depth of the node in the octree.

    Node(Index_t iDepth=0) : m_Depth(iDepth) {}
    std::vector<NodePtr> Split(Size_t iLeafSize=30);
    void AddPoint(const Point3D& iPoint);
};

void Octree::Node::AddPoint(const Point3D& iPoint)
{
    m_Innerpoints.push_back(iPoint);
    m_BoundingBox.Add(iPoint);
    m_Size++;
}

std::vector<Octree::NodePtr> Octree::Node::Split(Size_t iLeafSize)
{
    if(m_Innerpoints.size() <= iLeafSize)
        return{};

    Point3D center = m_BoundingBox.Center();
    std::vector<NodePtr> newChildren;
    newChildren.reserve(8);
    for(const Point3D& point : m_Innerpoints)
    {
        Octree::Node::Index_t octantIdx = 0;
        if(point[0] >= center[0]) octantIdx |= 1;
        if(point[1] >= center[1]) octantIdx |= 2;
        if(point[2] >= center[2]) octantIdx |= 4;
        if(!m_Children[octantIdx])
        {
            m_Children[octantIdx] = std::make_shared<Octree::Node>(m_Depth + 1);
            newChildren.push_back(m_Children[octantIdx]);
        }
        m_Children[octantIdx]->AddPoint(point);
    }
    m_Innerpoints = {}; // Clear points as they are now distributed to children.
    return newChildren;
}

Octree::Octree(const PointCloud& iCloud)
{
    m_Root = std::make_shared<Node>(0);
    for(const Point3D& point : PointCloudView(iCloud))
        m_Root->AddPoint(point);

    tbb::concurrent_vector<NodePtr> nodesToProcess;
    nodesToProcess.push_back(m_Root);
    while(!nodesToProcess.empty())
    {
        auto currentNodes = std::exchange(nodesToProcess, {});
        tbb::parallel_for_each(currentNodes, [&](const NodePtr& currentNode)
        {
            std::vector<NodePtr> children = currentNode->Split();
            for(const NodePtr& child : children)
                nodesToProcess.push_back(child);
        });
    }
}

Octree::~Octree() noexcept = default;

PointCloud Octree::Query(IOctreeQuery& iQuery) const
{
    if(!m_Root)
        return iQuery.GetResult();

    struct EvaluatedNode
    {
        NodePtr m_Node;
        double m_Eval;

        bool operator<(const EvaluatedNode& other) const { return m_Eval < other.m_Eval; }
    };
    
    std::priority_queue<EvaluatedNode, std::vector<EvaluatedNode>, std::less<EvaluatedNode>> nodesToProcess;
    auto addNode = [&](const NodePtr& iNode)
    {
        if(iNode)
        {
            std::optional<double> eval = iQuery.Eval(iNode->m_BoundingBox);
            if(eval)
                nodesToProcess.push({iNode, *eval});
        }
    };

    addNode(m_Root);
    while(!nodesToProcess.empty())
    {
        NodePtr currentNode = nodesToProcess.top().m_Node;
        nodesToProcess.pop();

        if(!iQuery.Accept(currentNode->m_BoundingBox))
            continue;

        for(const Point3D& point : currentNode->m_Innerpoints)
            iQuery.AddPoint(point);

        for(const auto& childEval : currentNode->m_Children)
            addNode(childEval);
    }
    return iQuery.GetResult();
}

Octree::Size_t Octree::Size() const
{
    return m_Root ? m_Root->m_Size : 0;
}

BoundingBox3D Octree::BoundingBox() const
{
    return m_Root ? m_Root->m_BoundingBox : BoundingBox3D();
}
}
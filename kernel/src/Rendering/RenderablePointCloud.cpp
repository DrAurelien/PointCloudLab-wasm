#include <vector>
#include <string>
#include <stdexcept>
#include "Model/PointCloud.hpp"
#include "Rendering/RenderablePointCloud.hpp"
#include "Rendering/Buffer.hpp"

namespace Rendering
{

struct RenderablePointCloud::Impl
{
    using CoordsBuffer = Buffer<float>;
    Model::PointCloud m_PointCloud{}; // The point cloud data to be rendered
    std::unique_ptr<CoordsBuffer> m_CoordsBuffer;
};

RenderablePointCloud::RenderablePointCloud(const Model::PointCloud& iPointCloud) :
    m_Impl(std::make_unique<Impl>())
{
    m_Impl->m_PointCloud = iPointCloud;
}

void RenderablePointCloud::PrepareRendering(const DrawingContext& context)
{
    std::vector<float> coords;
    coords.reserve(m_Impl->m_PointCloud.Size() * 3); // Reserve space for x, y, z coordinates
    for(const Point3D& point : m_Impl->m_PointCloud)
    {  
        coords.push_back(point[0]);
        coords.push_back(point[1]);
        coords.push_back(point[2]);
    }
    m_Impl->m_CoordsBuffer = std::make_unique<Impl::CoordsBuffer>(std::move(coords));
}

void RenderablePointCloud::Render(const DrawingContext& context) const
{
    if(!m_Impl->m_CoordsBuffer)
        throw std::runtime_error("RenderablePointCloud: Coordinates buffer is not initialized. Call PrepareRendering first.");
    m_Impl->m_CoordsBuffer->Bind();
    glVertexAttribPointer(context.m_VertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(m_Impl->m_PointCloud.Size()));
}

void RenderablePointCloud::CleanupRendering(const DrawingContext& context)
{}
}
#include "Model/PointCloud.hpp"
#include "Rendering/IRenderable.hpp"
#include <memory>

namespace Rendering
{
class RenderablePointCloud : public IRenderable
{
public:
    RenderablePointCloud(const Model::PointCloud& pointCloud);

    void PrepareRendering(const DrawingContext& context) override;

    void Render(const DrawingContext& context) const override;

    void CleanupRendering(const DrawingContext& context) override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_Impl;
};
}
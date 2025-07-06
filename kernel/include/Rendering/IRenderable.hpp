#include "Rendering/DrawingContext.hpp"

namespace Rendering
{
class IRenderable
{
    virtual void PrepareRendering(const DrawingContext& context) = 0;
    virtual void Render(const DrawingContext& context) const = 0;
    virtual void CleanupRendering(const DrawingContext& context) = 0;
};
}
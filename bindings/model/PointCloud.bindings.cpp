#include "bindings/common.hpp"
#include "model/PointCloud.hpp"

EMSCRIPTEN_BINDINGS(PointCloudLab) {
  emscripten::class_<PointCloud>("PointCloud")
    .constructor()
    .function("Size", &PointCloud::Size)
    .function("GetPoint", &PointCloud::GetPoint);
}
#include "common.hpp"
#include "Model/PointCloud.hpp"
#include "IO/LoadPointCloud.hpp"
#include <sstream>
#include <codecvt>
#include <iostream>


namespace Bindings
{
  // Wrapper for PointCloud to be used in emscripten bindings, that makes point cloud copyable.
  class PointCloudRef
  {
    public:
    using PointCloudPtr = std::shared_ptr<Model::PointCloud>;
    PointCloudRef() : m_PointCloud(std::make_shared<Model::PointCloud>()) {}
    PointCloudRef(Model::PointCloud&& iCloud) : m_PointCloud(std::make_shared<Model::PointCloud>(std::move(iCloud))) {}
    PointCloudRef(const PointCloudRef&) = default;
    ~PointCloudRef() = default;
    PointCloudRef(PointCloudPtr& iPointCloud) : m_PointCloud(iPointCloud) {}

    Point3D GetPoint(size_t iIndex) const { return m_PointCloud->GetPoint(iIndex); }
    size_t Size() const { return m_PointCloud->Size(); }

    PointCloudPtr m_PointCloud;
  };

  PointCloudRef LoadCloudFromCsv(const std::string& iFileContent)
  {
    std::wstring wFileContent = std::wstring(iFileContent.begin(), iFileContent.end());
    std::wstringstream stream(wFileContent);
    try
    {
      return IO::LoadCloudFromCsv(stream, IO::CsvOptions{';'});
    }
    catch(const IO::CSVFormmatingError& e)
    {
      std::cerr << e.what() << '\n';
    }
    return {};
  }
}

EMSCRIPTEN_BINDINGS(PointCloudLab) {
  emscripten::class_<Bindings::PointCloudRef>("PointCloud")
    .constructor()
    .function("Size", &Bindings::PointCloudRef::Size)
    .function("GetPoint", &Bindings::PointCloudRef::GetPoint);

    emscripten::function("LoadCloudFromCsv", &Bindings::LoadCloudFromCsv);
}
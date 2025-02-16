#include "common.hpp"
#include "model/PointCloud.hpp"
#include "io/LoadPointCloud.hpp"
#include <sstream>
#include <codecvt>
#include <iostream>


namespace
{
  // Wrapper for PointCloud to be used in emscripten bindings, that makes point cloud copyable.
  class PointCloudRef
  {
    public:
    using PointCloudPtr = std::shared_ptr<PointCloud>;
    PointCloudRef() : m_PointCloud(std::make_shared<PointCloud>()) {}
    PointCloudRef(PointCloud&& iCloud) : m_PointCloud(std::make_shared<PointCloud>(std::move(iCloud))) {}
    PointCloudRef(const PointCloudRef&) = default;
    ~PointCloudRef() = default;
    PointCloudRef(PointCloudPtr& iPointCloud) : m_PointCloud(iPointCloud) {}

    Point3D GetPoint(size_t iIndex) const { return m_PointCloud->GetPoint(iIndex); }
    size_t Size() const { return m_PointCloud->Size(); }

    PointCloudPtr m_PointCloud;
  };

  PointCloudRef LoadCloudFromCsv(const std::string& iFileContent)
  {
    const bool isUTF8 = false; // Assuming the input is UTF-8 encoded.
    std::wstring wFileContent;
    if(isUTF8)
    {
      // Convert UTF-8 string to wide string using std::wstring_convert
      std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
      wFileContent = converter.from_bytes(iFileContent);
    }
    else
      wFileContent = std::wstring(iFileContent.begin(), iFileContent.end());
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
  emscripten::class_<PointCloudRef>("PointCloud")
    .constructor()
    .function("Size", &PointCloudRef::Size)
    .function("GetPoint", &PointCloudRef::GetPoint);

    emscripten::function("LoadCloudFromCsv", &LoadCloudFromCsv);
}
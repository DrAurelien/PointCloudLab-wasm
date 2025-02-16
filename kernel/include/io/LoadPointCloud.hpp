#include "model/PointCloud.hpp"
#include <istream>

namespace IO
{
    struct CsvOptions
    {
        wchar_t Separator{L';'};
        uint8_t HeaderLinesToIgnore{0};
    };

    class CSVFormmatingError : public std::runtime_error
    {
        public:
        CSVFormmatingError(size_t iLineNumber, const std::string& iMessage) : std::runtime_error(iMessage), m_LineNumber(iLineNumber) {}
        size_t m_LineNumber;
    };

    PointCloud LoadCloudFromCsv(std::wistream& iFilePath, const CsvOptions& iOptions = {});
}
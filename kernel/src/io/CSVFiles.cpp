#include "io/LoadPointCloud.hpp"
#include <string_view>
#include <string>
#include <vector>
#include <istream>
#include <algorithm>
#include <cctype>
#include <optional>
#include <iostream>

namespace IO
{
namespace
{
    class CsvLine
    {
        public:
            CsvLine(std::wstring&& iLine, char iSeparator, size_t iLineNum) :
                m_Line(std::move(iLine)), m_Separator(iSeparator), m_LineNum(iLineNum)
            {
                ParseLine();
            }
            ~CsvLine() = default;

            std::wstring_view GetLine() const { return m_Line; }
            size_t GetLineNumber() const { return m_LineNum+1; }
            size_t GetColumnCount() const { return m_Columns.size(); }
            std::wstring_view GetColumn(size_t iIndex) const {
                if(iIndex >= m_Columns.size())
                    throw CSVFormmatingError(m_LineNum, "Index out of range in CSV column access.");
                return m_Columns[iIndex];
            }

            void ParseLine()
            {
                m_Columns.clear();
                size_t currentPos = 0;
                size_t nextPos = 0;
                while((nextPos = m_Line.find(m_Separator, currentPos)) != std::string::npos)
                {
                    EmplaceTrimmedEntry(currentPos, nextPos);
                    currentPos = nextPos+1;
                }
                if(currentPos<m_Line.size())
                    EmplaceTrimmedEntry(currentPos, m_Line.size());
            }

        private:
            void EmplaceTrimmedEntry(size_t start, size_t end)
            {
                while (start < end && IsWhiteChar(*(m_Line.begin()+start)))
                    start++;
                while (start < end && IsWhiteChar(*(m_Line.begin()+end-1)))
                    end--;
                std::wstring_view trimmed = std::wstring_view(m_Line).substr(start, end-start);
                m_Columns.emplace_back(trimmed);
            }
            static bool IsWhiteChar(wchar_t iChar)
            {
                return iChar == L' ' || iChar == L'\t';
            }
            
        private:
            std::wstring m_Line;
            size_t m_LineNum{0};
            const wchar_t m_Separator{L','};
            std::vector<std::wstring_view> m_Columns;
    };

    struct CsvColum
    {
        enum struct CsvColumType
        {
            X,Y,Z,
            NormalX,NormalY,NormalZ,
            ColorR,ColorG,ColorB,
            Custom
        };
        CsvColumType m_Type{CsvColumType::Custom};
        std::wstring m_Name;
        size_t m_Index{0};

        CsvColum(const std::wstring_view& iName, size_t iIndex) : m_Name(iName), m_Index(iIndex)
        {
            std::wstring name(iName);
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
            if(name == L"x")
                m_Type = CsvColumType::X;
            else if(name == L"y")
                m_Type = CsvColumType::Y;
            else if(name == L"z")
                m_Type = CsvColumType::Z;
            else if(name == L"normalx" || name == L"nx")
                m_Type = CsvColumType::NormalX;
            else if(name == L"normaly" || name == L"ny")
                m_Type = CsvColumType::NormalY;
            else if(name == L"normalz" || name == L"nz")
                m_Type = CsvColumType::NormalZ;
            else if(name == L"r")
                m_Type = CsvColumType::ColorR;
            else if(name == L"g")
                m_Type = CsvColumType::ColorG;
            else if(name == L"b")
                m_Type = CsvColumType::ColorB;
        }

        template<typename T>
        T GetValue(const CsvLine& iLine) const
        {
            if(m_Index >= iLine.GetColumnCount())
                throw CSVFormmatingError(iLine.GetLineNumber(), "Index out of range in CSV column access.");
            try
            {
                if constexpr (std::is_same_v<T, std::wstring>)
                    return std::wstring(iLine.GetColumn(m_Index));
                else if constexpr (std::is_same_v<T, int>)
                    return static_cast<T>(std::stoi(std::wstring(iLine.GetColumn(m_Index))));
                else if constexpr (std::is_same_v<T, float>)
                    return static_cast<T>(std::stof(std::wstring(iLine.GetColumn(m_Index))));
                else if constexpr (std::is_same_v<T, double>)
                    return static_cast<T>(std::stod(std::wstring(iLine.GetColumn(m_Index))));
            }
            catch(const std::invalid_argument& e)
            {
                throw CSVFormmatingError(iLine.GetLineNumber(), "Invalid number format in CSV file: " + std::string(e.what()));
            }
        }
    };

    class CsvParser
    {
        public:
            CsvParser(std::wistream& iStream, const CsvOptions& iOptions) : m_Stream(iStream), m_Options(iOptions) {}
            ~CsvParser() = default;

            PointCloud ReadPointCloud()
            {
                PointCloud result;
                bool headerParsed = false;
                std::wstring lineStr;
                for(size_t lineNum = 0; std::getline(m_Stream, lineStr); ++lineNum)
                {
                    if(lineNum < m_Options.HeaderLinesToIgnore)
                        continue;
                    CsvLine line(std::move(lineStr), m_Options.Separator, lineNum);
                    if(!std::exchange(headerParsed, true) && TryParseHeader(line))
                        continue;
                    if(line.GetColumnCount() == 0)
                        continue; 
                    if(m_NbExpectedColumns && line.GetColumnCount() != *m_NbExpectedColumns)   
                        throw CSVFormmatingError(line.GetLineNumber(), "Invalid number of columns in CSV file. Expected: " + std::to_string(*m_NbExpectedColumns) + ", Found: " + std::to_string(line.GetColumnCount()));
                    
                    Point3D point;
                    for(Point3D::IndexType index=0; index<3; ++index)
                        point[index] = m_PointColumns[index].GetValue<double>(line);
                    result.AddPoint(point);
                }
                return result;
            }

        protected:
            bool TryParseHeader(CsvLine& iLine)
            {
                const size_t columnCount = iLine.GetColumnCount();
                std::array<std::optional<CsvColum>, 3> pointCols;
                std::array<std::optional<CsvColum>, 3> normalCols;
                std::array<std::optional<CsvColum>, 3> colorsCols;
                for(size_t index=0; index<columnCount; ++index)
                {
                    CsvColum col(iLine.GetColumn(index), index);
                    switch (col.m_Type)
                    {
                        case CsvColum::CsvColumType::X:
                            pointCols[0] = std::move(col);
                            break;
                        case CsvColum::CsvColumType::Y:
                            pointCols[1] = std::move(col);
                            break;
                        case CsvColum::CsvColumType::Z:
                            pointCols[2] = std::move(col);
                            break;
                        case CsvColum::CsvColumType::NormalX:
                            normalCols[0] = std::move(col);
                            break;
                        case CsvColum::CsvColumType::NormalY:
                            normalCols[1] = std::move(col);
                            break;
                        case CsvColum::CsvColumType::NormalZ:
                            normalCols[2] = std::move(col);
                            break;
                        case CsvColum::CsvColumType::ColorR:
                            colorsCols[0] = std::move(col);
                            break;
                        case CsvColum::CsvColumType::ColorG:
                            colorsCols[1] = std::move(col);
                            break;
                        case CsvColum::CsvColumType::ColorB:
                            colorsCols[2] = std::move(col);
                            break;  
                        default:
                            m_CustomColumns.push_back(std::move(col));
                            break;
                    }
                }
                if(pointCols[0] && pointCols[1] && pointCols[2])
                {
                    m_PointColumns[0] = std::move(*pointCols[0]);
                    m_PointColumns[1] = std::move(*pointCols[1]);
                    m_PointColumns[2] = std::move(*pointCols[2]);
                    m_NormalColumns = normalCols;
                    m_ColorColumns = colorsCols;
                    m_NbExpectedColumns = columnCount;
                    return true;
                }
                return false;
            }

        private:
        std::wistream& m_Stream;
        CsvOptions m_Options;
        std::array<CsvColum, 3> m_PointColumns = {CsvColum(L"x", 0 ), CsvColum(L"y", 1), CsvColum(L"z", 2)};
        std::array<std::optional<CsvColum>,3> m_NormalColumns = {};
        std::array<std::optional<CsvColum>,3> m_ColorColumns = {};
        std::vector<CsvColum> m_CustomColumns = {};
        std::optional<size_t> m_NbExpectedColumns{};
    };
}

PointCloud LoadCloudFromCsv(std::wistream& iStream, const CsvOptions& iOptions)
{
    CsvParser parser(iStream, iOptions);
    return parser.ReadPointCloud();
}
}
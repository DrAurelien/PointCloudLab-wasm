#include <gtest/gtest.h>
#include <sstream>
#include "io/LoadPointCloud.hpp" // Assuming this is where LoadCloudFromCsv is declared

using namespace IO;

TEST(CSVReaderTests, LoadCloudFromCsv_ValidInput) {
    std::wstring csvData = L"x,y,z\n1.0,2.0,3.0\n4.0,5.0,6.0\n";
    std::wistringstream csvStream(csvData);

    auto pointCloud = LoadCloudFromCsv(csvStream, {L','});

    ASSERT_EQ(pointCloud.Size(), 2);
    auto point1 = pointCloud.GetPoint(0);
    auto point2 = pointCloud.GetPoint(1);
    EXPECT_EQ(point1[0], 1.0);
    EXPECT_EQ(point1[1], 2.0);
    EXPECT_EQ(point1[2], 3.0);
    EXPECT_EQ(point2[0], 4.0);
    EXPECT_EQ(point2[1], 5.0);
    EXPECT_EQ(point2[2], 6.0);
}

TEST(CSVReaderTests, LoadCloudFromCsv_EmptyInput) {
    std::wstring csvData = L"x,y,z\n";
    std::wistringstream csvStream(csvData);

    auto pointCloud = LoadCloudFromCsv(csvStream, {L','});

    ASSERT_EQ(pointCloud.Size(), 0);
}

TEST(CSVReaderTests, LoadCloudFromCsv_InvalidInput) {
    std::wstring csvData = L"x,y,z\n1.0,2.0\n4.0,5.0,6.0\n";
    std::wistringstream csvStream(csvData);

    EXPECT_THROW(LoadCloudFromCsv(csvStream), CSVFormmatingError);
}
#include "Geometry/BoundingBox.hpp"
#include "Model/PointCloud.hpp"
#include <gtest/gtest.h>

TEST(BoundingBoxTests, Validity)
{
    // Bounding box must be built invalid, by default.
    BoundingBox3D bbox;
    EXPECT_FALSE(bbox.IsValid());
    // Adding a point to the bounding box makes it valid.
    bbox.Add({1., 2., 3.});
    EXPECT_TRUE(bbox.IsValid());
    EXPECT_EQ(bbox.m_Min, Point3D({1., 2., 3.}));
    EXPECT_EQ(bbox.m_Max, Point3D({1., 2., 3.}));
}

TEST(BoundingBoxTests, Intersection)
{
    BoundingBox3D bbox1({0., 0., 0.}, {2., 2., 2.});
    BoundingBox3D bbox2({1., 1., 1.}, {3., 3., 3.});
    BoundingBox3D bbox3({4., 4., 4.}, {5., 5., 5.});
    BoundingBox3D invalid;
    EXPECT_TRUE(bbox1.Intersects(bbox2));
    EXPECT_TRUE(bbox2.Intersects(bbox1));
    EXPECT_FALSE(bbox1.Intersects(bbox3));
    EXPECT_FALSE(bbox3.Intersects(bbox1));
    EXPECT_FALSE(bbox2.Intersects(invalid));
    EXPECT_FALSE(invalid.Intersects(bbox2));

    BoundingBox3D intersection = bbox1.Intersection(bbox2);
    EXPECT_TRUE(intersection.IsValid());
    EXPECT_EQ(intersection.m_Min, Point3D({1., 1., 1.}));
    EXPECT_EQ(intersection.m_Max, Point3D({2., 2., 2.}));
}

TEST(BoundingBoxTests, BuildFromPointCloud)
{
    Model::PointCloud pointCloud;
    pointCloud.AddPoint({0., 0., 0.});
    pointCloud.AddPoint({1., 1., 1.});
    pointCloud.AddPoint({2., 2., 2.});

    BoundingBox3D bbox(pointCloud);
    EXPECT_TRUE(bbox.IsValid());
    EXPECT_EQ(bbox.m_Min, Point3D({0., 0., 0.}));
    EXPECT_EQ(bbox.m_Max, Point3D({2., 2., 2.}));
}

TEST(BoundingBoxTests, AddPoint)
{
    BoundingBox3D bbox;
    bbox.Add({1., 2., 3.});
    EXPECT_EQ(bbox.m_Min, Point3D({1., 2., 3.}));
    EXPECT_EQ(bbox.m_Max, Point3D({1., 2., 3.}));

    bbox.Add({0., 1., 2.});
    EXPECT_EQ(bbox.m_Min, Point3D({0., 1., 2.}));
    EXPECT_EQ(bbox.m_Max, Point3D({1., 2., 3.}));

    bbox.Add({4., 5., 6.});
    EXPECT_EQ(bbox.m_Min, Point3D({0., 1., 2.}));
    EXPECT_EQ(bbox.m_Max, Point3D({4., 5., 6.}));
}

TEST(BoundingBoxTests, Union)
{
    BoundingBox3D bbox1({0., 0., 0.}, {2., 2., 2.});
    BoundingBox3D bbox2({1., 1., 1.}, {3., 3., 3.});

    bbox1.Add(bbox2);
    EXPECT_EQ(bbox1.m_Min, Point3D({0., 0., 0.}));
    EXPECT_EQ(bbox1.m_Max, Point3D({3., 3., 3.}));
}

TEST(BoundingBoxTests, CenterAndExtent)
{
    BoundingBox3D bbox({0., 0., 0.}, {2., 2., 2.});
    EXPECT_EQ(bbox.Center(), Point3D({1., 1., 1.}));
    EXPECT_EQ(bbox.Extent(), Vector3D({2., 2., 2.}));
}
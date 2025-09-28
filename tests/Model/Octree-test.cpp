#include "Model/PointCloud.hpp"
#include "Model/Octree.hpp"
#include "Model/InsideSphereQuery.hpp"
#include "Model/KnnQuery.hpp"
#include "Tools/Timer.hpp"
#include <gtest/gtest.h>
#include <tbb/parallel_for.h>
#include <tbb/task_arena.h>
#include <map>

using namespace Model;

static PointCloud GenerateSampleCloud(PointCloud::Size_t iNumPoints, BoundingBox3D iBounds = BoundingBox3D({0., 0., 0.}, {1., 1., 1.}))
{
    PointCloud cloud;
    cloud.Reserve(iNumPoints);
    
    Vector3D extent = iBounds.Extent();
    tbb::concurrent_vector<PointCloud> subClouds(tbb::this_task_arena::max_concurrency());
    tbb::parallel_for(
        tbb::blocked_range<PointCloud::Index_t>(0, iNumPoints, 1024),
        [minPnt = iBounds.m_Min, extent, &subClouds](const tbb::blocked_range<PointCloud::Index_t>& r)
        {
            int index = tbb::this_task_arena::current_thread_index();
            PointCloud cloud = subClouds[index];
            for(PointCloud::Index_t i = r.begin(); i != r.end(); ++i)
            {
                Vector3D vec = {
                    static_cast<double>(rand()) / RAND_MAX * extent[0],
                    static_cast<double>(rand()) / RAND_MAX * extent[1],
                    static_cast<double>(rand()) / RAND_MAX * extent[2]
                };
                cloud.AddPoint(minPnt + vec);
            }
        });
    for(const auto& subCloud : subClouds)
        cloud += subCloud;
    return cloud;
}

class OctreeTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        m_SampleCloud = GenerateSampleCloud(1'000'000);
        Tools::Timer timer;
        m_Octree = std::make_unique<Octree>(m_SampleCloud);
        std::cout << "Octree build time for 1,000,000 points: " << timer.Elapsed() << " ms\n";
    }

protected:
    static PointCloud m_SampleCloud;
    static std::unique_ptr<Octree> m_Octree;
};

PointCloud OctreeTest::m_SampleCloud;
std::unique_ptr<Octree> OctreeTest::m_Octree;

TEST_F(OctreeTest, TestCreation)
{   
    EXPECT_EQ(m_Octree->Size(), m_SampleCloud.Size());
    BoundingBox3D bbox = BoundingBox3D::Compute(Model::PointCloudView(m_SampleCloud));
    EXPECT_EQ(m_Octree->BoundingBox(), bbox);
}

TEST_F(OctreeTest, TestQuerySphere)
{
    Point3D center {0.5, 0.5, 0.5};
    double radius = 0.3;

    PointCloud::Size_t expectedCount = tbb::parallel_reduce(
        Model::PointCloudView(m_SampleCloud),
        PointCloud::Size_t{0},
        [center, radius](const Model::PointCloudView& iSubRange, PointCloud::Size_t iCount) -> PointCloud::Size_t
        {
            for(const Point3D& point : iSubRange)
            {
                if((point - center).LengthSquared() <= radius * radius)
                    iCount++;
            }
            return iCount;  
        },
        std::plus<PointCloud::Size_t>()
    );

    Tools::Timer timer;
    InsideSphereQuery query(center, radius);
    PointCloud result = m_Octree->Query(query);
    std::cout << "Octree query time for sphere query: " << timer.Elapsed() << " ms (got " << result.Size() << " points)\n";

    EXPECT_EQ(result.Size(), expectedCount);
    for(const Point3D& point : PointCloudView(result))
        EXPECT_LE((point - center).LengthSquared(), radius * radius);
}


TEST_F(OctreeTest, TestQueryKnn)
{
    Point3D center {0.5, 0.5, 0.5};
    size_t k = 256;
    std::vector<double> distances = tbb::parallel_reduce(
        Model::PointCloudView(m_SampleCloud),
        std::vector<double>{},
        [center, k](const Model::PointCloudView& iSubRange, std::vector<double>&& iDistances) -> std::vector<double>
        {
            for(const Point3D& point : iSubRange)
                iDistances.push_back((point - center).LengthSquared());
            std::sort(iDistances.begin(), iDistances.end());
            if(iDistances.size() > k)
                iDistances.resize(k);
            return iDistances;
        },
        [k](std::vector<double>&& a, std::vector<double>&& b) {
            a.insert(a.end(), b.begin(), b.end());
            std::sort(a.begin(), a.end());
            if(a.size() > k)
                a.resize(k);
            return a;
        }
    );
    const double expectedMaxDist = distances[k-1];

    Tools::Timer timer;
    KnnQuery query(center, k);
    PointCloud result = m_Octree->Query(query);
    std::cout << "Octree query time for KNN query: " << timer.Elapsed() << " ms (got " << result.Size() << " points)\n";

    EXPECT_EQ(result.Size(), k);
    for(const Point3D& point : PointCloudView(result))
        EXPECT_LE((point - center).LengthSquared(), expectedMaxDist);
}
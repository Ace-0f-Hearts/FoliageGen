//
// Created by ace on 2026-02-11.
//

#define BOOST_TEST_DYN_LINK

#include <unordered_set>
#include <boost/test/unit_test.hpp>

#include <spatial/quadtree.h>

BOOST_AUTO_TEST_SUITE(QuadtreeCheckSuite)


BOOST_AUTO_TEST_CASE(Test_Quadtree_InitialiazionWithZeroPoints)
{
    std::vector<Spatial2D> points = {};
    BoundingBox2D bounding_box = {{-2.2,-2.2},{2.1,2.1}};
    Quadtree quadtree(points, bounding_box);

    auto expected_number_of_nodes = 1;
    auto actual_number_of_nodes = quadtree.node_count();

    auto expected_number_of_items = 0;
    auto actual_number_of_items = quadtree.item_count();

    auto expected_depth = -1;
    auto actual_depth = quadtree.depth();

    BOOST_CHECK_EQUAL(expected_number_of_nodes, actual_number_of_nodes);
    BOOST_CHECK_EQUAL(expected_number_of_items, actual_number_of_items);
    BOOST_CHECK_EQUAL(expected_depth, actual_depth);
}

BOOST_AUTO_TEST_CASE(Test_Quadtree_InitialiazionWithOnlyTwoLayers)
{
    std::vector<Spatial2D> points = {{1, 1}, {-1, -1},  {-2, 1}, };
    BoundingBox2D bounding_box = {{-2.2,-2.2},{2.1,2.1}};
    Quadtree quadtree(points, bounding_box);

    auto expected_number_of_nodes = 1;
    auto actual_number_of_nodes = quadtree.node_count();

    auto expected_depth = 1;
    auto actual_depth = quadtree.depth() ;

    BOOST_CHECK_EQUAL(expected_number_of_nodes, actual_number_of_nodes);
    BOOST_CHECK_EQUAL(expected_depth, actual_depth);
}

BOOST_AUTO_TEST_CASE(Test_Quadtree_InitializationWithOnlyThreeLayers)
{
    std::vector<Spatial2D> points = {{1, 1}, {2, 1}, {1, 2}, {2, 2}, {-1, -1}, {-1, -2}, {-2, -1}, {-2, -2}, {-2, 1}, {-1, 1.7}};
    BoundingBox2D bounding_box = {{-2.2,-2.2},{2.1,2.1}};
    Quadtree quadtree(points, bounding_box);

    auto expected_number_of_nodes = 4;
    auto actual_number_of_nodes = quadtree.node_count();

    auto expected_depth = 2;
    auto actual_depth = quadtree.depth();

    BOOST_CHECK_EQUAL(expected_number_of_nodes, actual_number_of_nodes);
    BOOST_CHECK_EQUAL(expected_depth, actual_depth);
}

BOOST_AUTO_TEST_CASE(Test_Quadtree_InitialiazionComplex)
{
    std::vector<Spatial2D> points = {{1.2, 1},{0.8,1}, {2, 1}, {1, 2}, {2, 2}, {-1, -1}, {-1, -2}, {-2, -1}, {-2, -2}, {-2, 1}, {-1, 1.7}};
    BoundingBox2D bounding_box = {{-2.2,-2.2},{2.1,2.1}};
    Quadtree quadtree(points, bounding_box);
    auto expected_number_of_nodes = 5;
    auto actual_number_of_nodes = quadtree.node_count();

    auto expected_depth = 3;
    auto actual_depth = quadtree.depth();


    BOOST_CHECK_EQUAL(expected_number_of_nodes, actual_number_of_nodes);
    BOOST_CHECK_EQUAL(expected_depth, actual_depth);
}

BOOST_AUTO_TEST_CASE(Test_Quadtree_Equality)
{
    const std::vector<Spatial2D> lhs_points = {{1.2, 1},{0.8,1}, {2, 1}, {1, 2}, {2, 2}, {-1, -1}, {-1, -2}, {-2, -1}, {-2, -2}, {-2, 1}, {-1, 1.7}};
    const std::vector<Spatial2D> rhs_points = { {2, 1}, {1, 2}, {2, 2}, {-1, -1}, {-1, -2}, {-2, -1}, {-2, -2}, {-2, 1}, {-1, 1.7},{1.2, 1},{0.8,1}};
    const BoundingBox2D lhs_bounding_box = {{-2.2,-2.2},{2.1,2.1}};
    const BoundingBox2D rhs_bounding_box = {{-2.2,-2.2},{2.1,2.1}};
    Quadtree lhs_quadtree(lhs_points, lhs_bounding_box);
    Quadtree rhs_quadtree(rhs_points, rhs_bounding_box);

    BOOST_CHECK_EQUAL(lhs_quadtree,rhs_quadtree);

}

BOOST_AUTO_TEST_CASE(Test_Quadtree_FindClosestNeighbour)
{
    const std::vector<Spatial2D> points = {{1.2, 1},{0.8,1}, {2, 1}, {1, 2}, {2, 2}, {-1, -1}, {-1, -2}, {-2, -1}, {-2, -2}, {-2, 1}, {-1, 1.7}};
    const BoundingBox2D bounding_box = {{-2.2,-2.2},{2.1,2.1}};

    Quadtree quadtree(points, bounding_box);

    const Spatial2D query = {-2,1};

    auto expected_neighbour_idx = 10;
    auto actual_neighbour_idx = quadtree.FindNearestNeighbour(query);



    BOOST_CHECK_EQUAL( expected_neighbour_idx, actual_neighbour_idx);
}

BOOST_AUTO_TEST_CASE(Test_Quadtree_FindClosestNeighbourDeeper)
{
    const std::vector<Spatial2D> points = {{1.2, 1},{0.8,1}, {2, 1}, {1, 2}, {2, 2}, {-1, -1}, {-1, -2}, {-2, -1}, {-2, -2}, {-2, 1}, {-1, 1.7}};
    const BoundingBox2D bounding_box = {{-2.2,-2.2},{2.1,2.1}};

    Quadtree quadtree(points, bounding_box);

    const Spatial2D query = {2,1};

    auto expected_neighbour_idx = 0;
    auto actual_neighbour_idx = quadtree.FindNearestNeighbour(query);



    BOOST_CHECK_EQUAL( expected_neighbour_idx, actual_neighbour_idx);
}

BOOST_AUTO_TEST_CASE(Test_Quadtree_FindClosestNeighbours)
{
    const std::vector<Spatial2D> points = {{1.2, 1},{0.8,1}, {2, 1}, {1, 2}, {2, 2}, {-1, -1}, {-1, -2}, {-2, -1}, {-2, -2}, {-2, 1}, {-1, 1.7}};
    const BoundingBox2D bounding_box = {{-2.2,-2.2},{2.1,2.1}};

    Quadtree quadtree(points, bounding_box);

    const Spatial2D query = {2,2};

    std::unordered_set<int> expected_neighbour_idx = {0,1,2,3};
    auto temp = quadtree.FindNearestNeighbours(query,4);
    std::unordered_set<int> actual_neighbour_idx(temp.begin(),temp.end());

    bool res = expected_neighbour_idx == actual_neighbour_idx;
    BOOST_TEST(res);
}


BOOST_AUTO_TEST_SUITE_END()
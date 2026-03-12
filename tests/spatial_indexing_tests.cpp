//
// Created by ace on 2026-02-03.
//

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <spatial/spatial_coordinates.h>
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(SpatialIndexCheckSuite)

BOOST_AUTO_TEST_CASE(Test_Spatial_ClassificationFor2DProducesExpectedBehaviour)
{
    const Spatial::Spatial2D origin({0,0});

    BOOST_CHECK_EQUAL(0,origin.PositionOther({1,1}));
    BOOST_CHECK_EQUAL(1,origin.PositionOther({-1,1}));
    BOOST_CHECK_EQUAL(2,origin.PositionOther({1,-1}));
    BOOST_CHECK_EQUAL(3,origin.PositionOther({-1,-1}));
}

BOOST_AUTO_TEST_CASE(Test_Spatial_PositioningWithIdenticalPointsProducesExpectedBehaviour)
{
    const Spatial::Spatial2D point_0_0{0,0};
    BOOST_CHECK_EQUAL(0,point_0_0.PositionOther({0,0}));
}

BOOST_AUTO_TEST_CASE(Test_Spatial_CopyConstrProducesIdenticalCopy)
{
    Spatial::Spatial2D point_0{0,0};
    Spatial::Spatial2D point_1(point_0);

    BOOST_CHECK_EQUAL(point_0,point_1);
}

BOOST_AUTO_TEST_CASE(Test_Spatial_DeepCopyAtCopyConstr)
{
    Spatial::Spatial2D original{0,0};
    Spatial::Spatial2D deep_copy(original);

    deep_copy += Spatial::Spatial2D({1,1});
    BOOST_CHECK_NE(original,deep_copy);
    BOOST_CHECK_NE(original[0],deep_copy[0]);
    BOOST_CHECK_NE(original[1],deep_copy[1]);
}


BOOST_AUTO_TEST_CASE(Test_Spatial_AdditionProducesExpectedObject)
{
    const Spatial::Spatial2D addition_lhs {0,0};
    const Spatial::Spatial2D addition_rhs {1,1};
    BOOST_CHECK_EQUAL(Spatial::Spatial2D({1,1}), addition_lhs + addition_rhs);
}

BOOST_AUTO_TEST_CASE(Test_Spatial_AdditionAssignmentModifiesObject)
{
    Spatial::Spatial2D point_to_be_modified {0,0};
    point_to_be_modified += Spatial::Spatial2D({1,1});
    BOOST_CHECK_EQUAL(Spatial::Spatial2D({1,1}),point_to_be_modified);
}

BOOST_AUTO_TEST_CASE(Test_Spatial2D_NotEqualPointsCompared)
{
    BOOST_CHECK_NE(Spatial::Spatial2D({1,1}),Spatial::Spatial2D({1,0}));
    BOOST_CHECK_NE(Spatial::Spatial2D({1,1}),Spatial::Spatial2D({0,0}));
    BOOST_CHECK_NE(Spatial::Spatial2D({1,1}),Spatial::Spatial2D({0,1}));
}

BOOST_AUTO_TEST_CASE(Test_Spatial2D_EqualPointsCompared)
{
    BOOST_CHECK_EQUAL(Spatial::Spatial2D({0,0}),Spatial::Spatial2D({0,0}));
}

BOOST_AUTO_TEST_CASE(Test_Spatial_ConstIndexingDoesNotCreateShallowCopy)
{
    const Spatial::Spatial2D original{0,0};
    float f = original[0];
    f = 2;
    BOOST_CHECK_EQUAL(original,Spatial::Spatial2D({0,0}));
}

BOOST_AUTO_TEST_CASE(Test_Spatial_IndexingCreatesShallowCopy)
{
    Spatial::Spatial2D original{0,0};
    float& f = original[0];
    f = 2;
    BOOST_CHECK_EQUAL(original,Spatial::Spatial2D({2,0}));
}

BOOST_AUTO_TEST_SUITE_END()




//
// Created by ace on 2026-02-08.
//

#define BOOST_TEST_MODULE Foliage
#define BOOST_TEST_DYN_LINK

#include <stdexcept>

#include <boost/test/unit_test_log.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>

#include <botanics_math/gaussian.h>
#include <botanics_math/range.h>


BOOST_AUTO_TEST_SUITE(BotanicsMath)

    BOOST_AUTO_TEST_CASE(Test_Range_ConstrNoThrowOnCorrectArgs)
    {

        BOOST_CHECK_NO_THROW(

                Range range(10,20)

        );
    }

    BOOST_AUTO_TEST_CASE(Test_Range_ConstrThrowErrorOnMinLTMax)
    {
        BOOST_CHECK_THROW(
            Range range(20,10),std::range_error
        );
    }


    BOOST_AUTO_TEST_CASE(Test_Gaussian_DeviationSetZeroThrowsException)
{
    BOOST_CHECK_THROW(
        Gaussian gaussian(10,10,0),std::invalid_argument
    );
}

BOOST_AUTO_TEST_SUITE_END()